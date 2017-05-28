
#include "vbf_analysis/FinalFit/interface/BkgModelBuilder.h"
#include "vbf_analysis/FinalFit/interface/BkgPdfSource.h"
#include "vbf_analysis/FinalFit/interface/EnvelopeBuilder.h"
#include "vbf_analysis/Utils/interface/PlotLibrary.h"

#include "TMath.h"
#include "RooExtendPdf.h"
#include "RooPlot.h"
#include "RooFitResult.h"


using namespace std;
using namespace RooFit;

BkgModelBuilder::BkgModelBuilder (RooRealVar* mass, RooDataSet* data, string outdir):
    mass_(mass),
    data_(data),
    outdir_(outdir)
{
}

BkgModelBuilder::~BkgModelBuilder ()
{
}

void BkgModelBuilder::MakeEnvelopePlot ()
{
    vector<RooAbsPdf*> multipdf;
    for (const auto& it : finalbkgcand_) {
        multipdf.emplace_back(EnvelopePdf(mass_, it.first, it.second));
    }
    EnvelopeBuilder envelopeBuilder (mass_, data_, multipdf, outdir_);
    envelopeBuilder.SetBestPdf (EnvelopePdf(mass_, bestpdf_, bestorder_));
    envelopeBuilder.BuilderCore ();

}

void BkgModelBuilder::FTestSelector (int last_order = 7)
{
    map<string, int>firstresult = FirstFTest (last_order);
    FinalFTest (firstresult);
}

double BkgModelBuilder::GetGoodnessOfFit (RooAbsPdf* testPdf)
{
    int nBinsForMass = 160;
    RooRealVar norm("norm","norm",data_->sumEntries(),0,10e6);
    RooExtendPdf* pdf = new RooExtendPdf("ext","ext",*testPdf,norm);

    RooPlot *plot_chi2 = mass_->frame();
    data_->plotOn(plot_chi2,Binning(nBinsForMass),Name("data"));
    pdf->plotOn(plot_chi2,Name("pdf"));
    int np = pdf->getParameters(*data_)->getSize();
    double chi2 = plot_chi2->chiSquare("pdf","data",np);
    double prob = TMath::Prob(chi2*(nBinsForMass-np),nBinsForMass-np);

    return prob;
}

map<string, int> BkgModelBuilder::FirstFTest (int last_order)
{
    map<string, int> pdfcand;
    for (const auto& it : bkgpdfset_) {
                                                                                                                                                  
        int order = 1;
        int pre_order = 0;
        int cache_order = 0;
        int highest_order = 0;
        double thisNll = 0.;
        double preNll = 10e8;
        double chi2 = 0.;
        double prob = 0.;
        int status = 0;
                                                                                                                                                  
        cout <<"[INFO]:"<< it << " function family... " << endl;
        while (order < last_order && prob < 0.05){
            RooAbsPdf* bkgCandPDf = EnvelopePdf(mass_, it, order);
        
            if (!bkgCandPDf) {
                order++;
            } else {
                RooFitResult* fitres = bkgCandPDf->fitTo(*data_,Minimizer("Minuit2","minimize"),Save(true),SumW2Error(true));
                thisNll = fitres->minNll();
                chi2 = 2.*(preNll - thisNll);
                if(order > 1 && chi2 < 0 ) chi2 = 0;
                int delta_dof = order - pre_order;
                prob = TMath::Prob(chi2,delta_dof);
                status = fitres ->status();
                 
                printf("............ This order = %d, Pre Order = %d ,p-value = %1.2e, status = %d \n",order,pre_order,prob,status);
                cache_order = pre_order;
                pre_order = order;
                preNll = thisNll;
                order++;
            }
        }
        highest_order = cache_order;
        pdfcand.insert(make_pair(it,highest_order));
        cout << "----> The highest order is " << highest_order << endl;
        cout << "-----------------------------------------------------------------------" << endl;
    }
    return pdfcand;
}

void BkgModelBuilder::FinalFTest (map<string, int> pdfcand)
{

    map<TString, TObject*> multipdfleg;
    int color[7] = {kBlue,kRed,kMagenta,kGreen+1,kOrange+7,kAzure+10,kBlack};
    mass_->setRange("Range1",100.,115.);
    mass_->setRange("Range2",135.,180.);
                                                                                                                                           
    RooPlot* bframe = mass_ -> frame(Title("Mgg"),Bins(80));
    data_ -> plotOn(bframe,MarkerStyle(20),CutRange("Range1,Range2"));
    TLegend leg(0.6,0.7,0.9,0.92);
    TObject *pdfLeg0 = bframe->getObject(int(bframe->numItems())-1);
    leg.AddEntry(pdfLeg0,"Data","LP");

    string bestfit = "";
    int best_order = 0;
    int j = 0;
    double mincorre = 10e8;
    for (const auto& it : pdfcand) {
        int order = 1;
        while (order <= it.second) {
        
            RooAbsPdf* bkgCandPDf = EnvelopePdf(mass_,it.first,order);
                                                                                                                                           
            if (!bkgCandPDf) { order++; continue; }
            RooFitResult* res = bkgCandPDf->fitTo(*data_,Minimizer("Minuit2","minimize"),Save(true),Verbose(false),SumW2Error(true));
            double prob = GetGoodnessOfFit(bkgCandPDf);
            if (order != it.second && prob < 0.01) { order++; continue; }
            cout << it.first<<order <<"  " << prob << endl;
            double correction = 2*res->minNll() + order;
                                                                                                                                           
            if (correction < mincorre){                                                                                                                                       
                mincorre = correction;
                bestfit = it.first;
                best_order = order;                                                                                                                               
            }
                                                                                                                                           
            cout << setw(15) << it.first << order << " ----> 2NLL + lambda(nparams) = " << correction <<endl;
            bkgCandPDf -> plotOn(bframe,LineColor(color[j]));
            TObject *pdfLeg = bframe->getObject(int(bframe->numItems())-1);
            
            finalbkgcand_.push_back(make_pair(it.first, order));
            multipdfleg.insert(make_pair(Form("%s_%d",(it.first).c_str(),order),pdfLeg));
            order++;
            j++;
        }
    }
                                                                                                                                           
    bestpdf_ = bestfit;
    bestorder_ = best_order;
    cout << "----> Best fit : " << bestfit << best_order << endl;
    
    for (const auto& it : multipdfleg) {
        if(it.first == Form("%s_%d",bestfit.c_str(),best_order) ) leg.AddEntry(it.second,it.first + "(best of fit)","L");
        else leg.AddEntry(it.second,it.first,"L");
    }
    TCanvas canv("canv","",700,600);
    canv.cd();
    bframe->Draw();
    bframe->SetXTitle("M_{#gamma#gamma} (GeV)");
    leg.SetFillStyle(0);
    leg.SetBorderSize(0);
    leg.Draw();
    canv.Print(Form("%s/ftest.pdf",outdir_.c_str()));
}

void BkgModelBuilder::SetBkgPdfSource (vector<string> bkgpdf)
{
    bkgpdfset_ = bkgpdf;
}



