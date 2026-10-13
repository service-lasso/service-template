// UNRUN pure source-control cases, not actual host/native runtime observations.
#include "windows_runtime_host_configuration.hpp"
#include <cstring>
namespace template_creator {
bool OriginalWindowsRuntimeOptionOrderRegression() {
    using Host=OriginalRuntimeHostConfiguration;using Outcome=Host::OrderedOptionSource::Outcome;
    struct IntegerCase {const char* text;bool known;std::int32_t value;};
    const IntegerCase integers[]={{"0",true,0},{"-0",true,0},{"2147483647",true,2147483647},
        {"-2147483648",true,INT32_MIN},{"2147483648",false,0},{"-2147483649",false,0},
        {"1.0",false,0},{"1e0",false,0},{"true",false,0}};
    for(const auto& c:integers){
        OriginalJson json(std::strlen(c.text)+1,2,OriginalJson::Mode::FirstRootCommentsFirstMember);
        if(!json.Capture(reinterpret_cast<const unsigned char*>(c.text),std::strlen(c.text)))return false;
        const auto value=Host::ObserveSignedIntegerSource(json,0);
        if(!value.attempted||value.jsonOriginal!=&json||value.token!=0||value.decimalSigned32!=c.known||
            (c.known&&value.value!=c.value))return false;
    }
    struct Case {const char* text;Outcome outcome;unsigned specified;bool dependent;bool probes;};
    const Case cases[]={
        {"{\"runtimeOptions\":{}}",Outcome::SourceReturnedTrue,0,false,true},
        {"{\"runtimeOptions\":null}",Outcome::NoOp,0,false,false},
        {"{}",Outcome::SourceReturnedFalse,0,false,false},
        {"{\"runtimeOptions\":{\"rollForward\":\"Minor\",\"applyPatches\":false,\"frameworks\":[]}}",Outcome::SourceReturnedFalse,1,false,true},
        {"{\"runtimeOptions\":{\"frameworks\":[]}}",Outcome::SourceReturnedTrue,0,true,true},
        {"{\"runtimeOptions\":{\"frameworks\":[],\"includedFrameworks\":[]}}",Outcome::SourceReturnedFalse,0,true,true},
        {"{\"runtimeOptions\":{\"rollForwardOnNoCandidateFx\":1,\"frameworks\":[]}}",Outcome::SourceReturnedTrue,2,true,true},
        {"{\"runtimeOptions\":{\"rollForwardOnNoCandidateFx\":1.0,\"frameworks\":[]}}",Outcome::IntegerConversionUnresolved,0,false,true},
        {"{\"runtimeOptions\":{\"configProperties\":{\"x\":{}},\"frameworks\":[]}}",Outcome::PropertyWriterUnresolved,0,false,false},
        {"{\"runtimeOptions\":{\"rollForward\":\"minor\",\"frameworks\":[]}}",Outcome::RollForwardComparisonUnresolved,0,false,true}
    };
    for(const auto& c:cases){
        OriginalJson json(std::strlen(c.text)+1,8,OriginalJson::Mode::FirstRootCommentsFirstMember);
        if(!json.Capture(reinterpret_cast<const unsigned char*>(c.text),std::strlen(c.text)))return false;
        unsigned specified=0;bool dependent=false;
        const auto result=Host::ObserveOrderedOptionSource(json,Host::ObserveOptionStage(json),false,specified,dependent);
        if(!result.attempted||result.jsonOriginal!=&json||result.outcome!=c.outcome||specified!=c.specified||
            result.specifiedAfter!=specified||dependent!=c.dependent||result.dependencyFlagAfter!=dependent||
            result.probesReached!=c.probes)return false;
    }
    // Dev mutates the setting before a later mutually exclusive setting fails;
    // main then sees the surviving mask even though dev returned false.
    const char* dev="{\"runtimeOptions\":{\"rollForward\":\"Minor\",\"applyPatches\":false}}";
    const char* main="{\"runtimeOptions\":{\"applyPatches\":true,\"frameworks\":[]}}";
    OriginalJson d(std::strlen(dev)+1,8,OriginalJson::Mode::FirstRootCommentsFirstMember);
    OriginalJson m(std::strlen(main)+1,8,OriginalJson::Mode::FirstRootCommentsFirstMember);
    if(!d.Capture(reinterpret_cast<const unsigned char*>(dev),std::strlen(dev))||
        !m.Capture(reinterpret_cast<const unsigned char*>(main),std::strlen(main)))return false;
    unsigned mask=0;bool dependent=false;
    const auto first=Host::ObserveOrderedOptionSource(d,Host::ObserveOptionStage(d),true,mask,dependent);
    const auto second=Host::ObserveOrderedOptionSource(m,Host::ObserveOptionStage(m),false,mask,dependent);
    return first.outcome==Outcome::SourceReturnedFalse&&first.rollForwardSource==2&&
        first.applyPatchesToken!=SIZE_MAX&&!first.applyPatchesSource&&first.specifiedAfter==1&&
        second.outcome==Outcome::SourceReturnedFalse&&second.specifiedBefore==1&&second.applyPatchesSource&&
        !dependent&&mask==1;
}
}
