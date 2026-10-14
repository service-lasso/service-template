// UNRUN source regressions. Pure decoder fixtures are not native input,
// framework/provider selection, installed runtime or authority evidence.
#include "windows_original_json.hpp"
#include <cstring>
namespace template_creator {
bool OriginalJsonSourceRegression() {
    const unsigned char emptyObject[]="{}";
    OriginalJson deferred(2,2,OriginalJson::Mode::StrictCompleteUnique,OriginalJson::DeferredStorage{});
    if(deferred.PrivateTokens()||deferred.RetainedFramesOriginal()||
        deferred.ReservedSourceBytes()!=sizeof(deferred)||deferred.Capture(emptyObject,2))return false;
    if(!deferred.PreownOriginal()||!deferred.PrivateTokens()||!deferred.RetainedFramesOriginal()||
        deferred.RetainedTokenCapacityOriginal()!=2||deferred.PreownOriginal()||
        !deferred.Capture(emptyObject,2))return false;
    OriginalJson invalid(0,0,OriginalJson::Mode::StrictCompleteUnique,OriginalJson::DeferredStorage{});
    if(invalid.PreownOriginal()||!invalid.PreownExceptionOriginal()||invalid.PrivateTokens()||
        invalid.RetainedFramesOriginal()||invalid.ReservedSourceBytes()!=sizeof(invalid)||invalid.Capture(emptyObject,2))return false;
    const unsigned char config[]="{\"runtimeOptions\":{\"tfm\":\"net10.0\",\"frameworks\":[{\"name\":\"Microsoft.NETCore.App\",\"version\":\"10.0.5\"}]}}";
    OriginalJson original(sizeof(config),6);
    if(!original.Capture(config,sizeof(config)-1)||original.PrivateOriginalBody()!=config)return false;
    const auto options=original.Field(0,"runtimeOptions");
    if(!original.StringEquals(original.Field(options,"tfm"),"net10.0")||
        original.Field(options,"absent")!=SIZE_MAX||original.Capture(config,sizeof(config)-1))return false;
    const unsigned char escaped[]="{\"runtim\\u0065Options\":{\"tfm\":\"\"}}";
    OriginalJson escape(sizeof(escaped),3);
    if(!escape.Capture(escaped,sizeof(escaped)-1)||
        !escape.StringEquals(escape.Field(escape.Field(0,"runtimeOptions"),"tfm"),""))return false;
    const unsigned char duplicate[]="{\"x\":0,\"\\u0078\":1}";
    OriginalJson duplicates(sizeof(duplicate),2);
    if(duplicates.Capture(duplicate,sizeof(duplicate)-1))return false;
    const unsigned char malformed[][8]={{'0','1'},{'[','1',',',']'}, {'"',0xc0,0x80,'"'},
        {'"','\\','u','D','C','0','0','"'}, {'{','}','x'}, {'"',0,'"'}};
    const size_t lengths[]={2,4,4,8,3,3};
    for(size_t i=0;i<sizeof(lengths)/sizeof(*lengths);++i){
        OriginalJson rejected(16,4);
        if(rejected.Capture(malformed[i],lengths[i]))return false;
    }
    const unsigned char pair[]="\"\\uD83D\\uDE00\"";
    OriginalJson surrogate(sizeof(pair),1);
    if(!surrogate.Capture(pair,sizeof(pair)-1))return false;
    const unsigned char bom[]={0xef,0xbb,0xbf,'{','}'};
    OriginalJson marked(1,0);if(!marked.Capture(bom,sizeof(bom)))return false;
    const unsigned char capacity[]="[0,1]";
    OriginalJson exhausted(2,2);if(exhausted.Capture(capacity,sizeof(capacity)-1))return false;
    OriginalJson depth(sizeof(capacity),0);if(depth.Capture(capacity,sizeof(capacity)-1))return false;
    const unsigned char targets[]="{\"runtimeTarget\":{\"name\":\"net10.0/win-x64\"},\"targets\":{\"net10.0/win-x64\":{\"lib/1\":{\"runtime\":{\"asset.dll\":{}}}}}}";
    OriginalJson named(sizeof(targets),sizeof(targets));
    if(!named.Capture(targets,sizeof(targets)-1))return false;
    const auto name=named.Field(named.Field(0,"runtimeTarget"),"name");
    const auto target=named.FieldForOriginalString(named.Field(0,"targets"),name);
    if(target==SIZE_MAX||named.Field(target,"lib/1")==SIZE_MAX||
        named.FieldForOriginalString(named.Field(0,"targets"),SIZE_MAX)!=SIZE_MAX)return false;
    const unsigned char stringTarget[]="{\"runtimeTarget\":\"net10.0/win-x64\",\"targets\":{\"net10.0/win-x64\":{}}}";
    OriginalJson directTarget(sizeof(stringTarget),sizeof(stringTarget),OriginalJson::Mode::FirstRootCommentsFirstMember);
    if(!directTarget.Capture(stringTarget,sizeof(stringTarget)-1)||
        directTarget.FieldForOriginalString(directTarget.Field(0,"targets"),directTarget.Field(0,"runtimeTarget"))==SIZE_MAX)return false;
    const unsigned char host[]="/*before*/{\"x\":\"first\",\"\\u0078\":\"second\",\"nested\":[//line\n{}]}/*unfinished ignored suffix";
    OriginalJson hostView(sizeof(host),sizeof(host),OriginalJson::Mode::FirstRootCommentsFirstMember);
    OriginalJson strictView(sizeof(host),sizeof(host));
    if(!hostView.Capture(host,sizeof(host)-1)||strictView.Capture(host,sizeof(host)-1)||
        !hostView.DuplicateNamesObserved()||!hostView.StringEquals(hostView.Field(0,"x"),"first")||
        hostView.ParsedExtent()>=hostView.OriginalExtent())return false;
    const unsigned char nulBefore[]={'/','/',0,'\n','{','}'};
    OriginalJson truncated(sizeof(nulBefore),2,OriginalJson::Mode::FirstRootCommentsFirstMember);
    if(truncated.Capture(nulBefore,sizeof(nulBefore))||truncated.ParserStreamExtent()!=2||
        truncated.OriginalExtent()!=sizeof(nulBefore))return false;
    const unsigned char nulAfter[]={'{','}',0,'x'};
    OriginalJson prefix(sizeof(nulAfter),2,OriginalJson::Mode::FirstRootCommentsFirstMember);
    if(!prefix.Capture(nulAfter,sizeof(nulAfter))||prefix.ParsedExtent()!=2||
        prefix.OriginalExtent()!=4||prefix.ParserStreamExtent()!=2)return false;
    const unsigned char badComment[]="/*unfinished";
    OriginalJson bad(sizeof(badComment),2,OriginalJson::Mode::FirstRootCommentsFirstMember);
    if(bad.Capture(badComment,sizeof(badComment)-1))return false;
    // GetString()+C-string construction differs from full JSON key equality.
    const unsigned char cstrings[]="{\"a\":\"lib\\u0000one\",\"b\":\"lib\\u0000two\",\"c\":\"lib\",\"d\":\"other\",\"e\":\"\\u0000tail\",\"f\":\"\"}";
    OriginalJson views(sizeof(cstrings),sizeof(cstrings));
    if(!views.Capture(cstrings,sizeof(cstrings)-1))return false;
    const auto a=views.Field(0,"a"),b=views.Field(0,"b"),c=views.Field(0,"c");
    if(views.SameOriginalString(a,b)||views.SameOriginalString(a,c)||
        !views.SameOriginalCString(a,b)||!views.SameOriginalCString(a,c)||
        views.SameOriginalCString(a,views.Field(0,"d"))||
        !views.SameOriginalCString(views.Field(0,"e"),views.Field(0,"f"))||
        views.SameOriginalCString(a,SIZE_MAX))return false;
    const unsigned char lookup[]="{\"name\":\"target\\u0000suffix\",\"targets\":{\"target\\u0000suffix\":\"full\",\"target\":\"cstring\"}}";
    OriginalJson names(sizeof(lookup),sizeof(lookup),OriginalJson::Mode::FirstRootCommentsFirstMember);
    if(!names.Capture(lookup,sizeof(lookup)-1))return false;
    const auto targetObject=names.Field(0,"targets"),targetName=names.Field(0,"name");
    if(!names.StringEquals(names.FieldForOriginalString(targetObject,targetName),"full")||
        !names.StringEquals(names.FieldForOriginalCString(targetObject,targetName),"cstring"))return false;
    const unsigned char missingCString[]="{\"name\":\"target\\u0000suffix\",\"targets\":{\"target\\u0000suffix\":{}}}";
    OriginalJson noPlain(sizeof(missingCString),sizeof(missingCString));
    if(!noPlain.Capture(missingCString,sizeof(missingCString)-1)||
        noPlain.FieldForOriginalCString(noPlain.Field(0,"targets"),noPlain.Field(0,"name"))!=SIZE_MAX||
        noPlain.FieldForOriginalCString(0,SIZE_MAX)!=SIZE_MAX)return false;
    return true;
}
}
