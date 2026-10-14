// UNCOMPILED source correspondence for corehost fx_ver_t, distinct from deps version_t.
#pragma once
#include "windows_original_json.hpp"
#include <array>
namespace template_creator {
struct OriginalFrameworkVersionSource {
    enum class Outcome {Unobserved,InvalidOriginal,SourceReturnedFalse,UnsignedConversionWouldThrow,
        SourceParsed,UnsignedToSignedAbiUnresolved};
    const OriginalJson* jsonOriginal=nullptr;
    size_t token=SIZE_MAX,convertedComponents=0,suffixBegin=SIZE_MAX,preExtent=0,
        buildBegin=SIZE_MAX,buildExtent=0,convertedPrefixScalars=0;
    std::array<std::uint32_t,3> components{};
    Outcome outcome=Outcome::Unobserved;
    bool attempted=false,prerelease=false;
    static OriginalFrameworkVersionSource Observe(const OriginalJson& json,size_t token) noexcept {
        OriginalFrameworkVersionSource r;r.attempted=true;r.jsonOriginal=&json;r.token=token;
        OriginalJson::OriginalStringCursor cursor;
        if(!json.Complete()||!json.BeginOriginalString(token,cursor)){r.outcome=Outcome::InvalidOriginal;return r;}
        size_t position=0;std::uint32_t scalar=0;bool end=false,invalid=false;
        auto next=[&]() noexcept {
            const auto step=json.NextOriginalString(cursor,scalar);
            invalid=step==OriginalJson::StringStep::Invalid;
            end=step==OriginalJson::StringStep::End||scalar==0;
            if(!invalid&&!end)++position;
        };
        auto fail=[&](Outcome outcome) noexcept {r.outcome=outcome;r.convertedPrefixScalars=position;return r;};
        next();
        for(size_t component=0;component<3;++component){
            size_t digits=0;std::uint32_t value=0;bool overflow=false,nonDigit=false,leadingZero=false;
            while(!invalid&&!end&&(component<2?scalar!='.':scalar>='0'&&scalar<='9')){
                if(scalar<'0'||scalar>'9')nonDigit=true;
                else {
                    if(!digits)leadingZero=scalar=='0';++digits;
                    const auto digit=scalar-'0';
                    if(value>(UINT32_MAX-digit)/10)overflow=true;
                    if(!overflow)value=value*10+digit;
                }
                next();
            }
            if(invalid)return fail(Outcome::InvalidOriginal);
            // Source searches for each major/minor separator before try_stou.
            if(component<2&&end)return fail(Outcome::SourceReturnedFalse);
            if(!digits||nonDigit)return fail(Outcome::SourceReturnedFalse);
            if(overflow)return fail(Outcome::UnsignedConversionWouldThrow);
            if(digits>1&&leadingZero)return fail(Outcome::SourceReturnedFalse);
            r.components[component]=value;++r.convertedComponents;
            if(component<2)next();
        }
        if(invalid)return fail(Outcome::InvalidOriginal);
        if(!end){
            r.suffixBegin=position-1;
            if(scalar!='-'&&scalar!='+')return fail(Outcome::SourceReturnedFalse);
            bool build=scalar=='+';r.prerelease=!build;
            if(build)r.buildBegin=position-1;
            size_t length=0;bool allNumeric=true,leadingZero=false;
            next();
            for(;;){
                if(invalid)return fail(Outcome::InvalidOriginal);
                if(end||scalar=='.'||(!build&&scalar=='+')){
                    if(!length||(!build&&allNumeric&&leadingZero&&length>1))return fail(Outcome::SourceReturnedFalse);
                    if(end)break;
                    if(scalar=='+'){
                        r.preExtent=position-1-r.suffixBegin;r.buildBegin=position-1;build=true;
                    }
                    length=0;allNumeric=true;leadingZero=false;next();continue;
                }
                const bool digit=scalar>='0'&&scalar<='9';
                if(!digit&&scalar!='-'&&!(scalar>='A'&&scalar<='Z')&&!(scalar>='a'&&scalar<='z'))
                    return fail(Outcome::SourceReturnedFalse);
                if(!length)leadingZero=scalar=='0';++length;allNumeric=allNumeric&&digit;next();
            }
            if(r.buildBegin!=SIZE_MAX)r.buildExtent=position-r.buildBegin;
            else r.preExtent=position-r.suffixBegin;
        }
        r.convertedPrefixScalars=position;r.outcome=Outcome::SourceParsed;
        for(const auto value:r.components)if(value>INT32_MAX)r.outcome=Outcome::UnsignedToSignedAbiUnresolved;
        return r;
    }
};
// Conditional Windows source case: these operands are borrowed original
// observations, not authenticated installed fx_ver_t objects or native results.
struct OriginalFrameworkVersionComparison {
    enum class Outcome {Unobserved,OperandUnresolved,InvalidOriginal,
        UnsignedConversionWouldThrow,SourceCompared};
    const OriginalFrameworkVersionSource *left=nullptr,*right=nullptr;
    size_t scalarComparisons=0,identifierBegin=SIZE_MAX;
    std::array<size_t,2> identifierExtents{};
    std::array<uint32_t,2> numericValues{};
    std::array<bool,2> numeric{};
    Outcome outcome=Outcome::Unobserved;
    int order=0;
    static bool PreScalar(const OriginalFrameworkVersionSource& source,size_t offset,uint32_t& scalar) noexcept {
        scalar=0;
        if(offset>=source.preExtent)return true;
        if(!source.jsonOriginal||source.suffixBegin>SIZE_MAX-offset)return false;
        OriginalJson::OriginalStringCursor cursor;
        if(!source.jsonOriginal->BeginOriginalString(source.token,cursor))return false;
        const size_t index=source.suffixBegin+offset;
        for(size_t i=0;;++i){
            const auto step=source.jsonOriginal->NextOriginalString(cursor,scalar);
            if(step!=OriginalJson::StringStep::Scalar||!scalar)return false;
            if(i==index)return true;
        }
    }
    static OriginalFrameworkVersionComparison Observe(const OriginalFrameworkVersionSource& a,
        const OriginalFrameworkVersionSource& b) noexcept {
        OriginalFrameworkVersionComparison r;r.left=&a;r.right=&b;
        using V=OriginalFrameworkVersionSource::Outcome;
        if(a.outcome!=V::SourceParsed||b.outcome!=V::SourceParsed){
            r.outcome=Outcome::OperandUnresolved;return r;
        }
        const auto done=[&](int order) noexcept {
            r.order=order;r.outcome=Outcome::SourceCompared;return r;
        };
        for(size_t i=0;i<3;++i)if(a.components[i]!=b.components[i])
            return done(a.components[i]>b.components[i]?1:-1);
        if(!a.prerelease||!b.prerelease)return done(!a.prerelease?int(b.prerelease):-1);
        size_t start=1;
        for(size_t i=1;;++i){
            uint32_t x=0,y=0;
            if(!PreScalar(a,i,x)||!PreScalar(b,i,y)){
                r.outcome=Outcome::InvalidOriginal;return r;
            }
            ++r.scalarComparisons;
            if(x==y){if(!x)return done(0);if(x=='.')start=i+1;continue;}
            if(!x&&y=='.')return done(-1);
            if(!y&&x=='.')return done(1);
            r.identifierBegin=start;
            const OriginalFrameworkVersionSource* operands[]={&a,&b};
            for(size_t operand=0;operand<2;++operand){
                bool allNumeric=true;
                size_t extent=0;
                for(size_t j=start;;++j){
                    uint32_t value=0;
                    if(!PreScalar(*operands[operand],j,value)){
                        r.outcome=Outcome::InvalidOriginal;return r;
                    }
                    if(!value||value=='.')break;
                    ++extent;allNumeric=allNumeric&&value>='0'&&value<='9';
                }
                r.identifierExtents[operand]=extent;
                r.numeric[operand]=extent&&allNumeric;
                // Actual try_stou rejects nonnumeric text before conversion.
                // Windows source unsigned long geometry is conditional here;
                // source-predicted overflow is not a caught native exception.
                if(r.numeric[operand])for(size_t j=0;j<extent;++j){
                    uint32_t value=0;
                    if(!PreScalar(*operands[operand],start+j,value)){
                        r.outcome=Outcome::InvalidOriginal;return r;
                    }
                    const uint32_t digit=value-'0';
                    if(r.numericValues[operand]>(UINT32_MAX-digit)/10){
                        r.outcome=Outcome::UnsignedConversionWouldThrow;return r;
                    }
                    r.numericValues[operand]=r.numericValues[operand]*10+digit;
                }
            }
            if(r.numeric[0]&&r.numeric[1])return done(r.numericValues[0]>r.numericValues[1]?1:-1);
            if(r.numeric[0]||r.numeric[1])return done(r.numeric[1]?1:-1);
            for(size_t j=0;;++j){
                uint32_t l=0,h=0;
                if(j<r.identifierExtents[0]&&!PreScalar(a,start+j,l)){
                    r.outcome=Outcome::InvalidOriginal;return r;
                }
                if(j<r.identifierExtents[1]&&!PreScalar(b,start+j,h)){
                    r.outcome=Outcome::InvalidOriginal;return r;
                }
                if(l!=h)return done(l>h?1:-1);
                if(!l)return done(0);
            }
        }
    }
};
struct OriginalFrameworkNameVersionSource {
    enum class Outcome {Unobserved,UnsupportedAccessor,SourceReturnedFalse,SourceReturnedTrue,
        ConversionWouldThrow,UnsignedToSignedAbiUnresolved};
    const OriginalJson* jsonOriginal=nullptr;
    size_t object=SIZE_MAX,name=SIZE_MAX,version=SIZE_MAX;
    OriginalFrameworkVersionSource versionSource;
    Outcome outcome=Outcome::Unobserved;
    bool attempted=false,nameAssigned=false,versionAssigned=false;
    static OriginalFrameworkNameVersionSource Observe(const OriginalJson& json,size_t object) noexcept {
        OriginalFrameworkNameVersionSource r;r.attempted=true;r.jsonOriginal=&json;r.object=object;
        if(!json.Complete()||object>=json.Count()||json.PrivateTokens()[object].kind!=OriginalJson::Kind::Object){
            r.outcome=Outcome::UnsupportedAccessor;return r;}
        r.name=json.Field(object,"name");
        if(r.name==SIZE_MAX){r.outcome=Outcome::SourceReturnedFalse;return r;}
        if(json.PrivateTokens()[r.name].kind!=OriginalJson::Kind::String){r.outcome=Outcome::UnsupportedAccessor;return r;}
        r.nameAssigned=true;r.version=json.Field(object,"version");
        if(r.version==SIZE_MAX){r.outcome=Outcome::SourceReturnedFalse;return r;}
        if(json.PrivateTokens()[r.version].kind!=OriginalJson::Kind::String){r.outcome=Outcome::UnsupportedAccessor;return r;}
        r.versionAssigned=true;r.versionSource=OriginalFrameworkVersionSource::Observe(json,r.version);
        using Version=OriginalFrameworkVersionSource::Outcome;
        if(r.versionSource.outcome==Version::UnsignedConversionWouldThrow){r.outcome=Outcome::ConversionWouldThrow;return r;}
        if(r.versionSource.outcome==Version::UnsignedToSignedAbiUnresolved){r.outcome=Outcome::UnsignedToSignedAbiUnresolved;return r;}
        if(r.versionSource.outcome!=Version::SourceParsed&&r.versionSource.outcome!=Version::SourceReturnedFalse){
            r.outcome=Outcome::UnsupportedAccessor;return r;}
        // set_fx_version ignores a false parse return; the name/version-only
        // parse_framework branch still returns true with its empty version object.
        r.outcome=Outcome::SourceReturnedTrue;return r;
    }
    static bool SameName(const OriginalJson& a,size_t x,const OriginalJson& b,size_t y) noexcept {
        OriginalJson::OriginalStringCursor left,right;
        if(!a.BeginOriginalString(x,left)||!b.BeginOriginalString(y,right))return false;
        for(;;){
            std::uint32_t l=0,r=0;const auto ls=a.NextOriginalString(left,l),rs=b.NextOriginalString(right,r);
            if(ls==OriginalJson::StringStep::Invalid||rs==OriginalJson::StringStep::Invalid)return false;
            const bool le=ls==OriginalJson::StringStep::End||!l,re=rs==OriginalJson::StringStep::End||!r;
            if(le||re)return le&&re;if(l!=r)return false;
        }
    }
};
}
