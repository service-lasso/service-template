// UNCOMPILED native identity/security observations. Raw structures are PRIVATE.
// No SID/path/hash comparison here supplies original owner or trust authority.
#pragma once
#include <windows.h>
#include <array>
namespace template_creator {
struct OriginalTokenResult {
    BOOL result = FALSE;
    DWORD error = 0, required = 0;
    alignas(void*) std::array<unsigned char, 65536> bytes{};
};
struct TokenObservation {
    HANDLE original = nullptr;
    BOOL openResult = FALSE;
    DWORD openError = 0;
    bool closeAttempted = false, closed = false, acquired = false, outputUnknown = false;
    BOOL closeResult = FALSE;
    DWORD closeError = 0;
    OriginalTokenResult user, owner, groups, privileges, integrity, elevation;
    bool attempted = false, captured = false;
    static void Query(HANDLE token, TOKEN_INFORMATION_CLASS kind,
                      OriginalTokenResult& destination) noexcept {
        destination.result = GetTokenInformation(token, kind, destination.bytes.data(),
              static_cast<DWORD>(destination.bytes.size()), &destination.required);
        if (!destination.result) destination.error = GetLastError();
    }
    bool Capture(HANDLE retainedProcess) noexcept {
        if (attempted) return false;
        attempted = true;
        // Destination handle cell exists before OpenProcessToken acquisition.
        openResult = OpenProcessToken(retainedProcess, TOKEN_QUERY, &original);
        if (!openResult) { openError = GetLastError(); outputUnknown = original != nullptr; return false; }
        acquired = original && original != INVALID_HANDLE_VALUE;
        if (!acquired) { outputUnknown = true; return false; }
        Query(original, TokenUser, user); Query(original, TokenOwner, owner);
        Query(original, TokenGroups, groups); Query(original, TokenPrivileges, privileges);
        Query(original, TokenIntegrityLevel, integrity); Query(original, TokenElevation, elevation);
        captured = user.result && owner.result && groups.result && privileges.result &&
                    integrity.result && elevation.result;
        return captured;
    }
    void Dispose() noexcept {
        if (!acquired || closeAttempted) return;
        closeAttempted = true;
        closeResult = CloseHandle(original);
        if (closeResult) closed = true;
        else closeError = GetLastError();
    }
};
struct FileIdentityObservation {
    HANDLE borrowedOriginal = nullptr;
    FILE_ID_INFO id{};
    FILE_BASIC_INFO basic{};
    FILE_STANDARD_INFO standard{};
    BOOL idResult = FALSE, basicResult = FALSE, standardResult = FALSE;
    DWORD idError = 0, basicError = 0, standardError = 0;
    std::array<wchar_t, 32768> finalPath{};
    DWORD pathResult = 0, pathError = 0;
    bool pathTooSmall = false;
    alignas(void*) std::array<unsigned char, 65536> security{};
    BOOL securityResult = FALSE;
    DWORD securityRequired = 0, securityError = 0;
    bool attempted = false, captured = false;
    bool Capture(HANDLE heldFile) noexcept {
        if (attempted) return false;
        attempted = true;
        borrowedOriginal = heldFile;
        idResult = GetFileInformationByHandleEx(heldFile, FileIdInfo, &id, sizeof(id));
        if (!idResult) idError = GetLastError();
        basicResult = GetFileInformationByHandleEx(heldFile, FileBasicInfo, &basic, sizeof(basic));
        if (!basicResult) basicError = GetLastError();
        standardResult = GetFileInformationByHandleEx(heldFile, FileStandardInfo, &standard, sizeof(standard));
        if (!standardResult) standardError = GetLastError();
        pathResult = GetFinalPathNameByHandleW(heldFile, finalPath.data(),
            static_cast<DWORD>(finalPath.size()), FILE_NAME_NORMALIZED | VOLUME_NAME_GUID);
        if (!pathResult) pathError = GetLastError();
        else pathTooSmall = pathResult >= finalPath.size();
        securityResult = GetKernelObjectSecurity(heldFile,
            OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION,
            reinterpret_cast<PSECURITY_DESCRIPTOR>(security.data()),
            static_cast<DWORD>(security.size()), &securityRequired);
        if (!securityResult) securityError = GetLastError();
        captured = idResult && basicResult && standardResult && securityResult &&
                   pathResult && pathResult < finalPath.size();
        return captured;
    }
};
// These retain actual API results rather than six trusted preflight Booleans.
// The concrete gate must still validate original owner receipts, SID/ACL access,
// source/root exclusion and object chains; raw results alone are not acceptance.
} // namespace template_creator
