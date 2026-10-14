// UNCOMPILED private native selection -> unchanged Python producer arguments.
#pragma once
#ifndef _WIN32
#include "native_session_core.hpp"
#include <array>
#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <variant>
#include "upstream-cli/bq_provider_expectations.h"
namespace template_creator_bridge {
struct ImmutableFileRequirement {
    std::string role,path;
    std::array<unsigned char,32> sha256{};
    std::uint64_t size=0;
};
struct NativeArchiveFoundationPrerequisite {
    std::uint64_t native_extent_bytes=0;
    slcli_bq_expected_occurrence_selector archive_foundation{},archive_readback{};
};
struct NativeArchiveReadbackPrerequisite {
    std::uint64_t native_extent_bytes=0;
    slcli_bq_expected_occurrence_selector archive_foundation{},archive_readback{};
};
using OriginalLinuxRequirementSelection=std::variant<ImmutableFileRequirement,
    NativeArchiveFoundationPrerequisite,NativeArchiveReadbackPrerequisite>;
inline const ImmutableFileRequirement* FileRequirement(const OriginalLinuxRequirementSelection& row) noexcept {
    return std::get_if<ImmutableFileRequirement>(&row);
}
inline std::uint64_t RequirementNativeExtent(const OriginalLinuxRequirementSelection& row) noexcept {
    if(const auto* file=FileRequirement(row))return file->size;
    if(const auto* writer=std::get_if<NativeArchiveFoundationPrerequisite>(&row))return writer->native_extent_bytes;
    if(const auto* reader=std::get_if<NativeArchiveReadbackPrerequisite>(&row))return reader->native_extent_bytes;
    return 0;
}
struct OriginalLinuxRouteSelection {
    OriginalPhase phase{};
    std::string node,root;
    std::vector<std::pair<std::string,std::string>> environment;
    std::array<std::vector<OriginalLinuxRequirementSelection>,2> requirements;
    std::array<std::uint64_t,2> observations{},pollNanoseconds{};
};
struct LinuxRequirementConversion {
    PyObject *role=nullptr,*path=nullptr,*digest=nullptr,*size=nullptr,*result=nullptr;
    PyObject *foundation=nullptr,*readback=nullptr;
    std::array<PyObject*,3> foundationFields{},readbackFields{};
};
struct LinuxEnvironmentConversion {PyObject *name=nullptr,*value=nullptr,*pair=nullptr;};
struct LinuxObservationConversion {PyObject *count=nullptr,*interval=nullptr,*result=nullptr;};
class OriginalLinuxRouteArguments {
    const OriginalLinuxRouteSelection selection_;
    std::array<std::unique_ptr<LinuxRequirementConversion[]>,2> requirementRows_;
    std::unique_ptr<LinuxEnvironmentConversion[]> environmentRows_;
    std::array<LinuxObservationConversion,2> observationRows_{};
    std::array<PyObject*,2> requirements_{};
    PyObject *commandModule_=nullptr,*requirementClass_=nullptr,*routeModule_=nullptr,*observationClass_=nullptr;
    PyObject *foundationClass_=nullptr,*readbackClass_=nullptr;
    PyObject *node_=nullptr,*root_=nullptr,*environment_=nullptr,*arguments_=nullptr;
    PyObject *failureType_=nullptr,*failureValue_=nullptr,*failureTraceback_=nullptr;
    bool attempted_=false,returned_=false;
    ~OriginalLinuxRouteArguments()=default; // Resident original references.
    static OriginalLinuxRouteSelection Retain(const OriginalLinuxRouteSelection& source,std::size_t budget){
        if(source.phase!=OriginalPhase::Generation&&source.phase!=OriginalPhase::FinalQualification)
            throw std::invalid_argument("original Linux protected route phase");
        std::size_t bytes=0;
        const auto charge=[&](const std::string& value){
            if(value.find('\0')!=std::string::npos||value.size()>static_cast<std::size_t>(PY_SSIZE_T_MAX)||
               value.size()==SIZE_MAX||value.size()+1>SIZE_MAX-bytes)
                throw std::invalid_argument("original Linux text geometry");
            bytes+=value.size()+1;
        };
        charge(source.node);charge(source.root);
        if(source.node.empty()||source.root.empty()||source.environment.size()>static_cast<std::size_t>(PY_SSIZE_T_MAX))
            throw std::invalid_argument("original Linux node/root/environment");
        for(const auto& pair:source.environment){
            charge(pair.first);charge(pair.second);
            if(pair.first.empty()||pair.first.find('=')!=std::string::npos)
                throw std::invalid_argument("original Linux environment name");
        }
        const std::size_t count=source.phase==OriginalPhase::Generation?1:2;
        for(std::size_t i=0;i<count;++i){
            if(source.requirements[i].empty()||source.requirements[i].size()>static_cast<std::size_t>(PY_SSIZE_T_MAX)||
               source.requirements[i].size()>SIZE_MAX/sizeof(LinuxRequirementConversion)||
               !source.observations[i]||source.observations[i]>65536||!source.pollNanoseconds[i])
                throw std::invalid_argument("original Linux requirements/schedule geometry");
            for(const auto& row:source.requirements[i]){
                if(const auto* file=FileRequirement(row)){
                    charge(file->role);charge(file->path);
                    if(file->role.empty()||file->path.empty())throw std::invalid_argument("original Linux requirement text");
                }else if(!RequirementNativeExtent(row))throw std::invalid_argument("original native archive extent");
            }
        }
        if(count==1&&!source.requirements[1].empty())throw std::invalid_argument("unexpected generation second roster");
        if(!budget||bytes>budget||source.environment.size()>SIZE_MAX/sizeof(LinuxEnvironmentConversion))
            throw std::invalid_argument("original Linux route copy reservation");
        return source; // Full preflight precedes copy; budget is not fit proof.
    }
    bool Present(PyObject* original) noexcept {
        if(!original&&!PyErr_Occurred())PyErr_SetString(PyExc_SystemError,"Linux argument conversion no result");
        if(PyErr_Occurred()){
            if(!failureType_&&!failureValue_&&!failureTraceback_){
                PyErr_Fetch(&failureType_,&failureValue_,&failureTraceback_);
                Py_XINCREF(failureType_);Py_XINCREF(failureValue_);Py_XINCREF(failureTraceback_);
                PyErr_Restore(failureType_,failureValue_,failureTraceback_);
            }
            return false;
        }
        return original!=nullptr;
    }
    static PyObject* Text(const std::string& value) noexcept {
        return PyUnicode_DecodeUTF8(value.data(),static_cast<Py_ssize_t>(value.size()),"strict");
    }
public:
    OriginalLinuxRouteArguments(const OriginalLinuxRouteSelection& source,std::size_t stringBytes)
        :selection_(Retain(source,stringBytes)){
        const std::size_t count=selection_.phase==OriginalPhase::Generation?1:2;
        for(std::size_t i=0;i<count;++i)requirementRows_[i]=std::make_unique<LinuxRequirementConversion[]>(selection_.requirements[i].size());
        environmentRows_=std::make_unique<LinuxEnvironmentConversion[]>(selection_.environment.size());
    }
    OriginalLinuxRouteArguments(const OriginalLinuxRouteArguments&)=delete;
    OriginalLinuxRouteArguments& operator=(const OriginalLinuxRouteArguments&)=delete;
    bool BuildOriginalArguments() noexcept {
        if(attempted_)return false;attempted_=true;
        if(PyErr_Occurred()){Present(nullptr);return false;}
        commandModule_=PyImport_ImportModule("command_inputs");if(!Present(commandModule_))return false;
        requirementClass_=PyObject_GetAttrString(commandModule_,"Requirement");if(!Present(requirementClass_))return false;
        foundationClass_=PyObject_GetAttrString(commandModule_,"NativeArchiveFoundationPrerequisite");if(!Present(foundationClass_))return false;
        readbackClass_=PyObject_GetAttrString(commandModule_,"NativeArchiveReadbackPrerequisite");if(!Present(readbackClass_))return false;
        routeModule_=PyImport_ImportModule("selected_route");if(!Present(routeModule_))return false;
        observationClass_=PyObject_GetAttrString(routeModule_,"ObservationSelection");if(!Present(observationClass_))return false;
        node_=Text(selection_.node);if(!Present(node_))return false;
        root_=Text(selection_.root);if(!Present(root_))return false;
        environment_=PyTuple_New(static_cast<Py_ssize_t>(selection_.environment.size()));if(!Present(environment_))return false;
        for(std::size_t i=0;i<selection_.environment.size();++i){
            auto& row=environmentRows_[i];const auto& original=selection_.environment[i];
            row.name=Text(original.first);if(!Present(row.name))return false;
            row.value=Text(original.second);if(!Present(row.value))return false;
            row.pair=PyTuple_Pack(2,row.name,row.value);if(!Present(row.pair))return false;
            PyTuple_SET_ITEM(environment_,static_cast<Py_ssize_t>(i),Py_NewRef(row.pair));
        }
        const std::size_t count=selection_.phase==OriginalPhase::Generation?1:2;
        constexpr char hex[]="0123456789abcdef";
        for(std::size_t i=0;i<count;++i){
            requirements_[i]=PyTuple_New(static_cast<Py_ssize_t>(selection_.requirements[i].size()));if(!Present(requirements_[i]))return false;
            for(std::size_t j=0;j<selection_.requirements[i].size();++j){
                auto& row=requirementRows_[i][j];const auto& original=selection_.requirements[i][j];
                if(const auto* file=FileRequirement(original)){
                row.role=Text(file->role);if(!Present(row.role))return false;
                row.path=Text(file->path);if(!Present(row.path))return false;
                std::array<char,64> digest{};
                for(std::size_t n=0;n<32;++n){digest[2*n]=hex[file->sha256[n]>>4];digest[2*n+1]=hex[file->sha256[n]&15];}
                row.digest=PyUnicode_DecodeASCII(digest.data(),digest.size(),"strict");if(!Present(row.digest))return false;
                row.size=PyLong_FromUnsignedLongLong(file->size);if(!Present(row.size))return false;
                row.result=PyObject_CallFunctionObjArgs(requirementClass_,row.role,row.path,row.digest,row.size,nullptr);if(!Present(row.result))return false;
                }else{
                    const auto* writer=std::get_if<NativeArchiveFoundationPrerequisite>(&original);
                    const auto* reader=std::get_if<NativeArchiveReadbackPrerequisite>(&original);
                    if(!writer&&!reader)return false;
                    const auto& w=writer?writer->archive_foundation:reader->archive_foundation;
                    const auto& r=writer?writer->archive_readback:reader->archive_readback;
                    const uint32_t wf[3]{w.phase,w.command,w.requirement},rf[3]{r.phase,r.command,r.requirement};
                    for(size_t n=0;n<3;++n){
                        row.foundationFields[n]=PyLong_FromUnsignedLong(wf[n]);if(!Present(row.foundationFields[n]))return false;
                        row.readbackFields[n]=PyLong_FromUnsignedLong(rf[n]);if(!Present(row.readbackFields[n]))return false;
                    }
                    row.foundation=PyTuple_Pack(3,row.foundationFields[0],row.foundationFields[1],row.foundationFields[2]);if(!Present(row.foundation))return false;
                    row.readback=PyTuple_Pack(3,row.readbackFields[0],row.readbackFields[1],row.readbackFields[2]);if(!Present(row.readback))return false;
                    row.size=PyLong_FromUnsignedLongLong(RequirementNativeExtent(original));if(!Present(row.size))return false;
                    row.result=PyObject_CallFunctionObjArgs(writer?foundationClass_:readbackClass_,row.size,row.foundation,row.readback,nullptr);if(!Present(row.result))return false;
                }
                PyTuple_SET_ITEM(requirements_[i],static_cast<Py_ssize_t>(j),Py_NewRef(row.result));
            }
            auto& row=observationRows_[i];
            row.count=PyLong_FromUnsignedLongLong(selection_.observations[i]);if(!Present(row.count))return false;
            row.interval=PyLong_FromUnsignedLongLong(selection_.pollNanoseconds[i]);if(!Present(row.interval))return false;
            row.result=PyObject_CallFunctionObjArgs(observationClass_,row.count,row.interval,nullptr);if(!Present(row.result))return false;
        }
        arguments_=count==1?PyTuple_Pack(5,node_,root_,environment_,requirements_[0],observationRows_[0].result):
            PyTuple_Pack(7,node_,root_,environment_,requirements_[0],requirements_[1],observationRows_[0].result,observationRows_[1].result);
        returned_=Present(arguments_);return returned_;
    }
    PyObject* OriginalArguments() const noexcept{return returned_?arguments_:nullptr;}
    OriginalPhase OriginalSelectedPhase() const noexcept{return selection_.phase;}
    bool ReservedSourceBytes(std::size_t& bytes) const noexcept {
        bytes=sizeof(*this);
        const auto add=[&](std::size_t n,std::size_t width=1) noexcept {
            if(width&&n>(SIZE_MAX-bytes)/width)return false;bytes+=n*width;return true;
        };
        const auto text=[&](const std::string& value) noexcept {
            return value.capacity()!=SIZE_MAX&&add(value.capacity()+1);
        };
        if(!text(selection_.node)||!text(selection_.root)||
           !add(selection_.environment.capacity(),sizeof(std::pair<std::string,std::string>))||
           !add(selection_.environment.size(),sizeof(LinuxEnvironmentConversion)))return false;
        for(const auto& row:selection_.environment)if(!text(row.first)||!text(row.second))return false;
        for(std::size_t i=0;i<2;++i){
            if(!add(selection_.requirements[i].capacity(),sizeof(OriginalLinuxRequirementSelection))||
               !add(selection_.requirements[i].size(),sizeof(LinuxRequirementConversion)))return false;
            for(const auto& row:selection_.requirements[i])if(const auto* file=FileRequirement(row))
                if(!text(file->role)||!text(file->path))return false;
        }
        return true; // Conservative SSO double-count; allocator/Python/overlap remain additional.
    }
};
}
#endif
