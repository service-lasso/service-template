// UNCOMPILED native-owned command copies used by the session/gate. Structural
// decoding is not authority: original ingress and native input associations
// must independently bind this exact retained snapshot before Begin.
#pragma once
#include <Python.h>
#include <array>
#include <cstdint>
#include <cstddef>
namespace template_creator_bridge {
struct WindowsOriginalCommand {
    WindowsOriginalCommand() = default;
    WindowsOriginalCommand(const WindowsOriginalCommand&) = delete;
    WindowsOriginalCommand& operator=(const WindowsOriginalCommand&) = delete;
    WindowsOriginalCommand(WindowsOriginalCommand&&) = delete;
    WindowsOriginalCommand& operator=(WindowsOriginalCommand&&) = delete;
    PyObject* snapshotOriginal = nullptr; // Strong original, never attribute lookup.
    std::array<char16_t, 32768> image{}, cwd{};
    std::array<char16_t, 32767> commandLine{};
    std::array<char16_t, 65536> environment{};
    std::size_t imageUnits = 0, cwdUnits = 0, commandUnits = 0, environmentUnits = 0;
    bool attempted = false, copied = false, shapeFailure = false;
    bool hasOriginalDeadline = false;
    std::uint64_t originalDeadlineMs = 0;
    const char* firstShapeFailure = nullptr;
    bool Fail(const char* operation) noexcept {
        shapeFailure = true;
        if (!firstShapeFailure) firstShapeFailure = operation;
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_ValueError, operation);
        return false;
    }
    template<std::size_t N>
    bool CopyUnicode(PyObject* original, std::array<char16_t, N>& destination,
                     std::size_t& used) noexcept {
        if (!PyUnicode_CheckExact(original)) return Fail("exact original Unicode required");
        Py_ssize_t length = PyUnicode_GET_LENGTH(original);
        // Source-bound scalar traversal precharges exact WCHAR expansion before
        // writing a destination; no intermediate encoder allocation is needed.
        std::size_t required = 0;
        for (Py_ssize_t i=0; i<length; ++i) {
            Py_UCS4 scalar = PyUnicode_READ_CHAR(original, i);
            if (!scalar || scalar > 0x10ffff || (scalar >= 0xd800 && scalar <= 0xdfff))
                return Fail("original Unicode scalar/NUL shape");
            const std::size_t width = scalar >= 0x10000 ? 2 : 1;
            if (required > N - 1 - width) return Fail("original WCHAR capacity before copy");
            required += width;
        }
        used = 0;
        for (Py_ssize_t i=0; i<length; ++i) {
            Py_UCS4 scalar = PyUnicode_READ_CHAR(original, i);
            if (scalar < 0x10000) destination[used++] = static_cast<char16_t>(scalar);
            else {
                scalar -= 0x10000;
                destination[used++] = static_cast<char16_t>(0xd800 + (scalar >> 10));
                destination[used++] = static_cast<char16_t>(0xdc00 + (scalar & 0x3ff));
            }
        }
        destination[used] = 0;
        return true;
    }
    bool CaptureSnapshot(PyObject* original) noexcept {
        if (attempted) return Fail("original command copy already attempted");
        attempted = true;
        if (!original) return Fail("original snapshot missing");
        snapshotOriginal = original; Py_INCREF(original);
        // NamedTuple source objects use immutable tuple storage. We never call
        // arbitrary properties, __str__, iteration, path conversion or encoders.
        if (!PyTuple_Check(original) || PyTuple_GET_SIZE(original) != 5)
            return Fail("original captured command tuple shape");
        PyObject* command = PyTuple_GET_ITEM(original, 0);
        if (!PyTuple_Check(command) || PyTuple_GET_SIZE(command) != 6)
            return Fail("original command tuple shape");
        if (!CopyUnicode(PyTuple_GET_ITEM(command, 0), image, imageUnits) || !imageUnits ||
            !CopyUnicode(PyTuple_GET_ITEM(command, 2), cwd, cwdUnits) || !cwdUnits ||
            !CopyUnicode(PyTuple_GET_ITEM(original, 1), commandLine, commandUnits) || !commandUnits)
            return Fail("original nonempty Windows command components");
        PyObject* deadline = PyTuple_GET_ITEM(command, 4);
        if (deadline != Py_None) {
            if (!PyLong_CheckExact(deadline)) return Fail("exact original deadline integer required");
            auto returned = PyLong_AsUnsignedLongLong(deadline);
            if (PyErr_Occurred()) return false;
            if (!returned) return Fail("original deadline must be positive");
            originalDeadlineMs = returned; hasOriginalDeadline = true;
        }
        PyObject* encoded = PyTuple_GET_ITEM(original, 2);
        if (!PyBytes_CheckExact(encoded)) return Fail("original Unicode environment bytes required");
        Py_ssize_t length = PyBytes_GET_SIZE(encoded);
        if (length < 4 || length % 2 || static_cast<std::size_t>(length / 2) > environment.size())
            return Fail("original Unicode environment capacity");
        const auto* bytes = reinterpret_cast<const unsigned char*>(PyBytes_AS_STRING(encoded));
        environmentUnits = static_cast<std::size_t>(length / 2);
        for (std::size_t i=0; i<environmentUnits; ++i)
            environment[i] = static_cast<char16_t>(bytes[2*i] | (static_cast<unsigned>(bytes[2*i+1]) << 8));
        if (environment[environmentUnits-1] || environment[environmentUnits-2])
            return Fail("original Unicode environment double NUL");
        for (std::size_t i=0; i+1<environmentUnits; ++i)
            if (!environment[i] && !environment[i+1] && i != environmentUnits-2)
                return Fail("original Unicode environment early terminator");
        copied = true;
        return true;
    }
    bool SameSnapshot(PyObject* candidate) const noexcept {
        return copied && !shapeFailure && candidate == snapshotOriginal;
    }
};
// Private source copies and original Python refs count toward simultaneous
// retained memory. No automatic decref/disposal while native custody depends on
// these inputs; explicit complete session closure/release remains to compose.
}