#include "vbf_analysis/FinalFit/interface/SigModelBuilder.h"
#include "vbf_analysis/FinalFit/interface/SigPdfSource.h"
#include "vbf_analysis/FinalFit/interface/FitManager.h"
#include "vbf_analysis/Utils/interface/PlotMgrLib.h"

#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "TMath.h"

using namespace std;
using namespace RooFit;

SigModelBuilder::SigModelBuilder (RooRealVar* mass, RooDataSet* data, const string& samplename, const string& outdir):
    _mass(mass),
    _data(data),
    _samplename(samplename),
    _outdir(outdir)
{
    _unweightdset = new RooDataSet("_unweightdset", "", *_mass);
    for (int i = 0; i < _data->numEntries(); i++) {
        _mass->setVal(_data->get(i)->getRealValue(_mass->GetName()));
        _unweightdset->add(RooArgSet(*_mass));
    }
}

SigModelBuilder::~SigModelBuilder ()
{
}

void SigModelBuilder::FTestSelector ()
{
    int pre_order = 0;
    int best_order = 0;
    double thisNll = 0;
    double preNll = 10e8;
    double chi2 = 0;
    double prob = 0;

    for (int order = 1; order < 5; order++) {
        FitManager sigfitmgr(RooArgSet(*_mass), MultiGaussians(_samplename, _mass, order, MH));
        //sigfitmgr.SetDataSetFitted(_data);
        sigfitmgr.SetDataSetFitted(_unweightdset);
        sigfitmgr.UseUnBinnedFitting(115., 135.);
        thisNll = sigfitmgr.GetMinNLL();
        chi2 = 2.*(preNll - thisNll);
        if(order > 1 && chi2 < 0) chi2 = 0;
        int delta_dof = (2 * order + (order - 1)) - (2 * pre_order + (pre_order - 1));
        prob = TMath::Prob(chi2, delta_dof);
        printf("............ This order = %d, Pre Order = %d ,p-value = %1.2e \n", order, pre_order, prob);

        if(prob > 0.05) break;
        sigfitmgr.FixPdfShape(true);
        _sigpdfset.emplace_back(sigfitmgr.GetPostPdf());
        pre_order = order;
        preNll = thisNll;
    }
    
    best_order = pre_order;
    cout << "----> Best Order is " << best_order << "(number of gaussians)." << endl;
}

//Chi2 test
void SigModelBuilder::AlterFTestSelector ()
{
    int pre_order = 0;
    int best_order = 0;
    double thisNll = 0;
    double preNll = 10e8;
    double chi2 = 0;
    double prob = 0;

    for (int order = 1; order < 5; order++) {

        FitManager sigfitmgr(RooArgSet(*_mass), MultiGaussians(_samplename, _mass, order, MH));
        sigfitmgr.SetDataSetFitted(_data);
        sigfitmgr.Use1DBinnedFitting(80, 115, 135);
        thisNll = sigfitmgr.GetMinNLL();
        chi2 = 2.*(preNll - thisNll);
        if(order > 1 && chi2 < 0) chi2 = 0;   

        int delta_dof = (2 * order + (order - 1)) - (2 * pre_order + (pre_order - 1));
        prob = TMath::Prob(chi2, delta_dof);
        //prob = TMath::Prob(sigfitmgr.GetChiSquare(), 2*order+(order-1));
        cout << TMath::Prob( sigfitmgr.GetChiSquare(), 40 - (2 * order + (order - 1))) << endl;
        printf("............ This order = %d, Pre Order = %d ,p-value = %1.2e \n", order, pre_order, prob);

        if(prob > 0.05) break;
        sigfitmgr.FixPdfShape(true);
        _sigpdfset.emplace_back(sigfitmgr.GetPostPdf());
        pre_order = order;
        preNll = thisNll;
    }
    
    best_order = pre_order;
    cout << "----> Best Order is " << best_order << "(number of gaussians)." << endl;
}

void SigModelBuilder::MakeFTestPlot ()
{

    int color[7] = {kBlue, kRed, kMagenta, kGreen+1, kOrange+7, kAzure+10, kBlack};
    RooPlot* sframe = _mass->frame(Bins(40), Range(115., 135.));
    _data->plotOn(sframe, MarkerStyle(20));
    TObject *pdfLeg0 = sframe->getObject(int(sframe->numItems()) - 1);
    TLegend leg(0.7,0.7,1.,0.92);
    leg.AddEntry(pdfLeg0,Form("%s_125", _samplename.c_str()), "LP");
                                                                                         
    int order = 1;
    for (const auto& it : _sigpdfset){
        it->plotOn(sframe, LineColor(color[order-1]));
        TObject *pdfLeg = sframe->getObject(int(sframe->numItems() - 1));
        leg.AddEntry(pdfLeg,Form("%d order",order), "L");
        order++;
    }
    TCanvas canv("canv","canv",700,600);
    TPad pad("pad","",0.,0.,0.98,0.98);
    canv.cd();
    pad.SetLeftMargin(0.20);
    pad.SetRightMargin(0.03);
    pad.Draw();
    pad.cd();
    sframe->SetXTitle("M_{#gamma#gamma} (GeV)");
    sframe->GetYaxis()->SetTitleOffset(1.35);
    sframe->Draw();
    leg.SetFillStyle(0);
    leg.SetBorderSize(0);
    leg.Draw();
    canv.Print(Form("%s/%s.pdf", _outdir.c_str(), _samplename.c_str()));
}

RooExtendPdf* SigModelBuilder::GetBestOfFitPdf ()
{
    RooRealVar* norm = new RooRealVar("norm", "", _data->sumEntries(), -1000., 10000000.);
    norm->setConstant(true);
    RooExtendPdf* extsigpdf = new RooExtendPdf(Form("%s_Extend", _samplename.c_str()), "", *(_sigpdfset.back()), *norm);
    
    return extsigpdf;
}



