// UNCOMPILED resident Linux CPython owner. Reference API: docs.python.org/3.14/
// c-api/init_config.html. Exact admitted runtime/headers, not current docs alone,
// govern eventual execution. No source/path tuple authenticates runtime inputs.
#pragma once
#ifndef _WIN32
#include <Python.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <cerrno>
#include <array>
#include <vector>
#include <string>
#include <memory>
#include <exception>
#include <stdexcept>
#include <cstdint>
extern "C" PyObject* PyInit_template_creator_native();
namespace template_creator_bridge {
struct OriginalLinuxPythonPaths {
    std::wstring home,program,executable,baseExecutable,prefix,execPrefix,basePrefix,baseExecPrefix;
    std::vector<std::wstring> modulePaths;
};
struct OriginalLinuxPythonConfigurationCall {
    const wchar_t* source=nullptr;PyStatus returned{};
    bool attempted=false,returnedObserved=false;
};
struct OriginalLinuxInterpreter {
    const OriginalLinuxPythonPaths paths;
    PyConfig configuration{};
    std::unique_ptr<OriginalLinuxPythonConfigurationCall[]> calls;
    std::size_t capacity=0,count=0;
    struct ThreadOriginal {long result=-1;int error=0;bool attempted=false,returned=false;};
    std::array<ThreadOriginal,3> threads{};
    PyThreadState* originalPythonThread=nullptr;
    PyInterpreterState* originalPythonInterpreter=nullptr;
    int originallyInitialized=0,afterInitialized=0,appendModuleReturned=-1,gilCheckReturned=0;
    PyStatus initializationReturned{};
    bool attempted=false,configurationInitialized=false,appendModuleAttempted=false,appendModuleReturnedObserved=false;
    bool initializeAttempted=false,initializeReturnedObserved=false,initialized=false,failed=false;
    std::exception_ptr nativeOriginal;
private:
    ~OriginalLinuxInterpreter()=default; // Resident config/status/runtime originals.
    static OriginalLinuxPythonPaths CopyPaths(const OriginalLinuxPythonPaths& original,std::size_t selectedUnits){
        std::size_t units=0;
        const auto charge=[&](const std::wstring& value){
            if(value.empty()||value.find(L'\0')!=std::wstring::npos||value.size()==SIZE_MAX||
               value.size()+1>SIZE_MAX-units)throw std::invalid_argument("original Linux Python path");
            units+=value.size()+1;
        };
        charge(original.home);charge(original.program);charge(original.executable);charge(original.baseExecutable);
        charge(original.prefix);charge(original.execPrefix);charge(original.basePrefix);charge(original.baseExecPrefix);
        for(const auto& value:original.modulePaths)charge(value);
        if(original.modulePaths.empty()||!selectedUnits||units>selectedUnits||units>SIZE_MAX/sizeof(wchar_t))
            throw std::invalid_argument("original Linux Python path precharge");
        if(original.modulePaths.size()>SIZE_MAX-8 ||
           original.modulePaths.size()>SIZE_MAX/sizeof(std::wstring) ||
           original.modulePaths.size()+8>SIZE_MAX/sizeof(OriginalLinuxPythonConfigurationCall))
            throw std::overflow_error("Linux Python source graph geometry");
        size_t total=sizeof(OriginalLinuxInterpreter);
        const auto add=[&](size_t n) {
            if(n>SIZE_MAX-total)return false;total+=n;return true;
        };
        if(!add(units*sizeof(wchar_t))||!add(original.modulePaths.size()*sizeof(std::wstring))||
           !add((original.modulePaths.size()+8)*sizeof(OriginalLinuxPythonConfigurationCall)))
            throw std::overflow_error("aggregate Linux Python requested source backing");
        // Requested storage only: string capacity, allocator and Python/native
        // runtime allocation remain independent actual whole-fit obligations.
        return original;
    }
    static void ObserveThread(ThreadOriginal& row) noexcept {
        row.attempted=true;row.result=syscall(SYS_gettid);if(row.result<0)row.error=errno;row.returned=true;
    }
    bool Set(wchar_t** destination,const std::wstring& original) noexcept {
        if(count==capacity){failed=true;return false;}
        auto& row=calls[count++];row.source=original.c_str();row.attempted=true;
        row.returned=PyConfig_SetString(&configuration,destination,row.source);row.returnedObserved=true;
        if(PyStatus_Exception(row.returned)){failed=true;return false;}return true;
    }
public:
    OriginalLinuxInterpreter(const OriginalLinuxPythonPaths& original,std::size_t selectedUnits)
        :paths(CopyPaths(original,selectedUnits)){
        if(paths.modulePaths.size()>SIZE_MAX-8)throw std::overflow_error("Linux Python path row count");
        capacity=paths.modulePaths.size()+8;
        if(capacity>SIZE_MAX/sizeof(OriginalLinuxPythonConfigurationCall))throw std::overflow_error("Linux Python path rows");
        calls=std::make_unique<OriginalLinuxPythonConfigurationCall[]>(capacity);
        ObserveThread(threads[0]);if(threads[0].result<=0)failed=true;
    }
    OriginalLinuxInterpreter(const OriginalLinuxInterpreter&)=delete;
    OriginalLinuxInterpreter& operator=(const OriginalLinuxInterpreter&)=delete;
    bool InitializeOriginal() noexcept {
        if(attempted||failed)return false;attempted=true;ObserveThread(threads[1]);
        if(threads[1].result<=0||threads[1].result!=threads[0].result){failed=true;return false;}
        originallyInitialized=Py_IsInitialized();if(originallyInitialized){failed=true;return false;}
        try{
            PyConfig_InitIsolatedConfig(&configuration);configurationInitialized=true;
            configuration.use_environment=0;configuration.parse_argv=0;configuration.install_signal_handlers=0;
            configuration.site_import=0;configuration.user_site_directory=0;configuration.write_bytecode=0;
            if(!Set(&configuration.home,paths.home)||!Set(&configuration.program_name,paths.program)||
               !Set(&configuration.executable,paths.executable)||!Set(&configuration.base_executable,paths.baseExecutable)||
               !Set(&configuration.prefix,paths.prefix)||!Set(&configuration.exec_prefix,paths.execPrefix)||
               !Set(&configuration.base_prefix,paths.basePrefix)||!Set(&configuration.base_exec_prefix,paths.baseExecPrefix))return false;
            configuration.module_search_paths_set=1;
            for(const auto& path:paths.modulePaths){
                if(count==capacity){failed=true;return false;}
                auto& row=calls[count++];row.source=path.c_str();row.attempted=true;
                row.returned=PyWideStringList_Append(&configuration.module_search_paths,row.source);row.returnedObserved=true;
                if(PyStatus_Exception(row.returned)){failed=true;return false;}
            }
            appendModuleAttempted=true;appendModuleReturned=PyImport_AppendInittab("template_creator_native",PyInit_template_creator_native);
            appendModuleReturnedObserved=true;if(appendModuleReturned){failed=true;return false;}
            initializeAttempted=true;initializationReturned=Py_InitializeFromConfig(&configuration);initializeReturnedObserved=true;
            afterInitialized=Py_IsInitialized();ObserveThread(threads[2]);
            if(PyStatus_Exception(initializationReturned)||!afterInitialized||threads[2].result!=threads[0].result){failed=true;return false;}
            gilCheckReturned=PyGILState_Check();
            originalPythonThread=PyThreadState_GetUnchecked();
            if(!gilCheckReturned||!originalPythonThread){failed=true;return false;}
            originalPythonInterpreter=PyThreadState_GetInterpreter(originalPythonThread);
            initialized=originalPythonInterpreter!=nullptr;failed=!initialized;return initialized;
        }catch(...){nativeOriginal=std::current_exception();failed=true;return false;}
    }
    // No Py_Finalize/Py_Exit/Config_Clear or destructor cleanup on failed or
    // successful initialization. Actual original main retains this owner through
    // all native borrowers/final receipt and explicit selected settlement.
};
}
#endif
