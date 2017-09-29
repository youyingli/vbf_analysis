#include <cmath>
#include "vbf_analysis/FinalFit/interface/SigPdfSource.h"

#include "RooArgList.h"
#include "RooAbsReal.h"
#include "RooFormulaVar.h"
#include "RooGaussian.h"
#include "RooCBShape.h"
#include "RooAddPdf.h"

using namespace std;
using namespace RooFit;


RooAbsPdf* MultiGaussians (string signame, RooRealVar* mass, int nGaussians, int mh) {

    RooArgList* gaussians = new RooArgList();
    RooArgList* coeffs = new RooArgList();
    //RooRealVar* MH = new RooRealVar("MH","MH",(double)mh,115.,135.);
    //MH->setConstant(true);

    for (int i = 0; i < nGaussians; i++) {
        //RooRealVar*  dm    = new RooRealVar(Form("%s_dm_mh%d_g%d",signame.c_str(),mh,i),Form("%s_dm_mh%d_g%d",signame.c_str(),mh,i),0.1,-3,3);
        //RooAbsReal*  mean  = new RooFormulaVar(Form("%s_mean_mh%d_g%d",signame.c_str(),mh,i),Form("%s_mean_mh%d_g%d",signame.c_str(),mh,i),"@0+@1",RooArgList(*MH,*dm));
        RooRealVar*  mean  = new RooRealVar(Form("%s_mean_mh%d_g%d",signame.c_str(),mh,i),Form("%s_mean_mh%d_g%d",signame.c_str(),mh,i), 124.5, 120., 127);//121 129
        RooRealVar*  sigma = new RooRealVar(Form("%s_sigma_mh%d_g%d",signame.c_str(),mh,i),Form("%s_sigma_mh%d_g%d",signame.c_str(),mh,i), 2, 0.4, 25.);  //2 0.5 25
        RooGaussian* gaus  = new RooGaussian(Form("%s_gaus_mh%d_g%d",signame.c_str(),mh,i),Form("%s_gaus_mh%d_g%d",signame.c_str(),mh,i),*mass,*mean,*sigma);
        gaussians->add(*gaus);

        if (i < nGaussians - 1){
            RooRealVar*  frac  = new RooRealVar(Form("%s_frac_mh%d_g%d",signame.c_str(),mh,i),Form("%s_frac_mh%d_g%d",signame.c_str(),mh,i), 0.12, 0.01, 0.99);//0.1 0.02 0.85
            coeffs->add(*frac);
        }
    }
    RooAddPdf* tmpMultiGaussians = new RooAddPdf(Form("PDF_%s%d_gaus%d",signame.c_str(),mh,nGaussians),Form("PDF_%s%d_gaus%d",signame.c_str(),mh,nGaussians),*gaussians,*coeffs, true);
    return tmpMultiGaussians;
}

RooAbsPdf* MultiCrystalBall (string signame, RooRealVar* mass, int nCrystalBall, int mh) {

    RooArgList* crystalball = new RooArgList();
    RooArgList* coeffs = new RooArgList();
    //RooRealVar* MH = new RooRealVar("MH","MH",(double)mh,115.,135.);
    //MH->setConstant(true);

    for (int i = 0; i < nCrystalBall; i++) {
        //RooRealVar*  dm    = new RooRealVar(Form("%s_dm_mh%d_g%d",signame.c_str(),mh,i),Form("%s_dm_mh%d_g%d",signame.c_str(),mh,i),0.1,-3,3);
        //RooAbsReal*  mean  = new RooFormulaVar(Form("%s_mean_mh%d_g%d",signame.c_str(),mh,i),Form("%s_mean_mh%d_g%d",signame.c_str(),mh,i),"@0+@1",RooArgList(*MH,*dm));
        RooRealVar*  mean  = new RooRealVar(Form("%s_mean_mh%d_g%d",signame.c_str(),mh,i),Form("%s_mean_mh%d_g%d",signame.c_str(),mh,i), 125, 121., 129.);//121 129
        RooRealVar*  sigma = new RooRealVar(Form("%s_sigma_mh%d_g%d",signame.c_str(),mh,i),Form("%s_sigma_mh%d_g%d",signame.c_str(),mh,i), 2, 0.4, 20.);  //2 0.5 25
        RooRealVar*  alpha = new RooRealVar(Form("%s_alpha_mh%d_g%d",signame.c_str(),mh,i),Form("%s_alpha_mh%d_g%d",signame.c_str(),mh,i), 2, 0, 5.);  //2 0.5 25
        RooRealVar*  n = new RooRealVar(Form("%s_n_mh%d_g%d",signame.c_str(),mh,i),Form("%s_n_mh%d_g%d",signame.c_str(),mh,i), 1, 0., 10.);  //2 0.5 25
        RooCBShape* cb  = new RooCBShape(Form("%s_cb_mh%d_g%d",signame.c_str(),mh,i),Form("%s_cb_mh%d_g%d",signame.c_str(),mh,i),*mass,*mean,*sigma,*alpha, *n);
        crystalball->add(*cb);

        if (i < nCrystalBall - 1){
            RooRealVar*  frac  = new RooRealVar(Form("%s_frac_mh%d_g%d",signame.c_str(),mh,i),Form("%s_frac_mh%d_g%d",signame.c_str(),mh,i), 0.1, 0.01, 0.99);//0.1 0.02 0.85
            coeffs->add(*frac);
        }
    }
    RooAddPdf* tmpMultiCrystalBall = new RooAddPdf(Form("PDF_%s%d_gaus%d",signame.c_str(),mh,nCrystalBall),Form("PDF_%s%d_gaus%d",signame.c_str(),mh,nCrystalBall),*crystalball,*coeffs, true);
    return tmpMultiCrystalBall;
}

























