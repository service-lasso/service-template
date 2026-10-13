// UNCOMPILED native selection -> exact Python source arguments. These copies
// select actual inputs; no list/digest/schedule becomes a native admission.
#pragma once
#include "native_session_core.hpp"
#include "windows_private_fixture_root.hpp"
#include <windows.h>
#include <array>
#include <cstdint>
#include <vector>
#include <string>
#include <stdexcept>
namespace template_creator_bridge {
struct OriginalNativeRequirementSelection {
    std::wstring role,path;
    std::array<unsigned char,32> sha256{};
    uint64_t size=0;
};
struct OriginalNativeRouteSelection {
    OriginalPhase phase{};
    std::wstring node,root;
    std::vector<std::pair<std::wstring,std::wstring>> environment;
    std::array<std::vector<OriginalNativeRequirementSelection>,2> requirements;
    std::array<uint64_t,2> observations{},pollNanoseconds{};
};
struct OriginalRequirementConversion {
    PyObject *role=nullptr,*path=nullptr,*digest=nullptr,*size=nullptr,*result=nullptr;
};
struct OriginalEnvironmentConversion {
    PyObject *name=nullptr,*value=nullptr,*pair=nullptr;
    SIZE_T originalSelectionOrdinal=SIZE_MAX;
    const template_creator::OriginalWindowsFixtureRoot* fixtureRootOriginal=nullptr;
};
struct OriginalObservationConversion { PyObject *count=nullptr,*interval=nullptr,*result=nullptr; };
class OriginalWindowsRouteArguments {
    const OriginalNativeRouteSelection selection_;
    std::array<std::unique_ptr<OriginalRequirementConversion[]>,2> requirementRows_;
    std::unique_ptr<OriginalEnvironmentConversion[]> environmentRows_;
    std::array<PyObject*,2> requirementTuples_{};
    std::array<OriginalObservationConversion,2> observationRows_{};
    PyObject *commandModule_=nullptr,*requirementClass_=nullptr,*routeModule_=nullptr,*observationClass_=nullptr;
    PyObject *node_=nullptr,*root_=nullptr,*environment_=nullptr,*arguments_=nullptr;
    PyObject *failureType_=nullptr,*failureValue_=nullptr,*failureTraceback_=nullptr;
    bool attempted_=false,returned_=false;
    const template_creator::OriginalWindowsFixtureRoot* fixtureRootOriginal_=nullptr;
    bool fixtureBindingAttempted_=false,fixtureBound_=false;
    SIZE_T environmentCount_=0;
    bool fixtureEnvironmentRequired_=true;
    SIZE_T fixtureEnvironmentUnits_=0;
    ~OriginalWindowsRouteArguments()=default; // Resident original Python references.
    static OriginalNativeRouteSelection Retain(const OriginalNativeRouteSelection& original,SIZE_T selectedStringUnits) {
        if(original.phase!=OriginalPhase::Generation && original.phase!=OriginalPhase::FinalQualification)
            throw std::invalid_argument("fixed protected native route phase");
        SIZE_T units=0;
        const auto charge=[&](const std::wstring& value) {
            if(value.find(L'\0')!=std::wstring::npos || value.size()>=32768 || value.size()+1>SIZE_MAX-units)
                throw std::invalid_argument("bounded original native route text");
            units+=value.size()+1;
        };
        charge(original.node);charge(original.root);
        if(original.node.empty()||original.root.empty())throw std::invalid_argument("actual original node/root selection");
        SIZE_T envUnits=1;
        for(const auto& pair:original.environment) {
            charge(pair.first);charge(pair.second);
            if(pair.first.empty() || pair.first.find(L'=')!=std::wstring::npos ||
                pair.first.size()+pair.second.size()+2>65536-envUnits)
                throw std::invalid_argument("precharged native Unicode environment");
            envUnits+=pair.first.size()+pair.second.size()+2;
        }
        const SIZE_T count=original.phase==OriginalPhase::Generation?1:2;
        for(SIZE_T i=0;i<count;++i) {
            if(original.requirements[i].empty()||original.requirements[i].size()>1024 ||
                !original.observations[i]||original.observations[i]>65536 || !original.pollNanoseconds[i])
                throw std::invalid_argument("actual bounded native input/schedule roster");
            for(const auto& requirement:original.requirements[i]) {
                charge(requirement.role);charge(requirement.path);
                if(requirement.role.empty()||requirement.path.empty())throw std::invalid_argument("actual input role/path selection");
            }
        }
        if(count==1 && !original.requirements[1].empty())throw std::invalid_argument("unexpected second generation input roster");
        if(!selectedStringUnits || units>selectedStringUnits)throw std::invalid_argument("native route copy precharge exhausted");
        return original; // Complete traversal/preflight precedes all copies.
    }
    bool Clean() noexcept {
        if(!PyErr_Occurred())return true;
        if(!failureType_&&!failureValue_&&!failureTraceback_) {
            PyErr_Fetch(&failureType_,&failureValue_,&failureTraceback_);
            Py_XINCREF(failureType_);Py_XINCREF(failureValue_);Py_XINCREF(failureTraceback_);
            PyErr_Restore(failureType_,failureValue_,failureTraceback_);
        }
        return false;
    }
    bool Present(PyObject* original) noexcept {
        if(!original&&!PyErr_Occurred())PyErr_SetString(PyExc_SystemError,"native route conversion returned no original");
        return Clean()&&original;
    }
    static PyObject* Text(const std::wstring& original) noexcept {
        return PyUnicode_FromWideChar(original.data(),static_cast<Py_ssize_t>(original.size()));
    }
    static bool TempName(const std::wstring& name) noexcept {
        if(name.size()!=3&&name.size()!=4)return false;
        constexpr wchar_t tmp[]=L"TMP",temp[]=L"TEMP";
        const wchar_t* expected=name.size()==3?tmp:temp;
        for(SIZE_T i=0;i<name.size();++i) {
            const wchar_t c=name[i]>=L'a'&&name[i]<=L'z'?name[i]-(L'a'-L'A'):name[i];
            if(c!=expected[i])return false;
        }
        return true;
    }
public:
    OriginalWindowsRouteArguments(const OriginalNativeRouteSelection& original,SIZE_T selectedStringUnits,
                                  bool deriveFixtureEnvironment=true)
        :selection_(Retain(original,selectedStringUnits)),fixtureEnvironmentRequired_(deriveFixtureEnvironment) {
        const SIZE_T count=selection_.phase==OriginalPhase::Generation?1:2;
        for(SIZE_T i=0;i<count;++i)requirementRows_[i]=std::make_unique<OriginalRequirementConversion[]>(selection_.requirements[i].size());
        if(selection_.environment.size()>SIZE_MAX-2)
            throw std::invalid_argument("fixture environment roster overflow");
        environmentCount_=deriveFixtureEnvironment?2:0;
        for(const auto& pair:selection_.environment)
            if(!deriveFixtureEnvironment||!TempName(pair.first))++environmentCount_;
        environmentRows_=std::make_unique<OriginalEnvironmentConversion[]>(environmentCount_);
    }
    OriginalWindowsRouteArguments(const OriginalWindowsRouteArguments&)=delete;
    OriginalWindowsRouteArguments& operator=(const OriginalWindowsRouteArguments&)=delete;
    bool BindOriginalFixtureRoot(const template_creator::OriginalWindowsFixtureRoot& root) noexcept {
        if(fixtureBindingAttempted_||attempted_||!fixtureEnvironmentRequired_)return false;
        fixtureBindingAttempted_=true;fixtureRootOriginal_=&root;
        fixtureBound_=root.created&&root.environmentPathBound&&root.rootAcquired&&!root.rootUnknown&&
            !root.rootCloseAttempted&&!root.dispositionAttempted;
        if(!fixtureBound_||root.environmentPathUnits>(65536-12)/2)return fixtureBound_=false;
        fixtureEnvironmentUnits_=12+2*root.environmentPathUnits;
        for(const auto& pair:selection_.environment)if(!TempName(pair.first)) {
            const SIZE_T rowUnits=pair.first.size()+pair.second.size()+2;
            if(rowUnits>65536-fixtureEnvironmentUnits_)return fixtureBound_=false;
            fixtureEnvironmentUnits_+=rowUnits;
        }
        return fixtureBound_;
    }
    bool BuildOriginalArguments() noexcept {
        if(attempted_)return false;attempted_=true;
        if(fixtureEnvironmentRequired_&&(!fixtureBound_||!fixtureRootOriginal_||
            fixtureRootOriginal_->rootCloseAttempted||fixtureRootOriginal_->dispositionAttempted)) {
            if(!PyErr_Occurred())PyErr_SetString(PyExc_RuntimeError,"same live original private fixture root required");
            return Clean();
        }
        if(!Clean())return false;
        commandModule_=PyImport_ImportModule("command_inputs");if(!Present(commandModule_))return false;
        requirementClass_=PyObject_GetAttrString(commandModule_,"Requirement");if(!Present(requirementClass_))return false;
        routeModule_=PyImport_ImportModule("selected_route");if(!Present(routeModule_))return false;
        observationClass_=PyObject_GetAttrString(routeModule_,"ObservationSelection");if(!Present(observationClass_))return false;
        node_=Text(selection_.node);if(!Present(node_))return false;
        root_=Text(selection_.root);if(!Present(root_))return false;
        environment_=PyTuple_New(static_cast<Py_ssize_t>(environmentCount_));if(!Present(environment_))return false;
        SIZE_T destination=0;
        for(SIZE_T i=0;i<selection_.environment.size();++i) {
            const auto& original=selection_.environment[i];
            if(fixtureEnvironmentRequired_&&TempName(original.first))continue;
            auto& row=environmentRows_[destination];row.originalSelectionOrdinal=i;
            row.name=Text(original.first);if(!Present(row.name))return false;
            row.value=Text(original.second);if(!Present(row.value))return false;
            row.pair=PyTuple_Pack(2,row.name,row.value);if(!Present(row.pair))return false;
            PyTuple_SET_ITEM(environment_,static_cast<Py_ssize_t>(destination++),Py_NewRef(row.pair));
        }
        if(fixtureEnvironmentRequired_)for(const wchar_t* name:{L"TEMP",L"TMP"}) {
            auto& row=environmentRows_[destination];row.fixtureRootOriginal=fixtureRootOriginal_;
            row.name=PyUnicode_FromWideChar(name,-1);if(!Present(row.name))return false;
            row.value=PyUnicode_FromWideChar(fixtureRootOriginal_->environmentPath.data(),
                static_cast<Py_ssize_t>(fixtureRootOriginal_->environmentPathUnits));
            if(!Present(row.value))return false;
            row.pair=PyTuple_Pack(2,row.name,row.value);if(!Present(row.pair))return false;
            PyTuple_SET_ITEM(environment_,static_cast<Py_ssize_t>(destination++),Py_NewRef(row.pair));
        }
        const SIZE_T count=selection_.phase==OriginalPhase::Generation?1:2;
        constexpr wchar_t hex[]=L"0123456789abcdef";
        for(SIZE_T i=0;i<count;++i) {
            requirementTuples_[i]=PyTuple_New(static_cast<Py_ssize_t>(selection_.requirements[i].size()));
            if(!Present(requirementTuples_[i]))return false;
            for(SIZE_T j=0;j<selection_.requirements[i].size();++j) {
                auto& row=requirementRows_[i][j];const auto& original=selection_.requirements[i][j];
                row.role=Text(original.role);if(!Present(row.role))return false;
                row.path=Text(original.path);if(!Present(row.path))return false;
                std::array<wchar_t,64> digest{};
                for(SIZE_T n=0;n<original.sha256.size();++n) {digest[2*n]=hex[original.sha256[n]>>4];digest[2*n+1]=hex[original.sha256[n]&15];}
                row.digest=PyUnicode_FromWideChar(digest.data(),digest.size());if(!Present(row.digest))return false;
                row.size=PyLong_FromUnsignedLongLong(original.size);if(!Present(row.size))return false;
                row.result=PyObject_CallFunctionObjArgs(requirementClass_,row.role,row.path,row.digest,row.size,nullptr);
                if(!Present(row.result))return false;
                PyTuple_SET_ITEM(requirementTuples_[i],static_cast<Py_ssize_t>(j),Py_NewRef(row.result));
            }
            auto& observation=observationRows_[i];
            observation.count=PyLong_FromUnsignedLongLong(selection_.observations[i]);if(!Present(observation.count))return false;
            observation.interval=PyLong_FromUnsignedLongLong(selection_.pollNanoseconds[i]);if(!Present(observation.interval))return false;
            observation.result=PyObject_CallFunctionObjArgs(observationClass_,observation.count,observation.interval,nullptr);
            if(!Present(observation.result))return false;
        }
        arguments_=count==1?PyTuple_Pack(5,node_,root_,environment_,requirementTuples_[0],observationRows_[0].result):
            PyTuple_Pack(7,node_,root_,environment_,requirementTuples_[0],requirementTuples_[1],observationRows_[0].result,observationRows_[1].result);
        returned_=Present(arguments_);return returned_;
    }
    bool Attempted() const noexcept {return attempted_;}
    bool Returned() const noexcept {return returned_;}
    OriginalPhase OriginalSelectedPhase() const noexcept {return selection_.phase;}
    PyObject* OriginalArguments() const noexcept {return returned_?arguments_:nullptr;}
    PyObject* OriginalNode() const noexcept {return returned_?node_:nullptr;}
    PyObject* OriginalRoot() const noexcept {return returned_?root_:nullptr;}
    PyObject* OriginalEnvironment() const noexcept {return returned_?environment_:nullptr;}
    const template_creator::OriginalWindowsFixtureRoot* OriginalFixtureRoot() const noexcept {
        return fixtureBound_?fixtureRootOriginal_:nullptr;
    }
    PyObject* OriginalRequirements(SIZE_T ordinal) const noexcept {
        return returned_ && ordinal<requirementTuples_.size()?requirementTuples_[ordinal]:nullptr;
    }
    // Every scalar/tuple/class/module/failed return reference remains private in
    // this owner. Python allocation/Unicode/module/exception graphs and native
    // string/vector capacity/overlap require complete measured accounting.
};
}
