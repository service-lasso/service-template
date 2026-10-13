// UNCOMPILED source path stage only. .NET v10.0.5 deps_entry.cpp and
// hostmisc/utils.cpp append_path/get_filename/get_directory. No native probe,
// bundle resolution, installed build equivalence or provider admission.
#pragma once
#include <cstddef>
#include <cstdint>
namespace template_creator {
struct OriginalResolverPathSource {
    struct Span {const char16_t* data=nullptr;std::size_t units=0;};
    enum class Layout {Directory,Package,LibraryPackage};
    enum class Asset {Runtime,Resources,Native};
    enum class Outcome {Unobserved,SourceFalseEmptyBase,Formed,InvalidOriginal,InsufficientPrecharge};
    Span baseOriginal{},relativeOriginal{},localOriginal{},libraryPathOriginal{},libraryNameOriginal{},libraryVersionOriginal{};
    char16_t* destinationOriginal=nullptr;std::size_t capacity=0,written=0;
    Layout layout=Layout::Directory;Asset asset=Asset::Runtime;
    bool runtimePack=false,attempted=false;
    Outcome outcome=Outcome::Unobserved;
    static bool Valid(Span s) noexcept{return s.data||!s.units;}
    static char16_t Unit(Span s,std::size_t i,bool normalized) noexcept {
        return normalized&&s.data[i]==u'/'?u'\\':s.data[i];
    }
    bool Put(char16_t c) noexcept {
        // One terminator unit remains reserved. On denial retained prefix is
        // source computation only and is never published as a candidate.
        if(!destinationOriginal||written>=capacity||capacity-written<=1){outcome=Outcome::InsufficientPrecharge;return false;}
        destinationOriginal[written++]=c;destinationOriginal[written]=0;return true;
    }
    bool AssignFull(Span s) noexcept {
        for(std::size_t i=0;i<s.units;++i)if(!Put(s.data[i]))return false;
        return true;
    }
    bool AppendCString(Span s,bool normalized) noexcept {
        std::size_t count=0;while(count<s.units&&Unit(s,count,normalized))++count;
        if(!count)return true;
        if(written&&destinationOriginal[written-1]!=u'\\'&&Unit(s,0,normalized)!=u'\\')if(!Put(u'\\'))return false;
        for(std::size_t i=0;i<count;++i)if(!Put(Unit(s,i,normalized)))return false;
        return true;
    }
    static Span Slice(Span s,std::size_t begin,std::size_t end) noexcept {
        return {s.data?s.data+begin:nullptr,end-begin};
    }
    static std::size_t FilenameBegin(Span s) noexcept {
        for(std::size_t i=s.units;i;--i)if(Unit(s,i-1,true)==u'\\')return i;
        return 0;
    }
    bool Form(Span base,Span relative,Span local,Span libraryPath,Span libraryName,Span libraryVersion,
        Layout requested,Asset kind,bool isRuntimePack,char16_t* output,std::size_t outputUnits) noexcept {
        if(attempted)return false;
        attempted=true;baseOriginal=base;relativeOriginal=relative;localOriginal=local;
        libraryPathOriginal=libraryPath;libraryNameOriginal=libraryName;libraryVersionOriginal=libraryVersion;
        layout=requested;asset=kind;runtimePack=isRuntimePack;destinationOriginal=output;capacity=outputUnits;
        if(!Valid(base)||!Valid(relative)||!Valid(local)||!Valid(libraryPath)||!Valid(libraryName)||!Valid(libraryVersion)||
           !output||!outputUnits||(requested!=Layout::Directory&&requested!=Layout::Package&&requested!=Layout::LibraryPackage)||
           (kind!=Asset::Runtime&&kind!=Asset::Resources&&kind!=Asset::Native)){
            outcome=Outcome::InvalidOriginal;return false;
        }
        output[0]=0;
        if(!base.units){outcome=Outcome::SourceFalseEmptyBase;return false;}
        // Caller supplies nonoverlapping preowned output and stable original
        // spans through this synchronous call; pointer shape is not ownership.
        if(!AssignFull(base))return false;
        if(requested==Layout::LibraryPackage){
            if(libraryPath.units){if(!AppendCString(libraryPath,false))return false;}
            else if(!AppendCString(libraryName,false)||!AppendCString(libraryVersion,false))return false;
        }
        if(requested!=Layout::Directory){
            if(!AppendCString(relative,true))return false;
        }else if(local.units){
            if(!AppendCString(local,true))return false;
        }else if(isRuntimePack){
            if(!AppendCString(relative,true))return false;
        }else{
            const auto filename=FilenameBegin(relative);
            if(kind==Asset::Resources){
                // get_directory strips trailing separators, then removes the
                // last separator run before appending one. Removing that final
                // separator and taking get_filename gives the culture span.
                std::size_t end=relative.units;
                while(end&&Unit(relative,end-1,true)==u'\\')--end;
                std::size_t separator=end;
                while(separator&&Unit(relative,separator-1,true)!=u'\\')--separator;
                std::size_t cultureEnd=end;
                if(separator){cultureEnd=separator-1;while(cultureEnd&&Unit(relative,cultureEnd-1,true)==u'\\')--cultureEnd;}
                const auto parent=Slice(relative,0,cultureEnd);
                const auto cultureBegin=FilenameBegin(parent);
                // Upstream constructs relative_path from the full culture
                // string, then append_path(file_name.c_str()), then finally
                // appends this constructed relative_path.c_str() to base.
                // Embedded NUL may therefore suppress filename entirely.
                const auto culture=Slice(relative,cultureBegin,cultureEnd);
                bool containsNul=false;for(std::size_t i=0;i<culture.units;++i)if(!Unit(culture,i,true)){containsNul=true;break;}
                if(!AppendCString(culture,true))return false;
                if(!containsNul&&!AppendCString(Slice(relative,filename,relative.units),true))return false;
            }else if(!AppendCString(Slice(relative,filename,relative.units),true))return false;
        }
        outcome=Outcome::Formed;return true;
    }
};
}