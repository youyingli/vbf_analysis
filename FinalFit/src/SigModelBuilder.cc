#include "vbf_analysis/FinalFit/interface/SigModelBuilder.h"
#include "vbf_analysis/FinalFit/interface/SigPdfSource.h"
#include "vbf_analysis/Utils/interface/PlotLibrary.h"

#include "TMath.h"
#include "RooPlot.h"
#include "RooFitResult.h"

using namespace std;
using namespace RooFit;

SigModelBuilder::SigModelBuilder (RooRealVar* mass, RooDataSet* data, string samplename, string outdir):
    mass_(mass),
    data_(data),
    samplename_(samplename),
    outdir_(outdir)
{
}

SigModelBuilder::~SigModelBuilder ()
{
}

void SigModelBuilder::FTestSelector ()
{

    int pre_order = 0;
    int best_order = 0;
    double thisNll = 0;
    double preNll = 0;
    double chi2 = 0;
    double prob = 0;
    int status = 0;

    for (int order = 1; order < 7; order++) {
 
        RooAbsPdf* SigCandPdf = MultiGaussians(samplename_, mass_, order, MH);
        RooFitResult* fitres = SigCandPdf->fitTo(*data_,Minimizer("Minuit2","minimize"),Save(true),Verbose(false),SumW2Error(true),Range(115.,135.));
    
        thisNll = fitres->minNll();
        chi2 = 2.*(preNll - thisNll);
        if(order > 1 && chi2 < 0) chi2 = 0;
        int delta_dof = (2 * order + (order - 1)) - ( 2 * pre_order + (pre_order - 1));
        prob = TMath::Prob(chi2, delta_dof);
        status = fitres->status();
        printf("............ This order = %d, Pre Order = %d ,p-value = %1.2e, status = %d \n",order,pre_order,prob,status);
        if(prob > 0.05) break;
        SigPdfSet.push_back(SigCandPdf);
        pre_order = order;
        preNll = thisNll;
    }
    
    best_order = pre_order;
    cout << "----> Best Order is " << best_order << "(number of gaussians)." << endl;
}

//Chi2 test
void SigModelBuilder::AlterFTestSelector ()
{

    RooPlot* plot = mass_->frame();
    data_->plotOn(plot);

    int pre_order = 0;
    int best_order = 0;
    double chi2 = 0;
    double prob = 0;
    int status = 0;

    for (int order = 1; order < 7; order++) {
    
        RooAbsPdf* SigCandPdf = MultiGaussians(samplename_, mass_, order, MH);
        RooFitResult* fitres = SigCandPdf->fitTo(*data_,Minimizer("Minuit2","minimize"),Save(true),Verbose(false),SumW2Error(true),Range(115.,135.));
        SigCandPdf->plotOn(plot);
    
        chi2 = plot->chiSquare();
        prob = TMath::Prob(chi2, 2 * order + (order - 1));
        status = fitres->status();
        printf("............ This order = %d, Pre Order = %d ,p-value = %1.2e, status = %d \n",order,pre_order,prob,status);
        if(prob > 0.05) break;
        SigPdfSet.push_back(SigCandPdf);
        pre_order = order;
    }
    
    best_order = pre_order;
    cout << "----> Best Order is " << best_order << "(number of gaussians)." << endl;
}

void SigModelBuilder::MakeFTestPlot ()
{
    

}

RooExtendPdf* SigModelBuilder::GetBestOfFitPdf ()
{
    RooRealVar* norm = new RooRealVar("norm","",data_->sumEntries(),-1000.,10000000.);
    norm->setConstant(true);
    RooExtendPdf* extsigpdf = new RooExtendPdf(Form("%s_Extend", samplename_.c_str()), "", *(SigPdfSet.back()), *norm);
    
    return extsigpdf;
}



