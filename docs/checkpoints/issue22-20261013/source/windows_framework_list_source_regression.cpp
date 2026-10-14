// UNRUN pure source regressions. No native/runtime/provider acceptance.
#include "windows_framework_list_source.hpp"
#include <cstring>
namespace template_creator {
bool FrameworkListSourceRegression() {
    using List=OriginalFrameworkListSource;using Host=OriginalRuntimeHostConfiguration;
    auto complete=[](const Host::FrameworkSettingsSource& source) noexcept {
        List::Completion r;
        // Explicit conditional source fixture: later ENV absent and no
        // command overrides. This is not an observed child ENV/runtime fact.
        if(source.outcome==Host::FrameworkSettingsSource::Outcome::EnvironmentAndOverridesPending)
            r.outcome=List::Completion::Outcome::ConditionalSourceReturnedTrue;
        return r;
    };
    struct Case {const char* dev;const char* main;SIZE_T rows,accepted;List::Outcome outcome;bool ignoredDev;};
    const Case cases[]={
        {"{}","{\"runtimeOptions\":{\"frameworks\":[{\"name\":\"A\",\"version\":\"1.2.3\"},{\"name\":\"B\",\"version\":\"2.0.0\"}]}}",2,2,List::Outcome::ConditionalSourceReturnedTrue,false},
        {"{}","{\"runtimeOptions\":{\"framework\":{\"name\":\"A\",\"version\":\"1.2.3\"},\"frameworks\":[{\"name\":\"A\",\"version\":\"2.0.0\"}]}}",2,1,List::Outcome::SourceReturnedFalse,false},
        {"{\"runtimeOptions\":{\"frameworks\":[{\"name\":\"A\",\"version\":\"1.2.3\"},{\"name\":\"A\",\"version\":\"2.0.0\"}]}}","{\"runtimeOptions\":{\"frameworks\":[{\"name\":\"A\",\"version\":\"3.0.0\"}]}}",3,1,List::Outcome::SourceReturnedFalse,true},
        {"{\"runtimeOptions\":{\"includedFrameworks\":[{\"name\":\"A\",\"version\":\"1.2.3\"}]}}","{\"runtimeOptions\":{\"includedFrameworks\":[{\"name\":\"B\",\"version\":\"1.2.3\"}]}}",2,2,List::Outcome::ConditionalSourceReturnedTrue,false},
        {"{}","{\"runtimeOptions\":{\"frameworks\":[],\"includedFrameworks\":[]}}",0,0,List::Outcome::SourceReturnedFalse,false},
        {"{}","{\"runtimeOptions\":{\"frameworks\":[{\"name\":\"A\",\"version\":\"1.2.3-\"}]}}",1,1,List::Outcome::ConditionalSourceReturnedTrue,false}
    };
    Host::OrderedOptionSource seed;seed.attempted=true;seed.rollForwardSource=2;
    seed.hasDefaultApplySource=true;seed.applyPatchesSource=true;
    for(const auto& c:cases){
        const SIZE_T d=std::strlen(c.dev),m=std::strlen(c.main);
        OriginalJson dev(d+1,16,OriginalJson::Mode::FirstRootCommentsFirstMember);
        OriginalJson main(m+1,16,OriginalJson::Mode::FirstRootCommentsFirstMember);
        if(!dev.Capture(reinterpret_cast<const unsigned char*>(c.dev),d)||
            !main.Capture(reinterpret_cast<const unsigned char*>(c.main),m))return false;
        List list(d+m+2);
        list.ObserveSourceOriginals(&dev,true,true,Host::ObserveOptionStage(dev),main,Host::ObserveOptionStage(main),seed,complete);
        if(list.SourceOutcome()!=c.outcome||list.RowCount()!=c.rows||
            list.PrivateStages()[0].ignoredDevFalse!=c.ignoredDev)return false;
        SIZE_T accepted=0;for(SIZE_T i=0;i<list.RowCount();++i)if(list.PrivateRows()[i].accepted)++accepted;
        if(accepted!=c.accepted)return false;
    }
    const char* text="{\"runtimeOptions\":{\"frameworks\":[{\"name\":\"A\",\"version\":\"1.2.3\"},{\"name\":\"B\",\"version\":\"1.2.3\"}]}}";
    const SIZE_T extent=std::strlen(text);OriginalJson main(extent+1,16,OriginalJson::Mode::FirstRootCommentsFirstMember);
    OriginalJson dev(3,4,OriginalJson::Mode::FirstRootCommentsFirstMember);
    if(!main.Capture(reinterpret_cast<const unsigned char*>(text),extent)||
        !dev.Capture(reinterpret_cast<const unsigned char*>("{}"),2))return false;
    List unresolved(extent+4);
    unresolved.ObserveSourceOriginals(&dev,true,true,Host::ObserveOptionStage(dev),main,Host::ObserveOptionStage(main),seed,
        [](const Host::FrameworkSettingsSource&) noexcept {return List::Completion{};});
    if(unresolved.SourceOutcome()!=List::Outcome::SourceUnresolved||unresolved.RowCount()!=1||
        unresolved.PrivateRows()[0].accepted)return false;
    const char* partial="{\"runtimeOptions\":{\"frameworks\":[{\"name\":\"A\",\"version\":\"1.2.3\"},{\"name\":\"A\",\"version\":\"2.0.0\",\"rollForward\":\"Major\"}]}}";
    const char* after="{\"runtimeOptions\":{\"applyPatches\":false}}";
    const SIZE_T p=std::strlen(partial),a=std::strlen(after);
    OriginalJson partialDev(p+1,16,OriginalJson::Mode::FirstRootCommentsFirstMember);
    OriginalJson afterMain(a+1,16,OriginalJson::Mode::FirstRootCommentsFirstMember);
    if(!partialDev.Capture(reinterpret_cast<const unsigned char*>(partial),p)||
        !afterMain.Capture(reinterpret_cast<const unsigned char*>(after),a))return false;
    List mutations(p+a+2);
    mutations.ObserveSourceOriginals(&partialDev,true,true,Host::ObserveOptionStage(partialDev),
        afterMain,Host::ObserveOptionStage(afterMain),seed,complete);
    const auto* rows=mutations.PrivateRows();const auto& stages=mutations.PrivateStages();
    return mutations.SourceOutcome()==List::Outcome::SourceReturnedFalse&&mutations.RowCount()==2&&
        rows[0].accepted&&!rows[1].accepted&&rows[1].duplicateOf==0&&
        rows[1].settings.effectiveRollForward==4&&rows[1].settings.specifiedAfter==1&&
        mutations.PrivateSpecifiedMask()==1&&mutations.PrivateDependentSource()&&
        stages[0].ignoredDevFalse&&stages[1].prefix.applyPatchesSource==false&&
        stages[1].prefix.hasDefaultApplySource&&stages[1].prefix.outcome==Host::OrderedOptionSource::Outcome::SourceReturnedFalse;
}
}
