// UNRUN synthetic structural rows through the actual owning consumer algorithm.
// No native A/unit owner, authentic source, IO or useful admission is fabricated.
#include "linux_received_source_unit.hpp"
namespace template_creator_linux {
struct InitialProviderUnitRowsSourceRegression {
    static bool Run() noexcept {
        struct slcli_source_association_record a[2]{};
        struct slcli_source_unit12_record unit[2]{};
        for(size_t i=0;i<2;++i){
            a[i].index=unit[i].index=i;a[i].project=unit[i].source_project=7;
            a[i].kind=unit[i].kind=3;a[i].body_bytes=unit[i].bytes=i+1;
            for(size_t j=0;j<32;++j){a[i].association[j]=unit[i].association[j]=static_cast<unsigned char>(i+j+1);
                a[i].capture[j]=unit[i].capture[j]=static_cast<unsigned char>(j+2);
                a[i].digest[j]=unit[i].digest[j]=static_cast<unsigned char>(j+3);}
        }
        size_t compared=SIZE_MAX;
        const auto compare=[&](size_t count) noexcept {
            return OriginalReceivedSourceUnitObservation::CompareAssociationRowsOriginal(a,2,unit,count,compared);
        };
        if(!compare(2)||compared!=2)return false;
        ++unit[1].bytes;if(compare(2)||compared!=1)return false;--unit[1].bytes;
        unit[1].index=0;if(compare(2)||compared!=1)return false;unit[1].index=1;
        unit[1].capture[31]^=1;if(compare(2)||compared!=1)return false;unit[1].capture[31]^=1;
        unit[1].digest[0]^=1;if(compare(2)||compared!=1)return false;unit[1].digest[0]^=1;
        if(compare(1)||compared!=0)return false;
        if(OriginalReceivedSourceUnitObservation::CompareAssociationRowsOriginal(nullptr,2,unit,2,compared)||compared!=0)return false;
        return compare(2)&&compared==2;
    }
};
}
int main(){return template_creator_linux::InitialProviderUnitRowsSourceRegression::Run()?0:1;}
