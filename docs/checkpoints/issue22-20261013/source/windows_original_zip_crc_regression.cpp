// UNRUN pure source vectors. Not a native package fixture or authority grant.
#include "windows_original_zip_crc.hpp"
namespace template_creator {
bool OriginalZipCrcSourceRegression() {
    const unsigned char joined[]="123456789adjacent";
    OriginalZipCrc first,empty,invalid;
    if(!first.Capture(joined,9)||!first.CompleteOriginal()||first.OriginalResult()!=0xcbf43926u||
        first.PrivateOriginal()!=joined||first.OriginalExtent()!=9||first.Capture(joined,sizeof(joined)-1))return false;
    if(!empty.Capture(nullptr,0)||!empty.CompleteOriginal()||empty.OriginalResult()!=0)return false;
    if(invalid.Capture(nullptr,1)||invalid.CompleteOriginal()||invalid.Capture(joined,9))return false;
    return true;
}
}
