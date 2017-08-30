#include "vbf_analysis/Utils/interface/GlobalStyle.h"

namespace plotmgr
{

    void SetGlobalStyle(){
        //Do not display any of the standard histogram decorations
        gStyle->SetOptTitle(0);
        gStyle->SetOptStat(0);
        gStyle->SetOptFit(0);
    }

}
