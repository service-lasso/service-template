// UNCOMPILED Linux counterpart of the session-owned native command plan.
#pragma once
#include "native_command_plan.hpp"
namespace template_creator_bridge {
struct LinuxOriginalCommand {
    LinuxOriginalCommand() = default;
    LinuxOriginalCommand(const LinuxOriginalCommand&) = delete;
    LinuxOriginalCommand& operator=(const LinuxOriginalCommand&) = delete;
    LinuxOriginalCommand(LinuxOriginalCommand&&) = delete;
    LinuxOriginalCommand& operator=(LinuxOriginalCommand&&) = delete;
    static constexpr std::size_t ArgumentLimit = 4097, ByteLimit = 1024 * 1024;
    PyObject* snapshotOriginal = nullptr;
    std::array<char, 32768> image{}, cwd{};
    std::array<char, ByteLimit> argumentBytes{}, environmentBytes{};
    std::array<char*, ArgumentLimit + 1> argv{}, environment{};
    std::size_t argumentUsed = 0, environmentUsed = 0, argumentCount = 0, environmentCount = 0;
    bool attempted = false, copied = false, shapeFailure = false, hasOriginalDeadline = false;
    std::uint64_t originalDeadlineMs = 0;
    const char* firstShapeFailure = nullptr;
    bool Fail(const char* operation) noexcept {
        shapeFailure = true; if (!firstShapeFailure) firstShapeFailure = operation;
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_ValueError, operation);
        return false;
    }
    bool UnicodeSize(PyObject* original, std::size_t available, std::size_t& bytes) noexcept {
        if (!PyUnicode_CheckExact(original)) return Fail("exact original UTF-8 string required");
        bytes = 0;
        for (Py_ssize_t i=0; i<PyUnicode_GET_LENGTH(original); ++i) {
            Py_UCS4 scalar = PyUnicode_READ_CHAR(original, i);
            if (!scalar || scalar > 0x10ffff || (scalar >= 0xd800 && scalar <= 0xdfff))
                return Fail("original UTF-8 scalar/NUL shape");
            std::size_t width = scalar < 0x80 ? 1 : scalar < 0x800 ? 2 : scalar < 0x10000 ? 3 : 4;
            if (width > available - bytes) return Fail("original UTF-8 capacity before copy");
            bytes += width;
        }
        return true;
    }
    static void WriteUnicode(PyObject* original, char* out) noexcept {
        for (Py_ssize_t i=0; i<PyUnicode_GET_LENGTH(original); ++i) {
            Py_UCS4 c = PyUnicode_READ_CHAR(original, i);
            if (c < 0x80) *out++ = static_cast<char>(c);
            else if (c < 0x800) { *out++ = static_cast<char>(0xc0 | (c >> 6)); *out++ = static_cast<char>(0x80 | (c & 63)); }
            else if (c < 0x10000) {
                *out++ = static_cast<char>(0xe0 | (c >> 12)); *out++ = static_cast<char>(0x80 | ((c >> 6) & 63)); *out++ = static_cast<char>(0x80 | (c & 63));
            } else {
                *out++ = static_cast<char>(0xf0 | (c >> 18)); *out++ = static_cast<char>(0x80 | ((c >> 12) & 63));
                *out++ = static_cast<char>(0x80 | ((c >> 6) & 63)); *out++ = static_cast<char>(0x80 | (c & 63));
            }
        }
    }
    bool CaptureSnapshot(PyObject* original) noexcept {
        if (attempted) return Fail("original Linux command copy already attempted");
        attempted = true;
        if (!original) return Fail("original Linux snapshot missing");
        snapshotOriginal = original; Py_INCREF(original);
        if (!PyTuple_Check(original) || PyTuple_GET_SIZE(original) != 5) return Fail("captured Linux tuple shape");
        PyObject* command = PyTuple_GET_ITEM(original, 0);
        if (!PyTuple_Check(command) || PyTuple_GET_SIZE(command) != 6) return Fail("Linux command tuple shape");
        PyObject* originalImage = PyTuple_GET_ITEM(command, 0);
        PyObject* originalCwd = PyTuple_GET_ITEM(command, 2);
        std::size_t imageSize = 0, cwdSize = 0;
        if (!UnicodeSize(originalImage, image.size()-1, imageSize) || !imageSize ||
            !UnicodeSize(originalCwd, cwd.size()-1, cwdSize) || !cwdSize) return Fail("Linux path extent");
        WriteUnicode(originalImage, image.data()); WriteUnicode(originalCwd, cwd.data());
        if (image[0] != '/' || cwd[0] != '/') return Fail("literal absolute Linux image/cwd required");
        PyObject* args = PyTuple_GET_ITEM(command, 1);
        PyObject* env = PyTuple_GET_ITEM(command, 3);
        if (!PyTuple_CheckExact(args) || PyTuple_GET_SIZE(args) > 4096 ||
            !PyTuple_CheckExact(env) || PyTuple_GET_SIZE(env) > 4096) return Fail("Linux original vector capacity");
        // Complete simultaneous encoded extents precharged before vector writes.
        argumentUsed = imageSize + 1;
        for (Py_ssize_t i=0; i<PyTuple_GET_SIZE(args); ++i) {
            std::size_t bytes = 0;
            if (argumentUsed == ByteLimit) return Fail("Linux argv aggregate capacity");
            if (!UnicodeSize(PyTuple_GET_ITEM(args,i), ByteLimit-argumentUsed-1, bytes)) return false;
            argumentUsed += bytes + 1;
        }
        environmentUsed = 0;
        for (Py_ssize_t i=0; i<PyTuple_GET_SIZE(env); ++i) {
            PyObject* pair = PyTuple_GET_ITEM(env,i);
            if (!PyTuple_CheckExact(pair) || PyTuple_GET_SIZE(pair) != 2) return Fail("Linux literal environment pair");
            std::size_t key = 0, value = 0;
            if (ByteLimit-environmentUsed < 2 || !UnicodeSize(PyTuple_GET_ITEM(pair,0), ByteLimit-environmentUsed-2, key) || !key ||
                !UnicodeSize(PyTuple_GET_ITEM(pair,1), ByteLimit-environmentUsed-key-2, value)) return Fail("Linux environment extent");
            PyObject* keyOriginal = PyTuple_GET_ITEM(pair,0);
            for (Py_ssize_t j=0; j<PyUnicode_GET_LENGTH(keyOriginal); ++j)
                if (PyUnicode_READ_CHAR(keyOriginal,j) == '=') return Fail("Linux environment key separator");
            environmentUsed += key + value + 2;
        }
        std::size_t offset = 0;
        argv[argumentCount++] = argumentBytes.data(); WriteUnicode(originalImage, argumentBytes.data()); offset = imageSize + 1;
        for (Py_ssize_t i=0; i<PyTuple_GET_SIZE(args); ++i) {
            auto* value = PyTuple_GET_ITEM(args,i); std::size_t size = 0;
            if (!UnicodeSize(value, ByteLimit-offset-1, size)) return false;
            argv[argumentCount++] = argumentBytes.data()+offset; WriteUnicode(value, argumentBytes.data()+offset); offset += size+1;
        }
        offset = 0;
        for (Py_ssize_t i=0; i<PyTuple_GET_SIZE(env); ++i) {
            PyObject* pair = PyTuple_GET_ITEM(env,i); std::size_t key = 0, value = 0;
            if (!UnicodeSize(PyTuple_GET_ITEM(pair,0), ByteLimit-offset-2, key) ||
                !UnicodeSize(PyTuple_GET_ITEM(pair,1), ByteLimit-offset-key-2, value)) return false;
            environment[environmentCount++] = environmentBytes.data()+offset;
            WriteUnicode(PyTuple_GET_ITEM(pair,0), environmentBytes.data()+offset); environmentBytes[offset+key] = '=';
            WriteUnicode(PyTuple_GET_ITEM(pair,1), environmentBytes.data()+offset+key+1); offset += key+value+2;
        }
        PyObject* deadline = PyTuple_GET_ITEM(command,4);
        if (deadline != Py_None) {
            if (!PyLong_CheckExact(deadline)) return Fail("Linux exact original deadline integer");
            originalDeadlineMs = PyLong_AsUnsignedLongLong(deadline);
            if (PyErr_Occurred()) return false;
            if (!originalDeadlineMs) return Fail("Linux original deadline positive");
            hasOriginalDeadline = true;
        }
        copied = true; return true;
    }
    bool SameSnapshot(PyObject* candidate) const noexcept { return copied && !shapeFailure && candidate == snapshotOriginal; }
};
// Both plan stores and the lifecycle owner's separate copied vectors remain
// simultaneously retained and must both enter the measured whole budget.
}