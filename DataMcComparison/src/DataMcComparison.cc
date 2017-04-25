#include <cassert>
#include <iostream>
#include "TFile.h"
#include "vbf_analysis/DataMcComparison/interface/DataMcComparison.h"
#include "vbf_analysis/DataMcComparison/interface/LatexDraw.h"

using namespace std;

DataMcComparison::DataMcComparison(string type, string plotName, string outDir):
    type_(type),
    plotName_(plotName),
    outDir_(outDir)
{
    system(Form("mkdir -p %s",outDir_.c_str()));
    stackPlot_ = new THStack("hs","");
    legend = ARTKIT::newLegend();
    legend-> SetNColumns(2);
    isSignal_ = false;
    isBkg_    = false;
    isData_   = false;
    doSysts_  = false;
}

DataMcComparison::~DataMcComparison()
{
    delete stackPlot_;
    stackPlot_ = NULL;
    th1service.Close();
}

void DataMcComparison::SetData(string fileName)
{
    th1service.AddPlotFromFile("hData", plotName_, fileName);
    th1service.GetPlot("hData")-> ConvertToPointPlot(kBlack,20,0.8);
    legend->AddEntry( th1service.GetPlot("hData")->GetObject(), "data", "PL" );
    isData_ = true;
}

void DataMcComparison::SetSignal(string fileName, string SigName, Color_t color, Style_t style, double shift_factor)
{
    th1service.AddPlotFromFile( SigName, plotName_, fileName );
    //th1service.GetPlot( SigName )-> SetScaleWeight( shift_factor );
    th1service.GetPlot( SigName )-> SetScaleWeight( shift_factor*Lumi_ );
    th1service.GetPlot( SigName )-> ConvertToLinePlot( color, style );
    if ( shift_factor<5 ) legend->AddEntry( th1service.GetPlot( SigName )->GetObject(), SigName.c_str(), "L" );
    else legend->AddEntry( th1service.GetPlot( SigName )->GetObject(), Form("%s x%.0f",SigName.c_str(),shift_factor ), "L" );
    signalCollection_.emplace_back( th1service.GetPlot( SigName ) );
    isSignal_ = true;

    cout << SigName << " : " << th1service.GetPlot( SigName )->GetEventN() << endl;
}

void DataMcComparison::SetBkgComp(vector<string> fileNames, string BkgName, Color_t color ,Style_t style, double scale_weight)
{
    scale_weight_ = scale_weight; 
    bool flag = false;
    int i = 0;
    for ( const auto& it : fileNames ) {
        if (flag) {
            th1service.AddPlotFromFile( BkgName + to_string(i), plotName_, it );
            th1service.GetPlot( BkgName )->AddPlot( th1service.GetPlot( BkgName + to_string(i) ) ->GetObject() );
            th1service.AddPlotFromFile( BkgName+"tmp" + to_string(i), plotName_, it );
            backgroundCollection_.emplace_back( th1service.GetPlot(BkgName+"tmp" +to_string(i)) );
            i++;
        } else {
            th1service.AddPlotFromFile( BkgName, plotName_, it ); flag = true; i++;
            th1service.AddPlotFromFile( BkgName + "tmp", plotName_, it );
            backgroundCollection_.emplace_back( th1service.GetPlot(BkgName+"tmp") );
        }
    }
    //th1service.GetPlot( BkgName )->SetScaleWeight( scale_weight );
    th1service.GetPlot( BkgName )->SetScaleWeight( scale_weight*Lumi_ );
    th1service.GetPlot( BkgName )->ConvertToFillPlot( color, style );
    stackPlot_->Add( th1service.GetPlot(BkgName)->GetObject() );
    legend -> AddEntry( th1service.GetPlot(BkgName)->GetObject(), BkgName.c_str(), "F");
    isBkg_ = true;
}

void DataMcComparison::SetLogScale(bool isLog)
{
    isLog_ = isLog;
}

void DataMcComparison::DrawDriven(string xtitle)
{
    assert( isBkg_ && isData_ );

    auto XRange = th1service.GetPlot("hData")->GetXRange();
    nbin_ = th1service.GetPlot("hData")->GetNbinsX();
    th1service.AddNewTH1("hBkg", nbin_, XRange.first, XRange.second);
    for (const auto& it : backgroundCollection_) {
        th1service.GetPlot("hBkg")->AddPlot(it->GetObject()); 
    }
    //th1service.GetPlot("hBkg")->SetScaleWeight(scale_weight_);
    th1service.GetPlot("hBkg")->SetScaleWeight(scale_weight_*Lumi_);

    ARTKIT::SetGlobalStyle();
// Add MC Uncertainties (Stat , Syst)    
//________________________________________________________________________________________________________________
    GetTotalMCError(doSysts_);
    th1service.AddNewTH1("ErrorPlot",nbin_,XRange.first,XRange.second);
    th1service.AddNewTH1("ErrorPlotr",nbin_,XRange.first,XRange.second);
    th1service.GetPlot("ErrorPlot")->SetBinContent(TotalContent_);
    th1service.GetPlot("ErrorPlotr")->SetBinContent(TotalrContent_);
    th1service.GetPlot("ErrorPlot")->ConvertToBoxErrorPlot(kGray+2,0.55);
    th1service.GetPlot("ErrorPlotr")->ConvertToBoxErrorPlot(kGray+2,0.55);
    if(doSysts_)legend -> AddEntry(th1service.GetPlot("ErrorPlot")->GetObject(),"Stat. #oplus Syst.","F");
    else legend -> AddEntry(th1service.GetPlot("ErrorPlot")->GetObject(),"Stat. Uncert.","F");
//_________________________________________________________________________________________________________________
//
    canv_ = ARTKIT::signalCanvas(); canv_->cd();

    //Top Plot
    TPad* topPad = ARTKIT::TopPad(); topPad->Draw(); topPad->cd();
    stackPlot_->Draw("histo");
    th1service.GetPlot("ErrorPlot")->Draw("E2 same");
    if ( isSignal_ ) 
        for ( const auto& it : signalCollection_ ) it -> Draw("histosame");
    th1service.GetPlot("hData")->Draw("E1 X0 same");

    stackPlot_ -> GetXaxis()->SetLabelSize(0.);
    stackPlot_ -> GetYaxis()->SetTitle(Form("Events/(%.2f)",stackPlot_->GetXaxis()->GetBinWidth(1)));
    stackPlot_ -> GetHistogram() -> SetTitleFont(42,"xyz");
    stackPlot_ -> GetHistogram() -> SetLabelFont(42,"xyz");
    stackPlot_ -> GetHistogram() -> SetLabelSize(0.04,"xyz");
    stackPlot_ -> GetHistogram() -> SetTitleSize(0.04,"xyz");

    if ( isLog_ ) {
       topPad->SetLogy();
       stackPlot_ -> SetMinimum(0.01);
       stackPlot_ -> SetMaximum( 800 * th1service.GetPlot("hData")->GetMaxContent() );
    }else {
       stackPlot_ -> SetMinimum(0.);
       stackPlot_ -> SetMaximum( 1.6 * th1service.GetPlot("hData")->GetMaxContent() );
    }
    
    legend->Draw();
    AddLatexContent(Lumi_, type_);

    canv_->Update();
    
    //Bottom Plot
    canv_->cd();
    TPad* bottomPad = ARTKIT::BottomPad(); bottomPad->Draw(); bottomPad->cd();
    TH1D* comparePlot = ARTKIT::ratioPlot(th1service.GetPlot("hData")->GetObject(), 
                                          th1service.GetPlot("hBkg")->GetObject(), 
                                          Form("%s",xtitle.c_str()), "Data/MC" );
    comparePlot -> Draw("E1 X0");
    th1service.GetPlot("ErrorPlotr")->Draw("E2 same");
    TLine* horizonline =  ARTKIT::horizontalLine( comparePlot,1,kBlue,7,3 );
    horizonline -> Draw();
    comparePlot -> Draw("E1 X0 same");
    
    canv_->Update();
    end();
}


void DataMcComparison::end()
{
    system(Form("mkdir -p %s",outDir_.c_str()));
    canv_->Print(Form("%s/%s.pdf",outDir_.c_str(),plotName_.c_str()));
    canv_->Close();
}

void DataMcComparison::SetLumi( double Lumi )
{
    Lumi_ = Lumi;
}

void DataMcComparison::SetSystError( PairVector SystError, bool doSysts )
{
    SystError_ = SystError;
    doSysts_ = doSysts;
}

void DataMcComparison::GetTotalMCError(bool AddSysts)
{
    auto BkgContent = th1service.GetPlot("hBkg")->GetBinContent();

    for ( int i=0; i<nbin_; i++ ) {
        if (BkgContent[i].first == 0.) {
            TotalContent_.emplace_back( make_pair(0.,0.) );
            TotalrContent_.emplace_back( make_pair(0.,0.) );
        } else if ( AddSysts ) {
            double totalUp = sqrt( pow(BkgContent[i].second,2.0) + pow(SystError_[i].first,2.0) );
            double totalDown = sqrt( pow(BkgContent[i].second,2.0) + pow(SystError_[i].second,2.0) );
            double UpValue = totalUp + BkgContent[i].first;
            double DownValue =  BkgContent[i].first - totalDown;
            TotalContent_ .emplace_back( make_pair( (UpValue+DownValue)/2., (UpValue-DownValue)/2. ) );
            TotalrContent_.emplace_back( make_pair( (UpValue+DownValue)/(2.*BkgContent[i].first), (UpValue-DownValue)/(2.*BkgContent[i].first) ) );
        } else {
            TotalContent_ .emplace_back( make_pair( BkgContent[i].first, BkgContent[i].second ) );
            TotalrContent_.emplace_back( make_pair( 1., BkgContent[i].second/BkgContent[i].first) );
        }
    }//i++
}

