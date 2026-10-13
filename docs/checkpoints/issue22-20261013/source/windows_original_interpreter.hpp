// UNCOMPILED native standalone interpreter owner. This source configures actual
// CPython initialization; it does not authenticate the selected Python runtime,
// stdlib/extension/source paths or grant execution admission. No argv/ENV import.
#pragma once
#include <Python.h>
#include <windows.h>
#include <array>
#include <vector>
#include <string>
#include <memory>
#include <exception>
#include <stdexcept>
#include <initializer_list>
extern "C" PyObject* PyInit_template_creator_native();
namespace template_creator_bridge {
struct OriginalPythonPaths {
    std::wstring home,program,executable,baseExecutable,prefix,execPrefix,basePrefix,baseExecPrefix;
    std::vector<std::wstring> modulePaths;
};
struct OriginalPythonConfigurationCall {
    const wchar_t* source=nullptr;
    PyStatus returned{};
    bool attempted=false;
};
struct OriginalWindowsInterpreter {
    const OriginalPythonPaths paths;
    PyConfig configuration{};
    std::unique_ptr<OriginalPythonConfigurationCall[]> calls;
    size_t capacity=0,count=0;
    DWORD originalNativeThread=0,initializationNativeThread=0,initializedNativeThread=0;
    PyThreadState* originalPythonThread=nullptr;
    PyInterpreterState* originalPythonInterpreter=nullptr;
    int originallyInitialized=0,afterInitialized=0,appendModuleReturned=-1;
    PyStatus initializationReturned{};
    bool attempted=false,configurationInitialized=false,appendModuleAttempted=false;
    bool initializeAttempted=false,initialized=false,failed=false;
    std::exception_ptr nativeOriginal;
private:
    ~OriginalWindowsInterpreter()=default; // Retain config/status strings and interpreter owner.
    static OriginalPythonPaths RetainPaths(const OriginalPythonPaths& originals,size_t selectedPathUnits) {
        size_t total=0;
        const auto charge=[&](const std::wstring& text) {
            if (text.empty() || text.find(L'\0')!=std::wstring::npos || text.size()==SIZE_MAX ||
                text.size()+1>SIZE_MAX-total) throw std::invalid_argument("exact original Python path string");
            total+=text.size()+1;
        };
        charge(originals.home); charge(originals.program); charge(originals.executable); charge(originals.baseExecutable);
        charge(originals.prefix); charge(originals.execPrefix); charge(originals.basePrefix); charge(originals.baseExecPrefix);
        for (const auto& path:originals.modulePaths) charge(path);
        if (!selectedPathUnits || total>selectedPathUnits || total>SIZE_MAX/sizeof(wchar_t))
            throw std::invalid_argument("precharged original Python path copies");
        if (originals.modulePaths.empty() || originals.modulePaths.size()>SIZE_MAX-8 ||
            originals.modulePaths.size()>SIZE_MAX/sizeof(std::wstring))
            throw std::invalid_argument("complete original interpreter path roster");
        const size_t rows=originals.modulePaths.size()+8;
        if (rows>SIZE_MAX/sizeof(OriginalPythonConfigurationCall))
            throw std::invalid_argument("original interpreter configuration row capacity");
        size_t requestedSourceBytes=sizeof(OriginalWindowsInterpreter);
        for (const size_t bytes : {total*sizeof(wchar_t),
                originals.modulePaths.size()*sizeof(std::wstring),
                rows*sizeof(OriginalPythonConfigurationCall)}) {
            if (bytes>SIZE_MAX-requestedSourceBytes)
                throw std::invalid_argument("original interpreter complete source graph");
            requestedSourceBytes+=bytes;
        }
        // Requested source storage only; string capacity, allocator and CPython
        // configuration/runtime backing remain independent full-fit inputs.
        return originals; // Copy only after complete length/NUL/aggregate preflight.
    }
    bool Set(wchar_t** destination,const std::wstring& original) noexcept {
        if (count==capacity || original.empty()) { failed=true; return false; }
        auto& row=calls[count++]; row.source=original.c_str(); row.attempted=true;
        row.returned=PyConfig_SetString(&configuration,destination,row.source);
        if (PyStatus_Exception(row.returned)) { failed=true; return false; }
        return true;
    }
public:
    explicit OriginalWindowsInterpreter(const OriginalPythonPaths& actualSelection,size_t selectedPathUnits)
        :paths(RetainPaths(actualSelection,selectedPathUnits)) {
        if (paths.modulePaths.empty() || paths.modulePaths.size()>SIZE_MAX-8)
            throw std::invalid_argument("complete original interpreter path roster");
        capacity=paths.modulePaths.size()+8;
        if (capacity>SIZE_MAX/sizeof(OriginalPythonConfigurationCall))
            throw std::invalid_argument("original interpreter configuration row capacity");
        calls=std::make_unique<OriginalPythonConfigurationCall[]>(capacity);
        // Source copies and row capacity precede the first native observation.
        originalNativeThread=GetCurrentThreadId();
    }
    OriginalWindowsInterpreter(const OriginalWindowsInterpreter&)=delete;
    OriginalWindowsInterpreter& operator=(const OriginalWindowsInterpreter&)=delete;
    bool InitializeOriginal() noexcept {
        if (attempted) return false;
        attempted=true;
        initializationNativeThread=GetCurrentThreadId();
        if (initializationNativeThread!=originalNativeThread) { failed=true; return false; }
        originallyInitialized=Py_IsInitialized();
        if (originallyInitialized) { failed=true; return false; }
        try {
            PyConfig_InitIsolatedConfig(&configuration); configurationInitialized=true;
            configuration.use_environment=0; configuration.parse_argv=0;
            configuration.install_signal_handlers=0; configuration.site_import=0;
            configuration.user_site_directory=0; configuration.write_bytecode=0;
            // Explicit all path outputs prevent ambient default path discovery.
            // Actual runtime version/config/loader inputs still need admission.
            if (!Set(&configuration.home,paths.home) || !Set(&configuration.program_name,paths.program) ||
                !Set(&configuration.executable,paths.executable) || !Set(&configuration.base_executable,paths.baseExecutable) ||
                !Set(&configuration.prefix,paths.prefix) || !Set(&configuration.exec_prefix,paths.execPrefix) ||
                !Set(&configuration.base_prefix,paths.basePrefix) || !Set(&configuration.base_exec_prefix,paths.baseExecPrefix)) return false;
            configuration.module_search_paths_set=1;
            for (const auto& path:paths.modulePaths) {
                if (path.empty() || count==capacity) { failed=true; return false; }
                auto& row=calls[count++]; row.source=path.c_str(); row.attempted=true;
                row.returned=PyWideStringList_Append(&configuration.module_search_paths,row.source);
                if (PyStatus_Exception(row.returned)) { failed=true; return false; }
            }
            appendModuleAttempted=true;
            appendModuleReturned=PyImport_AppendInittab("template_creator_native",PyInit_template_creator_native);
            if (appendModuleReturned!=0) { failed=true; return false; }
            initializeAttempted=true;
            initializationReturned=Py_InitializeFromConfig(&configuration);
            afterInitialized=Py_IsInitialized();
            initializedNativeThread=GetCurrentThreadId();
            initialized=!PyStatus_Exception(initializationReturned) && afterInitialized &&
                initializedNativeThread==originalNativeThread;
            if (!initialized) { failed=true; return false; }
            originalPythonThread=PyThreadState_Get();
            originalPythonInterpreter=PyThreadState_GetInterpreter(originalPythonThread);
            initialized=originalPythonThread && originalPythonInterpreter;
            failed=!initialized;
            return initialized;
        } catch (...) { nativeOriginal=std::current_exception(); failed=true; return false; }
    }
    // The native main must keep THIS interpreter and its original thread alive
    // across creator/final collectors. There is deliberately no Py_Finalize,
    // Py_Exit, destructor cleanup or implicit failure-to-process-exit path.
    // Config_Clear belongs to later explicit lifetime closure, after all status
    // references/observation consumers have finished; not a finally on failure.
};
}
