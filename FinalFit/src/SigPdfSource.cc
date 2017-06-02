#include <cmath>
#include "vbf_analysis/FinalFit/interface/SigPdfSource.h"

#include "RooArgList.h"
#include "RooAbsReal.h"
#include "RooFormulaVar.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"

using namespace std;
using namespace RooFit;


RooAbsPdf* MultiGaussians (string signame, RooRealVar* mass, int nGaussians, int mh) {

    RooArgList* gaussians = new RooArgList();
    RooArgList* coeffs = new RooArgList();
    RooRealVar* MH = new RooRealVar("MH","MH",(double)mh,115.,135.);
    MH->setConstant(true);

    for (int i = 0; i < nGaussians; i++) {
        RooRealVar*  dm    = new RooRealVar(Form("%s_dm_mh%d_g%d",signame.c_str(),mh,i),Form("%s_dm_mh%d_g%d",signame.c_str(),mh,i),0.1,-3.,3.);
        RooAbsReal*  mean  = new RooFormulaVar(Form("%s_mean_mh%d_g%d",signame.c_str(),mh,i),Form("%s_mean_mh%d_g%d",signame.c_str(),mh,i),"@0+@1",RooArgList(*MH,*dm));
        RooRealVar*  sigma = new RooRealVar(Form("%s_sigma_mh%d_g%d",signame.c_str(),mh,i),Form("%s_sigma_mh%d_g%d",signame.c_str(),mh,i),2.,0.4,20.);
        RooGaussian* gaus  = new RooGaussian(Form("%s_gaus_mh%d_g%d",signame.c_str(),mh,i),Form("%s_gaus_mh%d_g%d",signame.c_str(),mh,i),*mass,*mean,*sigma);
        gaussians->add(*gaus);

        if (i < nGaussians - 1){
            RooRealVar*  frac  = new RooRealVar(Form("%s_frac_mh%d_g%d",signame.c_str(),mh,i),Form("%s_frac_mh%d_g%d",signame.c_str(),mh,i),0.1,0.01,0.99);
            coeffs->add(*frac);
        }
    }
    RooAddPdf* tmpMultiGaussians = new RooAddPdf(Form("PDF_%s%d",signame.c_str(),mh),Form("PDF_%s%d",signame.c_str(),mh),*gaussians,*coeffs);
    return tmpMultiGaussians;
}

