// UNRUN source cases; no framework/native runtime/package authority inferred.
#include "windows_runtime_host_configuration.hpp"
#include "windows_framework_resolution_source.hpp"
#include <cstring>
namespace template_creator {
bool OriginalWindowsFrameworkVersionSourceRegression() {
    using Version=OriginalFrameworkVersionSource;using Outcome=Version::Outcome;
    struct Case {const char* json;Outcome expected;bool pre;};
    const Case cases[]={
        {"\"1.2.3\"",Outcome::SourceParsed,false},
        {"\"1.2\"",Outcome::SourceReturnedFalse,false},
        {"\"1.2.3.4\"",Outcome::SourceReturnedFalse,false},
        {"\"01.2.3\"",Outcome::SourceReturnedFalse,false},
        {"\"1.02.3\"",Outcome::SourceReturnedFalse,false},
        {"\"1.2.03\"",Outcome::SourceReturnedFalse,false},
        {"\"1.2.3-alpha.0+001\"",Outcome::SourceParsed,true},
        {"\"1.2.3+001\"",Outcome::SourceParsed,false},
        {"\"1.2.3-01\"",Outcome::SourceReturnedFalse,true},
        {"\"1.2.3-01a\"",Outcome::SourceParsed,true},
        {"\"1.2.3-alpha..b\"",Outcome::SourceReturnedFalse,true},
        {"\"1.2.3+\"",Outcome::SourceReturnedFalse,false},
        {"\"999999999999999\"",Outcome::SourceReturnedFalse,false},
        {"\"999999999999999x.2.3\"",Outcome::SourceReturnedFalse,false},
        {"\"999999999999999.2\"",Outcome::UnsignedConversionWouldThrow,false},
        {"\"1.999999999999999\"",Outcome::SourceReturnedFalse,false},
        {"\"1.2.999999999999999.x\"",Outcome::UnsignedConversionWouldThrow,false},
        {"\"2147483648.2.3\"",Outcome::UnsignedToSignedAbiUnresolved,false},
        {"\"1.2.3\\u0000ignored\"",Outcome::SourceParsed,false}
    };
    for(const auto& c:cases){
        OriginalJson json(std::strlen(c.json)+1,4,OriginalJson::Mode::FirstRootCommentsFirstMember);
        if(!json.Capture(reinterpret_cast<const unsigned char*>(c.json),std::strlen(c.json)))return false;
        const auto result=Version::Observe(json,0);
        if(!result.attempted||result.jsonOriginal!=&json||result.token!=0||result.outcome!=c.expected||
            result.prerelease!=c.pre)return false;
        if(c.expected==Outcome::SourceParsed&&result.convertedComponents!=3)return false;
    }
    using Comparison=OriginalFrameworkVersionComparison;using Compared=Comparison::Outcome;
    struct CompareCase {const char *a,*b;Compared expected;int order;};
    const CompareCase comparisons[]={
        {"\"1.2.3\"","\"2.0.0\"",Compared::SourceCompared,-1},
        {"\"1.3.0\"","\"1.2.99\"",Compared::SourceCompared,1},
        {"\"1.2.4\"","\"1.2.3\"",Compared::SourceCompared,1},
        {"\"1.2.3\"","\"1.2.3-a\"",Compared::SourceCompared,1},
        {"\"1.2.3-9\"","\"1.2.3-10\"",Compared::SourceCompared,-1},
        {"\"1.2.3-1\"","\"1.2.3-a\"",Compared::SourceCompared,-1},
        {"\"1.2.3-a\"","\"1.2.3-a.1\"",Compared::SourceCompared,-1},
        {"\"1.2.3-alpha9\"","\"1.2.3-alpha10\"",Compared::SourceCompared,1},
        {"\"1.2.3-a+old\"","\"1.2.3-a+new\"",Compared::SourceCompared,0},
        {"\"1.2.3-4294967296\"","\"1.2.3-4294967296\"",Compared::SourceCompared,0},
        {"\"1.2.3-4294967296\"","\"1.2.3-z\"",Compared::UnsignedConversionWouldThrow,0},
        {"\"1.2.3-4294967296a\"","\"1.2.3-z\"",Compared::SourceCompared,-1},
        {"\"1.2.3-\"","\"1.2.3\"",Compared::OperandUnresolved,0},
        {"\"1.2.3-a\\u0000ignored\"","\"1.2.3-a\"",Compared::SourceCompared,0}
    };
    for(const auto& c:comparisons){
        OriginalJson a(std::strlen(c.a)+1,4,OriginalJson::Mode::FirstRootCommentsFirstMember);
        OriginalJson b(std::strlen(c.b)+1,4,OriginalJson::Mode::FirstRootCommentsFirstMember);
        if(!a.Capture(reinterpret_cast<const unsigned char*>(c.a),std::strlen(c.a))||
           !b.Capture(reinterpret_cast<const unsigned char*>(c.b),std::strlen(c.b)))return false;
        const auto left=Version::Observe(a,0),right=Version::Observe(b,0);
        const auto result=Comparison::Observe(left,right);
        if(result.left!=&left||result.right!=&right||result.outcome!=c.expected||
           (c.expected==Compared::SourceCompared&&result.order!=c.order))return false;
    }
    // Pure conditional settings/list fixtures, never actual native membership
    // or an authenticated runtime reference. Version bodies are really parsed.
    using Reference=OriginalFrameworkReferenceSource;using Search=OriginalFrameworkCandidateSearch;
    struct SearchCase {
        const char* requested;std::array<const char*,4> candidates;size_t count;
        Reference::Range range;bool apply,highest,prefer;size_t expected;
        Search::Outcome outcome;
    };
    const SearchCase searches[]={
        {"\"1.2.3\"",{"\"1.2.4-preview\"","\"1.2.5\""},2,Reference::Range::Minor,true,false,true,1,Search::Outcome::ConditionalMatch},
        {"\"1.2.3\"",{"\"1.2.4-preview\"","\"1.2.5\""},2,Reference::Range::Minor,true,false,false,0,Search::Outcome::ConditionalMatch},
        {"\"1.2.3\"",{"\"1.2.4\"","\"1.2.8\"","\"1.3.0\""},3,Reference::Range::Minor,true,false,false,1,Search::Outcome::ConditionalMatch},
        {"\"1.2.3\"",{"\"1.2.8\"","\"1.3.0\"","\"1.3.2\""},3,Reference::Range::Minor,true,true,false,2,Search::Outcome::ConditionalMatch},
        {"\"1.2.3\"",{"\"2.0.0\"","\"1.2.8\"","\"1.2.4\""},3,Reference::Range::Major,true,false,false,1,Search::Outcome::ConditionalMatch},
        {"\"1.2.3\"",{"\"1.2.3+a\"","\"1.2.3+b\""},2,Reference::Range::Patch,true,false,false,1,Search::Outcome::ConditionalMatch},
        {"\"1.2.3-a\"",{"\"1.2.3-a+first\"","\"1.2.3-a+second\""},2,Reference::Range::Patch,true,false,false,0,Search::Outcome::ConditionalMatch},
        {"\"1.2.3\"",{"\"1.2.4\""},1,Reference::Range::Patch,false,false,false,SIZE_MAX,Search::Outcome::ConditionalNoMatch},
        {"\"1.2.3\"",{"\"1.2.3-\""},1,Reference::Range::Minor,true,false,false,SIZE_MAX,Search::Outcome::SourceUnresolved}
    };
    for(const auto& c:searches){
        OriginalJson requested(std::strlen(c.requested)+1,4,OriginalJson::Mode::FirstRootCommentsFirstMember);
        if(!requested.Capture(reinterpret_cast<const unsigned char*>(c.requested),std::strlen(c.requested)))return false;
        const auto requestedVersion=Version::Observe(requested,0);
        std::array<std::unique_ptr<OriginalJson>,4> jsons;
        std::array<Version,4> versions;std::array<const Version*,4> originals{};
        for(size_t i=0;i<c.count;++i){
            jsons[i]=std::make_unique<OriginalJson>(std::strlen(c.candidates[i])+1,4,OriginalJson::Mode::FirstRootCommentsFirstMember);
            if(!jsons[i]->Capture(reinterpret_cast<const unsigned char*>(c.candidates[i]),std::strlen(c.candidates[i])))return false;
            versions[i]=Version::Observe(*jsons[i],0);originals[i]=&versions[i];
        }
        Reference reference;reference.version=&requestedVersion;reference.range=c.range;
        reference.applyPatches=c.apply;reference.rollHighest=c.highest;reference.preferRelease=c.prefer;
        reference.outcome=Reference::Outcome::ConditionalSourceReady;
        Search search(reference,originals.data(),c.count);
        if(search.ObserveCompleteSourceList()!=c.outcome||
           search.SelectedConditionalSource()!=(c.expected==SIZE_MAX?nullptr:&versions[c.expected]))return false;
        size_t bytes=0;if(!search.ReservedSourceBytes(bytes)||bytes<sizeof(search))return false;
    }
    using Host=OriginalRuntimeHostConfiguration;using Stage=Host::OrderedOptionSource::Outcome;
    const char* dev="{\"runtimeOptions\":{\"includedFrameworks\":[{\"name\":\"A\",\"version\":\"1.2.3\"},{\"name\":\"A\",\"version\":\"bad\"}]}}";
    const char* main="{\"runtimeOptions\":{\"includedFrameworks\":[{\"name\":\"B\",\"version\":\"bad\"},{\"name\":\"A\",\"version\":\"1.2.3\"}]}}";
    OriginalJson d(std::strlen(dev)+1,8,OriginalJson::Mode::FirstRootCommentsFirstMember);
    OriginalJson m(std::strlen(main)+1,8,OriginalJson::Mode::FirstRootCommentsFirstMember);
    if(!d.Capture(reinterpret_cast<const unsigned char*>(dev),std::strlen(dev))||
        !m.Capture(reinterpret_cast<const unsigned char*>(main),std::strlen(main)))return false;
    unsigned specified=0;bool dependent=false;
    const auto first=Host::ObserveOrderedOptionSource(d,Host::ObserveOptionStage(d),true,specified,dependent);
    const auto second=Host::ObserveOrderedOptionSource(m,Host::ObserveOptionStage(m),false,specified,dependent,&first);
    return first.outcome==Stage::SourceReturnedFalse&&first.includedFrameworksAccepted==1&&
        first.lastFrameworkSource.outcome==OriginalFrameworkNameVersionSource::Outcome::SourceReturnedTrue&&
        first.lastFrameworkSource.versionSource.outcome==Outcome::SourceReturnedFalse&&
        second.outcome==Stage::SourceReturnedFalse&&second.includedFrameworksAccepted==1&&!dependent&&
        second.lastFrameworkSource.jsonOriginal==&m;
}
}
