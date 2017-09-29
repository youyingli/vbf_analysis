#include "vbf_analysis/FinalFit/interface/BkgModelBuilder.h"
#include "vbf_analysis/FinalFit/interface/BkgPdfSource.h"
#include "vbf_analysis/FinalFit/interface/EnvelopeBuilder.h"
#include "vbf_analysis/FinalFit/interface/FitManager.h"
#include "vbf_analysis/Utils/interface/PlotMgrLib.h"

#include "TMath.h"
#include "RooExtendPdf.h"
#include "RooPlot.h"
#include "RooFitResult.h"

#include <iomanip>

using namespace std;
using namespace RooFit;

BkgModelBuilder::BkgModelBuilder (RooRealVar* mass, RooDataSet* data, string outdir):
    _mass(mass),
    _data(data),
    _outdir(outdir)
{
    _hasSignal = false;
}

BkgModelBuilder::~BkgModelBuilder ()
{
}

void BkgModelBuilder::MakeEnvelopePlot (bool isblind, bool iscombined)
{
    vector<RooAbsPdf*> multipdf;
    for (const auto& it : _finalbkgcand) {
        multipdf.emplace_back(EnvelopePdf(_mass, it.first, it.second));
    }
    EnvelopeBuilder envelopeBuilder (_mass, _data, multipdf, _outdir);
    envelopeBuilder.SetBestPdf (EnvelopePdf(_mass, _bestpdf, _bestorder));
    if (_hasSignal) envelopeBuilder.SetSignalPdf(_exsigpdf);
    envelopeBuilder.BuilderCore (isblind, iscombined);
}

void BkgModelBuilder::FTestSelector (int last_order = 7)
{
    map<string, int>firstresult = FirstFTest (last_order);
    FinalFTest (firstresult);
}

double BkgModelBuilder::GetGoodnessOfFit (RooAbsPdf* testPdf)
{
    int nBinsForMass = 160;
    RooRealVar norm("norm", "norm", _data->sumEntries(), 0, 10e6);
    RooExtendPdf* pdf = new RooExtendPdf("ext", "ext", *testPdf, norm);

    RooPlot *plot_chi2 = _mass->frame();
    _data->plotOn(plot_chi2, Binning(nBinsForMass), Name("data"));
    pdf->plotOn(plot_chi2, Name("pdf"));
    int np = pdf->getParameters(*_data)->getSize();
    double chi2 = plot_chi2->chiSquare("pdf", "data", np);
    double prob = TMath::Prob(chi2 * (nBinsForMass - np), nBinsForMass-np);
    delete pdf;
    return prob;
}

map<string, int> BkgModelBuilder::FirstFTest (int last_order)
{
    map<string, int> pdfcand;
    for (const auto& it : _bkgpdfset) {                                                   
        int order = 1;
        int pre_order = 0;
        int cache_order = 0;
        int highest_order = 0;
        double thisNll = 0.;
        double preNll = 10e8;
        double chi2 = 0.;
        double prob = 0.;
                                                                                                                                                  
        cout <<"[INFO]:"<< it << " function family... " << endl;
        while (order < last_order && prob < 0.05){
            RooAbsPdf* bkgCandPDf = EnvelopePdf(_mass, it, order);
            if (bkgCandPDf == nullptr) {
                order++;
            } else {
                FitManager bkgfitmgr(RooArgSet(*_mass), bkgCandPDf);
                bkgfitmgr.SetDataSetFitted(_data);
                bkgfitmgr.UseUnBinnedFitting(100., 180.);
                thisNll = bkgfitmgr.GetMinNLL();

                chi2 = (order > 1 && (preNll - thisNll) < 0) ? 0 : 2.*(preNll - thisNll);
                int delta_dof = order - pre_order;
                prob = TMath::Prob(chi2,delta_dof);
                 
                printf("............ This order = %d, Pre Order = %d ,p-value = %1.2e \n",order,pre_order,prob);
                cache_order = pre_order;
                pre_order = order;
                preNll = thisNll;
                order++;
            }
        }
        highest_order = cache_order;
        pdfcand.insert(make_pair(it, highest_order));
        cout << "----> The highest order is " << highest_order << endl;
        cout << "-----------------------------------------------------------------------" << endl;
    }
    return pdfcand;
}

void BkgModelBuilder::FinalFTest (map<string, int> pdfcand)
{
    map<TString, TObject*> multipdfleg;
    int color[7] = {kBlue,kRed,kMagenta,kGreen+1,kOrange+7,kAzure+10,kBlack};
    _mass->setRange("RangeLeft",100.,115.);
    _mass->setRange("RangeRight",135.,180.);
                                                                                                                                           
    RooPlot* bframe = _mass->frame(Bins(80));
    _data->plotOn(bframe, MarkerStyle(20), CutRange("RangeLeft,RangeRight"));
    TLegend* newleg = plotmgr::NewLegend();
    TObject *pdfLeg0 = bframe->getObject(int(bframe->numItems()) - 1);
    newleg->AddEntry(pdfLeg0,"Data","LP");

    string bestfit = "";
    int best_order = 0;
    double mincorre = 10e8;
    int legcount = 0;
    for (const auto& it : pdfcand) {
        int order = 1;
        while (order <= it.second) { 
            RooAbsPdf* bkgCandPdf = EnvelopePdf(_mass, it.first, order);                                                                                                       
            if (bkgCandPdf == nullptr) { order++; continue; }
            FitManager bkgfitmgr(RooArgSet(*_mass), bkgCandPdf);
            bkgfitmgr.SetDataSetFitted(_data);
            bkgfitmgr.UseUnBinnedFitting(100., 180.);

            double prob = GetGoodnessOfFit(bkgfitmgr.GetPostPdf());
            if (order != it.second && prob < 0.01) { order++; continue; }
            //cout << it.first<<order <<"  " << prob << endl;

            double correction = 2 * bkgfitmgr.GetMinNLL() + order;
                                                                                                                                           
            if (correction < mincorre){                                                                                                                                       
                mincorre = correction;
                bestfit = it.first;
                best_order = order;                                                                                                                               
            }
                                                                                                                                           
            cout << setw(15) << it.first << order << " ----> 2NLL + lambda(nparams) = " << correction <<endl;
            bkgCandPdf->plotOn(bframe, LineColor(color[legcount]));
            TObject *pdfLeg = bframe->getObject(int(bframe->numItems()) - 1);
            _finalbkgcand.emplace_back(make_pair(it.first, order));
            multipdfleg.insert(make_pair(Form("%s_%d", (it.first).c_str(), order), pdfLeg));
            order++;
            legcount++;
        }
    }
                                                                                                                                           
    _bestpdf = bestfit;
    _bestorder = best_order;
    cout << "----> Best fit : " << bestfit << best_order << endl;
    
    for (const auto& it : multipdfleg) {
        if(it.first == Form("%s_%d",bestfit.c_str(), best_order)) newleg->AddEntry(it.second, it.first + "(best of fit)", "L");
        else newleg->AddEntry(it.second, it.first, "L");
    }
    TCanvas* canv = plotmgr::NewCanvas();
    canv->cd();
    TPad* pad = plotmgr::NewNormalPad();
    pad->Draw(); pad->cd();
    bframe->Draw();
    bframe->SetXTitle("M_{#gamma#gamma} (GeV)");
    newleg->Draw();
    canv->Print(Form("%s/ftest.pdf",_outdir.c_str()));
}

void BkgModelBuilder::SetBkgPdfSource (vector<string> bkgpdf)
{
    _bkgpdfset = bkgpdf;
}

void BkgModelBuilder::SetSignalPdf (map<string, RooExtendPdf*> exsigpdf)
{
    _exsigpdf = exsigpdf;
    _hasSignal = true;
}



