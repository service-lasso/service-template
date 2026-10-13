// UNCOMPILED original-byte JSON reader. Syntax/field correspondence is not
// host resolution, managed assembly selection, or vendor/build authority.
#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <exception>
namespace template_creator {
class OriginalJson {
public:
    enum class Mode {StrictCompleteUnique,FirstRootCommentsFirstMember};
    struct DeferredStorage {};
    enum class Kind {Object,Array,String,Number,True,False,Null};
    struct Token {
        Kind kind=Kind::Null;
        size_t begin=0,end=0,parent=SIZE_MAX,subtreeEnd=0;
    };
private:
    const unsigned char* body_=nullptr;
    size_t extent_=0,originalExtent_=0,position_=0,count_=0,capacity_=0,depthLimit_=0;
    std::unique_ptr<Token[]> tokens_;
    struct Frame {size_t token=0;unsigned phase=0;};
    std::unique_ptr<Frame[]> frames_;
    size_t frameCount_=0;
    bool attempted_=false,complete_=false;
    bool syntaxFailed_=false,duplicateNamesObserved_=false,numericLexemesObserved_=false;
    size_t failureOffset_=0,parsedExtent_=0;
    Mode mode_=Mode::StrictCompleteUnique;
    static unsigned Hex(unsigned c) noexcept {
        return c>='0'&&c<='9'?c-'0':c>='a'&&c<='f'?c-'a'+10:c>='A'&&c<='F'?c-'A'+10:16;
    }
    static bool Scalar(const unsigned char* b,size_t end,size_t& p,uint32_t& value) noexcept {
        if(p==end)return false;
        unsigned c=b[p++];
        if(c<0x80){value=c;return true;}
        unsigned need=0;uint32_t minimum=0;
        if(c>=0xc2&&c<=0xdf){need=1;value=c&31;minimum=0x80;}
        else if(c>=0xe0&&c<=0xef){need=2;value=c&15;minimum=0x800;}
        else if(c>=0xf0&&c<=0xf4){need=3;value=c&7;minimum=0x10000;}
        else return false;
        if(need>end-p)return false;
        while(need--){c=b[p++];if((c&0xc0)!=0x80)return false;value=(value<<6)|(c&63);}
        return value>=minimum&&value<=0x10ffff&&!(value>=0xd800&&value<=0xdfff);
    }
    static bool StringScalar(const unsigned char* b,size_t end,size_t& p,uint32_t& value) noexcept {
        if(p==end)return false;
        if(b[p]!='\\')return Scalar(b,end,p,value)&&value>=0x20;
        ++p;if(p==end)return false;unsigned c=b[p++];
        switch(c){
            case '"':case '\\':case '/':value=c;return true;
            case 'b':value=8;return true;case 'f':value=12;return true;
            case 'n':value=10;return true;case 'r':value=13;return true;case 't':value=9;return true;
            case 'u':break;default:return false;
        }
        if(end-p<4)return false;value=0;
        for(unsigned i=0;i<4;++i){unsigned h=Hex(b[p++]);if(h==16)return false;value=(value<<4)|h;}
        if(value>=0xdc00&&value<=0xdfff)return false;
        if(value<0xd800||value>0xdbff)return true;
        if(end-p<6||b[p++]!='\\'||b[p++]!='u')return false;
        uint32_t low=0;for(unsigned i=0;i<4;++i){unsigned h=Hex(b[p++]);if(h==16)return false;low=(low<<4)|h;}
        if(low<0xdc00||low>0xdfff)return false;
        value=0x10000+((value-0xd800)<<10)+(low-0xdc00);return true;
    }
    void Space() noexcept {
        while(position_<extent_){
            const unsigned c=body_[position_];
            if(c==' '||c=='\t'||c=='\r'||c=='\n'){++position_;continue;}
            if(mode_!=Mode::FirstRootCommentsFirstMember||c!='/'||extent_-position_<2)return;
            const unsigned next=body_[position_+1];
            if(next=='/'){
                position_+=2;
                while(position_<extent_&&body_[position_]!='\r'&&body_[position_]!='\n')++position_;
            }else if(next=='*'){
                position_+=2;bool closed=false;
                while(extent_-position_>=2){
                    if(body_[position_]=='*'&&body_[position_+1]=='/'){position_+=2;closed=true;break;}
                    ++position_;
                }
                if(!closed){syntaxFailed_=true;position_=extent_;return;}
            }else return;
        }
    }
    bool Take(unsigned c) noexcept {if(position_==extent_||body_[position_]!=c)return false;++position_;return true;}
    bool Literal(const char* text) noexcept {
        while(*text){if(position_==extent_||body_[position_++]!=static_cast<unsigned char>(*text++))return false;}return true;
    }
    bool String() noexcept {
        if(!Take('"'))return false;
        while(position_<extent_){
            if(body_[position_]=='"'){++position_;return true;}
            uint32_t scalar=0;if(!StringScalar(body_,extent_,position_,scalar))return false;
        }return false;
    }
    bool Number() noexcept {
        Take('-');if(position_==extent_)return false;
        if(!Take('0')){
            if(body_[position_]<'1'||body_[position_]>'9')return false;
            do{++position_;}while(position_<extent_&&body_[position_]>='0'&&body_[position_]<='9');
        }
        if(Take('.')){
            const size_t first=position_;
            while(position_<extent_&&body_[position_]>='0'&&body_[position_]<='9')++position_;
            if(first==position_)return false;
        }
        if(position_<extent_&&(body_[position_]=='e'||body_[position_]=='E')){
            ++position_;if(position_<extent_&&(body_[position_]=='+'||body_[position_]=='-'))++position_;
            const size_t first=position_;
            while(position_<extent_&&body_[position_]>='0'&&body_[position_]<='9')++position_;
            if(first==position_)return false;
        }return true;
    }
    bool SameString(size_t a,size_t b) const noexcept {
        size_t x=tokens_[a].begin+1,y=tokens_[b].begin+1;
        const size_t xe=tokens_[a].end-1,ye=tokens_[b].end-1;
        while(x<xe&&y<ye){uint32_t u=0,v=0;if(!StringScalar(body_,xe,x,u)||!StringScalar(body_,ye,y,v)||u!=v)return false;}
        return x==xe&&y==ye;
    }
    bool Atom(size_t parent) noexcept {
        Space();if(syntaxFailed_||position_==extent_||frameCount_>depthLimit_||count_==capacity_)return false;
        const size_t index=count_++;auto& row=tokens_[index];row.begin=position_;row.parent=parent;
        const unsigned c=body_[position_];
        if(c=='{'||c=='['){
            const bool object=c=='{';row.kind=object?Kind::Object:Kind::Array;++position_;Space();
            const unsigned finish=object?'}':']';
            if(!Take(finish)){
                if(frameCount_==capacity_)return false;
                frames_[frameCount_++]={index,0};return true;
            }
        }else if(c=='"'){row.kind=Kind::String;if(!String())return false;}
        else if(c=='t'){row.kind=Kind::True;if(!Literal("true"))return false;}
        else if(c=='f'){row.kind=Kind::False;if(!Literal("false"))return false;}
        else if(c=='n'){row.kind=Kind::Null;if(!Literal("null"))return false;}
        else{row.kind=Kind::Number;numericLexemesObserved_=true;if(!Number())return false;}
        row.end=position_;row.subtreeEnd=count_;return true;
    }
    bool Value() noexcept {
        if(!Atom(SIZE_MAX))return false;
        while(frameCount_){
            auto& frame=frames_[frameCount_-1];auto& container=tokens_[frame.token];
            const bool object=container.kind==Kind::Object;
            if(frame.phase==0&&object){
                Space();if(position_==extent_||body_[position_]!='"'||frameCount_>depthLimit_||count_==capacity_)return false;
                const size_t key=count_++;auto& row=tokens_[key];
                row.kind=Kind::String;row.begin=position_;row.parent=frame.token;
                if(!String())return false;row.end=position_;row.subtreeEnd=count_;
                // Reject ambiguous decoded names, including escaped spellings.
                for(size_t old=frame.token+1;old<key;){
                    if(SameString(old,key)){
                        duplicateNamesObserved_=true;
                        if(mode_==Mode::StrictCompleteUnique)return false;
                    }
                    const size_t value=tokens_[old].subtreeEnd;
                    if(value>=key)return false;old=tokens_[value].subtreeEnd;
                }
                Space();if(!Take(':'))return false;frame.phase=1;
            }
            if(frame.phase<=1){
                const size_t parent=frame.token;frame.phase=2;
                if(!Atom(parent))return false;continue;
            }
            Space();
            if(Take(object?'}':']')){
                container.end=position_;container.subtreeEnd=count_;--frameCount_;
            }else{
                if(!Take(','))return false;frame.phase=0;
            }
        }return true;
    }
public:
    // Caller precharges selected input-derived token and nesting storage before
    // native acquisition. Parsing uses this heap frame roster, not recursive
    // native-stack growth. Original body/allocator/host overhead remain separate.
    static size_t DynamicSourceBytesFor(size_t tokenCapacity) {
        if(!tokenCapacity||tokenCapacity>(SIZE_MAX-sizeof(OriginalJson))/(sizeof(Token)+sizeof(Frame)))throw std::invalid_argument("original JSON geometry");
        return tokenCapacity*(sizeof(Token)+sizeof(Frame));
    }
    OriginalJson(size_t tokenCapacity,size_t depthLimit,Mode mode=Mode::StrictCompleteUnique)
        :OriginalJson(tokenCapacity,depthLimit,mode,DeferredStorage{}) {
        if(!PreownOriginal())std::rethrow_exception(preownExceptionOriginal_);
    }
    OriginalJson(size_t tokenCapacity,size_t depthLimit,Mode mode,DeferredStorage)
        :capacity_(tokenCapacity),depthLimit_(depthLimit),mode_(mode) {}
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        try {
        const size_t tokenCapacity=capacity_;
        DynamicSourceBytesFor(tokenCapacity);
        tokens_=std::make_unique<Token[]>(tokenCapacity);
        frames_=std::make_unique<Frame[]>(tokenCapacity);
        preowned_=true;return true;
        }catch(...){preownExceptionOriginal_=std::current_exception();return false;}
    }
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownExceptionOriginal_;}
    OriginalJson(const OriginalJson&)=delete;
    OriginalJson& operator=(const OriginalJson&)=delete;
    bool Capture(const unsigned char* originalBody,size_t originalExtent) noexcept {
        if(!preowned_||attempted_)return false;attempted_=true;body_=originalBody;extent_=originalExtent;originalExtent_=originalExtent;
        if(!body_||!extent_)return false;
        // The referenced host Parse(data) uses a zero-terminated stream. Keep
        // the full held file original while bounding this distinct parser view
        // at its first raw NUL. Escaped JSON \u0000 remains a decoded scalar.
        if(mode_==Mode::FirstRootCommentsFirstMember)
            for(size_t i=0;i<extent_;++i)if(body_[i]==0){extent_=i;break;}
        // Retain the BOM in the original body; permit only the leading UTF-8
        // marker, never an embedded marker as JSON whitespace.
        if(extent_>=3&&body_[0]==0xef&&body_[1]==0xbb&&body_[2]==0xbf)position_=3;
        complete_=Value();parsedExtent_=position_;
        if(mode_==Mode::StrictCompleteUnique){Space();complete_=complete_&&!syntaxFailed_&&position_==extent_;}
        else complete_=complete_&&!syntaxFailed_;
        failureOffset_=position_;return complete_;
    }
    class OriginalStringCursor {
        friend class OriginalJson;
        const OriginalJson* owner_=nullptr;
        size_t token_=SIZE_MAX,position_=0,end_=0;
        bool valid_=false;
    public:
        bool Valid() const noexcept{return valid_;}
    };
    enum class StringStep {Scalar,End,Invalid};
    bool BeginOriginalString(size_t token,OriginalStringCursor& cursor) const noexcept {
        if(!complete_||token>=count_||tokens_[token].kind!=Kind::String)return false;
        cursor.owner_=this;cursor.token_=token;cursor.position_=tokens_[token].begin+1;
        cursor.end_=tokens_[token].end-1;cursor.valid_=true;return true;
    }
    StringStep NextOriginalString(OriginalStringCursor& cursor,uint32_t& scalar) const noexcept {
        if(cursor.owner_!=this||!cursor.valid_||!complete_||cursor.token_>=count_||
            cursor.position_>cursor.end_){cursor.valid_=false;return StringStep::Invalid;}
        if(cursor.position_==cursor.end_)return StringStep::End;
        if(!StringScalar(body_,cursor.end_,cursor.position_,scalar)){
            cursor.valid_=false;return StringStep::Invalid;
        }
        return StringStep::Scalar;
    }
    bool SameOriginalString(size_t first,size_t second) const noexcept {
        return complete_&&first<count_&&second<count_&&
            tokens_[first].kind==Kind::String&&tokens_[second].kind==Kind::String&&SameString(first,second);
    }
    // Only for consuming source APIs which construct a C string from GetString().
    // Original JSON spelling, full token and trusted-input equality stay intact.
    bool SameOriginalCString(size_t first,size_t second) const noexcept {
        if(!complete_||first>=count_||second>=count_||
            tokens_[first].kind!=Kind::String||tokens_[second].kind!=Kind::String)return false;
        size_t x=tokens_[first].begin+1,y=tokens_[second].begin+1;
        const size_t xe=tokens_[first].end-1,ye=tokens_[second].end-1;
        for(;;){
            uint32_t a=0,b=0;
            const bool ax=x<xe,by=y<ye;
            if(ax&&!StringScalar(body_,xe,x,a))return false;
            if(by&&!StringScalar(body_,ye,y,b))return false;
            const bool aEnd=!ax||a==0,bEnd=!by||b==0;
            if(aEnd||bEnd)return aEnd&&bEnd;
            if(a!=b)return false;
        }
    }
    bool StringEquals(size_t token,const char* ascii) const noexcept {
        if(!complete_||!ascii||token>=count_||tokens_[token].kind!=Kind::String)return false;
        size_t p=tokens_[token].begin+1;const size_t end=tokens_[token].end-1;
        while(*ascii){uint32_t value=0;if(p==end||!StringScalar(body_,end,p,value)||value!=static_cast<unsigned char>(*ascii++))return false;}
        return p==end;
    }
    size_t Field(size_t object,const char* ascii) const noexcept {
        if(!complete_||object>=count_||tokens_[object].kind!=Kind::Object)return SIZE_MAX;
        for(size_t key=object+1;key<tokens_[object].subtreeEnd;){
            const size_t value=tokens_[key].subtreeEnd;
            if(StringEquals(key,ascii))return value;key=tokens_[value].subtreeEnd;
        }return SIZE_MAX;
    }
    size_t FieldForOriginalString(size_t object,size_t originalKey) const noexcept {
        if(!complete_||object>=count_||originalKey>=count_||tokens_[object].kind!=Kind::Object||
            tokens_[originalKey].kind!=Kind::String)return SIZE_MAX;
        for(size_t key=object+1;key<tokens_[object].subtreeEnd;){
            const size_t value=tokens_[key].subtreeEnd;
            if(SameString(key,originalKey))return value;key=tokens_[value].subtreeEnd;
        }return SIZE_MAX;
    }
    // FindMember(const Ch*) creates a strlen-sized lookup name, but retains
    // the JSON member's complete decoded length, including embedded NULs.
    size_t FieldForOriginalCString(size_t object,size_t originalKey) const noexcept {
        if(!complete_||object>=count_||originalKey>=count_||tokens_[object].kind!=Kind::Object||
            tokens_[originalKey].kind!=Kind::String)return SIZE_MAX;
        for(size_t key=object+1;key<tokens_[object].subtreeEnd;){
            const size_t value=tokens_[key].subtreeEnd;
            size_t x=tokens_[key].begin+1,y=tokens_[originalKey].begin+1;
            const size_t xe=tokens_[key].end-1,ye=tokens_[originalKey].end-1;
            bool equal=false;
            for(;;){
                uint32_t a=0,b=0;const bool ax=x<xe,by=y<ye;
                if(ax&&!StringScalar(body_,xe,x,a))return SIZE_MAX;
                if(by&&!StringScalar(body_,ye,y,b))return SIZE_MAX;
                const bool aEnd=!ax,bEnd=!by||b==0;
                if(aEnd||bEnd){equal=aEnd&&bEnd;break;}
                if(a!=b)break;
            }
            if(equal)return value;key=tokens_[value].subtreeEnd;
        }
        return SIZE_MAX;
    }
    bool Complete() const noexcept{return complete_;}
    const Token* PrivateTokens() const noexcept{return tokens_.get();}
    const unsigned char* PrivateOriginalBody() const noexcept{return body_;}
    size_t Count() const noexcept{return count_;}
    size_t FailureOffset() const noexcept{return failureOffset_;}
    size_t ParsedExtent() const noexcept{return parsedExtent_;}
    size_t OriginalExtent() const noexcept{return originalExtent_;}
    size_t ParserStreamExtent() const noexcept{return extent_;}
    bool DuplicateNamesObserved() const noexcept{return duplicateNamesObserved_;}
    bool NumericLexemesObserved() const noexcept{return numericLexemesObserved_;}
    Mode OriginalMode() const noexcept{return mode_;}
    size_t ReservedSourceBytes() const noexcept{return sizeof(*this)+(tokens_?capacity_*sizeof(Token):0)+(frames_?capacity_*sizeof(Frame):0);}
    size_t RetainedTokenCapacityOriginal() const noexcept{return tokens_?capacity_:0;}
    const void* RetainedFramesOriginal() const noexcept{return frames_.get();}
    size_t RetainedFrameBytesOriginal() const noexcept{return frames_?capacity_*sizeof(Frame):0;}
private:
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownExceptionOriginal_;
    // Numbers retain lexical bytes only. Native host integer/double conversion,
    // range errors and config-property formatting are not modeled by this
    // decoder; this framing/lookup mode alone cannot prove host equivalence.
};
}
