#include <cmath>

#include "vbf_analysis/FinalFit/interface/BkgPdfSource.h"

#include "RooArgList.h"
#include "RooAbsReal.h"
#include "RooFormulaVar.h"
#include "RooBernstein.h"
#include "RooAddPdf.h"
#include "RooExponential.h"
#include "RooGenericPdf.h"

using namespace std;
using namespace RooFit;

RooAbsPdf* EnvelopePdf (RooRealVar* var, string pdfType, int order) {

    RooAbsPdf* chosenpdf = NULL;

    if (pdfType == "Bernstein") {
        RooRealVar* param;
        RooArgList* coeffs = new RooArgList();
        for (int i=0; i <= order; i++) {
            param = new RooRealVar(Form("param_%d",i), Form("param_%d",i), 0.1*(i+1), 0., 25.);
            coeffs->add(*param);
        }
        RooBernstein* BernPdf = new RooBernstein(Form("%s_%d",pdfType.c_str(),order), Form("%s_%d",pdfType.c_str(),order), *var, *coeffs);
        chosenpdf = BernPdf;
    }


    if (pdfType == "Laurent") {
        int nlower = int(ceil(order/2.));
        int nhigher = order - nlower;
        RooArgList* coeffs = new RooArgList();
        RooArgList* LaurPowList = new RooArgList();
        //first 0th order
        RooGenericPdf* LaurPow0Pdf = new RooGenericPdf("LaurPow_0", "LaurPow_0", "TMath::Power(@0,-4.)", RooArgList(*var));
        LaurPowList->add(*LaurPow0Pdf);

        RooRealVar* efrac;
        RooRealVar* ofrac;
        //even terms 
        for (int i=1; i<=nlower; i++ ) {
            RooGenericPdf* LaurPowePdf = new RooGenericPdf(Form("LaurPow_e%d",i), Form("LaurPow_e%d",i), Form("TMath::Power(@0,-4.-%d)",i), RooArgList(*var));
            efrac = new RooRealVar(Form("efrac_%d",i), Form("efrac_%d",i), 0.25/order, 0.000001, 0.8);
            LaurPowList->add(*LaurPowePdf);
            coeffs->add(*efrac);
        }
        //odd terms
        for (int i=1;i<=nhigher;i++) {
            RooGenericPdf* LaurPowoPdf = new RooGenericPdf(Form("LaurPow_o%d",i), Form("LaurPow_o%d",i), Form("TMath::Power(@0,-4.+%d)",i), RooArgList(*var));
            ofrac = new RooRealVar(Form("ofrac_%d",i), Form("ofrac_%d",i), 0.25/order, 0.000001, 0.99999);
            LaurPowList->add(*LaurPowoPdf);
            coeffs->add(*ofrac);
        }
        RooAddPdf* LaurPdf = new RooAddPdf(Form("%s_%d",pdfType.c_str(),order), Form("%s_%d",pdfType.c_str(),order), *LaurPowList, *coeffs, true);

        chosenpdf = LaurPdf;
    }

    if (pdfType == "PowerLaw") {

        if (order%2 != 0) {
            return NULL;
        } else {
           
            int nterm = order/2;
            string formula = "";
            int num = 1;
            RooArgList* params = new RooArgList();
            params->add(*var);
            RooRealVar* coeff;
            RooRealVar* expo;
            for (int i = 1; i <= nterm; i++) {
                coeff = new RooRealVar(Form("coeff_%d",i), Form("coeff_%d",i), 0.9-float(i-1)*1./nterm, 0., 1.);
                expo = new RooRealVar(Form("expo_%d",i), Form("expo_%d",i), TMath::Max(-9.,-1.*(i+1)), -9., 4.5);
                if (i == 1) formula += Form("@%d*pow(@0,@%d)", num, num+1);
                else formula += Form("+@%d*pow(@0,@%d)", num, num+1);
                params->add(*coeff);
                params->add(*expo);
                num += 2;
            }
            RooGenericPdf* PowLow2Pdf = new RooGenericPdf(Form("%s_%d", pdfType.c_str(), order), Form("%s_%d", pdfType.c_str(), order), formula.c_str(), *params);
            chosenpdf = PowLow2Pdf;
        }
    }

    if (pdfType == "Exponential") {
                                                                                                                                                            
        if (order%2 != 0) {
            return NULL;
        } else {
               
            int nterm = order/2;
            string formula = "";
            int num = 1;
            RooArgList* params = new RooArgList();
            params->add(*var);
            RooRealVar* coeff;
            RooRealVar* expo;
            for (int i = 1; i <= nterm; i++) {
                coeff = new RooRealVar(Form("coeff_%d",i), Form("coeff_%d",i), 0.9-float(i-1)*1./nterm, 0.3, 1.);
                expo = new RooRealVar(Form("expo_%d",i), Form("expo_%d",i), TMath::Max(-1.,-0.04*(i+1)), -1., 0.);
                if (i == 1) formula += Form("@%d*exp(@%d*@0)", num, num+1);
                else formula += Form("+@%d*exp(@%d*@0)", num, num+1);
                params->add(*coeff);
                params->add(*expo);
                num += 2;
            }
            RooGenericPdf* Exp2Pdf = new RooGenericPdf(Form("%s_%d",pdfType.c_str(),order),Form("%s_%d",pdfType.c_str(),order),formula.c_str(),*params);
            chosenpdf = Exp2Pdf;
        }
    }
    //------------------------
    if (pdfType == "Exponential2"){
        if (order%2 == 0){
            return NULL;
        } else {
            int nfrac = (order-1)/2;
            int npow = order - nfrac;
            RooArgList* coeffs = new RooArgList();
            RooArgList* ExpList = new RooArgList();
            for (int i = 1;i <= npow; i++){
                RooRealVar* expo = new RooRealVar(Form("expo_%d",i),Form("expo_%d",i),TMath::Max(-1.,-0.04*(i+1)),-1.,0.);
                RooExponential* exp = new RooExponential(Form("exp_%d",i),Form("exp_%d",i),*var,*expo);
                ExpList->add(*exp);
            }
            for (int i = 1; i <= nfrac; i++){
                RooRealVar* frac = new RooRealVar(Form("frac_%d",i),Form("frac_%d",i),0.9-float(i-1)*1./nfrac,0.,1.);
                coeffs->add(*frac);
            }
            RooAddPdf* ExpPdf = new RooAddPdf(Form("%s_%d",pdfType.c_str(),order),Form("%s_%d",pdfType.c_str(),order),*ExpList,*coeffs,true);
            //RooAddPdf* ExpPdf = new RooAddPdf(Form("%s_%d",pdfType.c_str(),order),Form("%s_%d",pdfType.c_str(),order),*ExpList,*coeffs);
          
            chosenpdf = ExpPdf;
        }
    }

    if (pdfType == "PowerLaw2"){
        if(order%2 == 0){
            return NULL;
        } else {
                                                                                                                                           
            int nfrac = (order-1)/2;
            int npow = order - nfrac;
            RooArgList* coeffs = new RooArgList();
            RooArgList* PowList = new RooArgList();
            for (int i = 1; i <= npow; i++) {
                RooRealVar* expo = new RooRealVar(Form("expo_%d",i),Form("expo_%d",i),TMath::Max(-9.,-1.*(i+1)),-9.,10.);
                RooGenericPdf* PowPdf = new RooGenericPdf(Form("Pow_%d",i),Form("Pow_%d",i),"TMath::Power(@0,@1)",RooArgList(*var,*expo));
                PowList->add(*PowPdf);
            }
            for (int i = 1; i <= nfrac; i++) {
                RooRealVar* frac = new RooRealVar(Form("frac_%d",i),Form("frac_%d",i),0.9-float(i-1)*1./nfrac,0.,1.);
                coeffs->add(*frac);
            }
                                                                                                                                           
            RooAddPdf* PowLowPdf = new RooAddPdf(Form("%s_%d",pdfType.c_str(),order),Form("%s_%d",pdfType.c_str(),order),*PowList,*coeffs,true);
            //RooAddPdf* PowLowPdf = new RooAddPdf(Form("%s_%d",pdfType.c_str(),order),Form("%s_%d",pdfType.c_str(),order),*PowList,*coeffs);
                                                                                                                                           
            chosenpdf = PowLowPdf;
        }
    }
    return chosenpdf;

}

