
#include "vbf_analysis/FinalFit/interface/EnvelopeBuilder.h"
#include "vbf_analysis/Utils/interface/PlotLibrary.h"

#include <iostream>

#include "TMath.h"

#include "RooExtendPdf.h"
#include "RooPlot.h"
#include "RooMinimizer.h"
#include "RooHist.h"

#include "TGraphAsymmErrors.h"


using namespace std;
using namespace RooFit;


EnvelopeBuilder::EnvelopeBuilder (RooRealVar* var, RooDataSet* data, vector<RooAbsPdf*> multipdf, string outdir):
    var_(var),
    data_(data),
    multipdf_(multipdf),
    outdir_(outdir)
{
}

EnvelopeBuilder::~EnvelopeBuilder ()
{
}

double EnvelopeBuilder::GetNormNLL (double norm, double varlow, double varhigh) {

    double bestFitNll=1.e9;

    for (const auto& it : multipdf_) {
        var_->setRange("errRange",varlow,varhigh);
        RooRealVar normVar("norm","",0.,1.e6);
        RooExtendPdf extPdf("extPdf", "", *it, normVar, "errRange");
        RooAbsReal* nll = extPdf.createNLL(*data_,Extended());
    
        normVar.setConstant(false);
        normVar.setVal(norm);
        normVar.setConstant(true);

        RooMinimizer minim(*nll);
        minim.setStrategy(0);
        minim.migrad();
        double corrNll = 2 * (nll->getVal()) + it->getParameters(*data_)->getSize();
        if(corrNll<bestFitNll) bestFitNll = corrNll;
    }
    return bestFitNll;
}

double EnvelopeBuilder::GetAsymmetryError (double bestPoint ,double nllbest, double boundary, double varlow, double varhigh, double diff) {

    bool isDownSide;
    double UpEdge, DownEdge;
    double value = 0; 
    double valueNll = 0;
    if (boundary < bestPoint) {
       isDownSide = true;
       UpEdge = bestPoint;
       DownEdge = boundary;
    } else {
       isDownSide = false;
       UpEdge = boundary;
       DownEdge = bestPoint;
    }

    double distancetotrue = 1.e6;
    int nits=0;

    while (TMath::Abs(distancetotrue/diff) > 0.05) {

        value = (UpEdge-DownEdge)/2 + DownEdge;
        valueNll = GetNormNLL(value,varlow,varhigh) - nllbest;
        distancetotrue = diff - valueNll;

        if (isDownSide) {
            if (valueNll > diff) DownEdge = value;
            else UpEdge = value;
        } else {
            if (valueNll > diff) UpEdge = value;
            else DownEdge = value;
        }
        nits++;

        if (nits > 20) {
            return value;
            DownEdge = TMath::Max(0., DownEdge-20.);
            UpEdge += 20.;
            nits = 0;
            if(TMath::Abs(valueNll) > 2.e4) return 0;
        }
    }
    return value;
}

void EnvelopeBuilder::BuilderCore (bool isblind, bool combine) {

    ARTKIT::SetGlobalStyle();
     
    RooPlot* plot = var_->frame();
    plot->GetXaxis()->SetLabelSize(0.);
    plot->GetYaxis()->SetTitleSize(0.04);
    plot->GetYaxis()->SetLabelSize(0.04);
    data_->plotOn(plot,Binning(80),Invisible());
    RooHist* plotdata = (RooHist*)plot->getObject(plot->numItems()-1);
    TObject* dataleg = (TObject*)plot->getObject(plot->numItems()-1);
//    if(combine) bestpdf_->plotOn(plot,LineColor(kRed),LineWidth(3),LineStyle(7));
//    else bestpdf_->plotOn(plot,LineColor(kRed),LineWidth(3));     
    bestpdf_->plotOn(plot,LineColor(kRed),LineWidth(3));     
    RooCurve* bestPdfcurve = (RooCurve*)plot->getObject(plot->numItems()-1);

    TLegend* leg = ARTKIT::newLegend();
    leg -> SetNColumns(2);
    leg->AddEntry(dataleg,"data","LEP");
    leg->AddEntry(bestPdfcurve,"Bkg fit","L");

    TGraphAsymmErrors* oneSigmaband = new TGraphAsymmErrors();   //SetName
    TGraphAsymmErrors* twoSigmaband = new TGraphAsymmErrors();
    TGraphAsymmErrors* oneSigmaband_r = new TGraphAsymmErrors();
    TGraphAsymmErrors* twoSigmaband_r = new TGraphAsymmErrors();

    int p=0;
    for (double MH = 100.; MH < 180.5; MH += 1.) {

        double lowedge = MH-0.5;
        double highedge = MH+0.5;
        double center = MH;
        double normbkg = bestPdfcurve->interpolate(center);
        double bestnll = GetNormNLL(normbkg, lowedge, highedge);
        double lowRange = TMath::Max(0., normbkg - 3 * TMath::Sqrt(normbkg));
        double highRange = normbkg + 3 * TMath::Sqrt(normbkg);

        double errdown1Value = GetAsymmetryError(normbkg, bestnll, lowRange, lowedge, highedge, 1);
        double errdown2Value = GetAsymmetryError(normbkg, bestnll, lowRange, lowedge, highedge, 4);
        double errUp1Value = GetAsymmetryError(normbkg, bestnll, highRange, lowedge, highedge, 1);
        double errUp2Value = GetAsymmetryError(normbkg, bestnll, highRange, lowedge, highedge, 4);

        double errdown1 = normbkg - errdown1Value;
        double errdown2 = normbkg - errdown2Value;
        double errUp1 = errUp1Value - normbkg;
        double errUp2 = errUp2Value - normbkg;

        oneSigmaband->SetPoint(p, center, normbkg);
        twoSigmaband->SetPoint(p, center, normbkg);
        oneSigmaband->SetPointError(p,0.,0.,errdown1,errUp1);
        twoSigmaband->SetPointError(p,0.,0.,errdown2,errUp2);

        oneSigmaband_r->SetPoint(p,center,0.);
        twoSigmaband_r->SetPoint(p,center,0.);
        oneSigmaband_r->SetPointError(p,0.,0.,errdown1,errUp1);
        twoSigmaband_r->SetPointError(p,0.,0.,errdown2,errUp2);

        cout << " step = " << p << ", MH = " << MH <<endl;
        p++;
    }

    TCanvas canv("canv","",600,700);
    canv.cd();
    TPad* toppad = ARTKIT::TopPad();
    toppad->Draw();
    toppad->cd();
    plot->Draw();

    twoSigmaband->SetLineColor(kYellow);
    twoSigmaband->SetFillColor(kYellow);
    twoSigmaband->SetMarkerColor(kYellow);
    twoSigmaband->Draw("L3 same");
    oneSigmaband->SetLineColor(kGreen);
    oneSigmaband->SetFillColor(kGreen);
    oneSigmaband->SetMarkerColor(kGreen);
    oneSigmaband->Draw("L3 same");
    leg->AddEntry(oneSigmaband,"#pm1#sigma","F");
    leg->AddEntry(twoSigmaband,"#pm2#sigma","F");

    twoSigmaband_r->SetLineColor(kYellow);
    twoSigmaband_r->SetFillColor(kYellow);
    twoSigmaband_r->SetMarkerColor(kYellow);
    oneSigmaband_r->SetLineColor(kGreen);
    oneSigmaband_r->SetFillColor(kGreen);
    oneSigmaband_r->SetMarkerColor(kGreen);

//    RooPlot*splot = var_-> frame();
/*
    if(addsignal){
       sigPDF["ggf"]->plotOn(plot,Normalization(1.0,RooAbsReal::RelativeExpected),LineColor(kBlue),LineWidth(1));
       RooCurve* ggfCurve = (RooCurve*)plot->getObject(plot->numItems()-1);
       sigPDF["ggf"]->plotOn(plot,Normalization(1.0,RooAbsReal::RelativeExpected),LineColor(kBlue),LineWidth(1),FillColor(38),FillStyle(3001),DrawOption("F"));
       TObject* ggfleg = (TObject*)plot->getObject(plot->numItems()-1);
       sigPDF["vbf"]->plotOn(plot,Normalization(1.0,RooAbsReal::RelativeExpected),LineColor(kRed),LineWidth(1));
       RooCurve* vbfCurve = (RooCurve*)plot->getObject(plot->numItems()-1);
       sigPDF["vbf"]->plotOn(plot,Normalization(1.0,RooAbsReal::RelativeExpected),LineColor(kRed),LineWidth(1),FillColor(46),FillStyle(3001),DrawOption("F"));
       TObject* vbfleg = (TObject*)plot->getObject(plot->numItems()-1);
       leg.AddEntry(vbfleg,"vbf(M_{H} = 126GeV)","F");
       leg.AddEntry(ggfleg,"ggf(M_{H} = 126GeV)","F");
       RooRealVar fvbf("fvbf","",vbfCurve->average(100.,180.)*80.,0.,1000.); fvbf.setConstant(true);
       RooRealVar fggf("fggf","",ggfCurve->average(100.,180.)*80.,0.,1000.); fggf.setConstant(true);
       RooAddPdf sigTotalPdf("total","",RooArgList(*sigPDF["ggf"],*sigPDF["vbf"]),RooArgList(fggf,fvbf));
       sigTotalPdf.plotOn(plot,Normalization(1.0,RooAbsReal::RelativeExpected),LineColor(kViolet+2),LineWidth(4),LineStyle(2));
       TObject* totalleg = (TObject*)plot->getObject(plot->numItems()-1);
       leg.AddEntry(totalleg,"vbf+ggf(M_{H} = 126GeV}","L");
       if(combine){
           sigTotalPdf.plotOn(splot,Normalization(1.0,RooAbsReal::RelativeExpected),LineColor(kRed),LineWidth(3),LineStyle(1));
           RooRealVar fsig("fsig","",fvbf.getValV()+fggf.getValV(),0.,100000.); fsig.setConstant(true);
           RooRealVar fbkg("fbkg","",bestPdfcurve->average(100.,180.)*80.,0.,100000.); fbkg.setConstant(true);
           RooAddPdf TotalPDF("TotalPDF","",RooArgList(*bestpdf_,sigTotalPdf),RooArgList(fbkg,fsig));
           TotalPDF.plotOn(plot,Normalization(1.0,RooAbsReal::RelativeExpected),LineColor(kRed),LineWidth(3),LineStyle(1));
       }
    }
*/
    if (isblind) {
       var_->setRange("unblind_low",100.,115.);
       var_->setRange("unblind_high",135.,180.);
       data_->plotOn(plot,Binning(80),CutRange("unblind_low,unblind_high"),MarkerSize(0.9),XErrorSize(0));
    } else {
       data_->plotOn(plot,Binning(80),MarkerSize(0.9));
    }

    plot->Draw("same");
    leg->Draw("same");

    ARTKIT::SetCMSandLUMI(35.87);

    canv.Update();
    canv.cd();
    TPad* bottompad = ARTKIT::BottomPad();
    bottompad->Draw();
    bottompad->cd();

    int npoint = plotdata->GetN();
    double xtmp,ytmp;

    TGraphAsymmErrors *hdatasub = new TGraphAsymmErrors(npoint);

    for (int i=0; i < npoint; ++i) {

        plotdata->GetPoint(i,xtmp,ytmp);
        double bkgvalue = bestPdfcurve->interpolate(xtmp);
        if (isblind) {
            if(xtmp>115.&&xtmp<135.) continue;
        }
        double errorUp = plotdata -> GetErrorYhigh(i);
        double errorDown = plotdata -> GetErrorYlow(i);
        hdatasub->SetPoint(i,xtmp,ytmp-bkgvalue);
        hdatasub->SetPointError(i,0.,0.,errorDown,errorUp);

    }

    TH1 *hdata = new TH1D("hdata","",80,100,180);
    hdata->SetMaximum(hdatasub->GetHistogram()->GetMaximum()+1);
    hdata->SetMinimum(hdatasub->GetHistogram()->GetMinimum()-1);
    //hdata->GetYaxis()->SetTitle("data - best fit PDF");
    //hdata->GetYaxis()->SetTitleSize(0.12);
    hdata->GetYaxis()->SetLabelSize(0.11);
    hdata->GetXaxis()->SetTitle("M_{#gamma#gamma} (GeV)");
    hdata->GetXaxis()->SetTitleSize(0.11);
    hdata->GetXaxis()->SetLabelSize(0.11);
    hdata->Draw("HISTO");
    twoSigmaband_r->Draw("L3 SAME");
    oneSigmaband_r->Draw("L3 SAME");
    hdata->GetYaxis()->SetNdivisions(905);

    TLatex* latex2 = new TLatex();
    latex2 -> SetNDC();
    latex2 -> SetTextFont(42);
    latex2 -> SetTextSize(0.13);
    latex2 -> DrawText(0.7,0.86,"Bkg subtracted");
         
    TLine *line3 = new TLine(100,0.,180,0.);
    line3->SetLineColor(kRed);
    line3->SetLineWidth(3);
    line3->Draw();
    /*
    if (combine) {
        line3->SetLineStyle(7);
        splot->Draw("SAME");
    }*/
    hdatasub->Draw("PESAME");
    hdatasub->SetMarkerStyle(20);
    hdatasub->SetMarkerSize(0.9);

    canv.Update();
    canv.Print(Form("%s/envelopepdf.pdf",outdir_.c_str()));

}

void EnvelopeBuilder::SetBestPdf (RooAbsPdf* bestpdf)
{
    bestpdf->fitTo(*data_);
    bestpdf_ = bestpdf;
}

void EnvelopeBuilder::SetRangeAndRes (double resolution, double min, double max)
{
    resolution_ = resolution;
    min_ = min;
    max_ = max;
}
