#include <cassert>
#include <iostream>
#include "TFile.h"
#include "vbf_analysis/DataMcComparison/interface/DataMcComparison.h"

using namespace std;

DataMcComparison::DataMcComparison( string plotName, string outDir ):
    plotName_(plotName),
    outDir_(outDir)
{
    system(Form("mkdir -p %s",outDir_.c_str()));
    stackPlot_ = new THStack("hs","");
    hBkg_ = new TH1D();
    legend = ARTKIT::newLegend();
    legend-> SetNColumns(2);
    gflag_    = false;
    isSignal_ = false;
    isBkg_    = false;
    isData_   = false;
    doSysts_  = false;
}

DataMcComparison::~DataMcComparison()
{
    delete stackPlot_;
    delete hBkg_;
}

void DataMcComparison::SetData( string fileName )
{
    TFile* file = TFile::Open( fileName.c_str() );
    TH1D* hdata = (TH1D*)file->Get( plotName_.c_str() );
    hData_ = ARTKIT::newH1PointPlot( hdata );
    legend->AddEntry( hData_, "data", "PL");
    isData_ = true;
}

void DataMcComparison::SetSignal( string fileName, string leg, Color_t color, Style_t style, double shift_factor )
{
    TFile* file = TFile::Open( fileName.c_str() );
    TH1D* hsignal = (TH1D*)file->Get( plotName_.c_str() );
    hsignal->Scale( shift_factor );
    signalCollection_.emplace_back( ARTKIT::newH1LinePlot( hsignal, color, style ) );
    //if(shift_factor == 1)legend->AddEntry( ARTKIT::newH1LinePlot( hsignal, color, style ), leg.c_str(), "L" );
    //else legend->AddEntry( ARTKIT::newH1LinePlot( hsignal, color, style ), Form("%s x%.0f",leg.c_str(),shift_factor ), "L" );
    legend->AddEntry( ARTKIT::newH1LinePlot( hsignal, color, style ), leg.c_str(), "L" );
    isSignal_ = true;
}

void DataMcComparison::SetBkgComp( vector<string> fileNames, string leg, Color_t color ,Style_t style, double scale_weight )
{
    scale_weight_ = scale_weight; 
    
    TH1D* hplot = new TH1D();   
    bool flag = false;
    for( const auto& it : fileNames ){
        TFile* file = TFile::Open( it.c_str() );
        if(flag){
           hplot->Add( (TH1D*)file->Get( plotName_.c_str()) );
        }else{
           hplot = (TH1D*)file->Get( plotName_.c_str() ); flag = true;
        }
        if(gflag_){ 
           hBkg_->Add( (TH1D*)file->Get( plotName_.c_str()) );  
        }else{
           hBkg_ = (TH1D*)file->Get( plotName_.c_str() );  gflag_ = true;
        }
    }
    hplot->Scale( scale_weight );
    stackPlot_->Add( ARTKIT::newH1FillPlot(hplot, color, style) );
    legend->AddEntry( ARTKIT::newH1FillPlot(hplot, color, style), leg.c_str(), "F");
    isBkg_ = true;
}

void DataMcComparison::SetLogScale( bool isLog )
{
    isLog_ = isLog;
}

void DataMcComparison::DrawDriven( string xtitle )
{
    assert( isBkg_ && isData_ );

// Add MC Uncertainties (Stat , Syst)    
//________________________________________________________________________________________________________________
    GetTotalMCError(doSysts_, scale_weight_);
    TH1D* ErrorPlot  = new TH1D("ErrorPlot","", nbin_-1, hData_->GetXaxis()-> GetXmin(),hData_->GetXaxis() -> GetXmax());
    TH1D* ErrorPlotr = new TH1D("ErrorPlotr","", nbin_-1, hData_->GetXaxis()-> GetXmin(),hData_->GetXaxis() -> GetXmax());
    for(int i=1;i<nbin_;i++){
        ErrorPlot ->SetBinContent(i,CenterValue_[i-1]); 
        ErrorPlot ->SetBinError(i,TotalError_[i-1]);
        ErrorPlotr->SetBinContent(i,CenterrValue_[i-1]); 
        ErrorPlotr->SetBinError(i,TotalrError_[i-1]);
    }
    ErrorPlot ->SetLineColor(kBlack);
    ErrorPlot ->SetLineWidth(2);
    ErrorPlot ->SetFillColorAlpha(kGray+2, 0.45);
    ErrorPlotr->SetFillColorAlpha(kGray+2, 0.45);
    ErrorPlot ->SetMarkerColorAlpha(kGray+2, 0.);
    ErrorPlotr->SetMarkerColorAlpha(kGray+2, 0.);
    if(doSysts_)legend -> AddEntry(ErrorPlot,"Stat. #oplus Syst.","F");
    else legend -> AddEntry(ErrorPlot,"Stat. Uncert.","F");
//_________________________________________________________________________________________________________________

    ARTKIT::SetGlobalStyle();
    canv_ = ARTKIT::signalCanvas();
    canv_->cd();
    TPad* topPad = ARTKIT::TopPad();
    topPad->Draw();
    topPad->cd();
    stackPlot_->Draw("histo");
    ErrorPlot ->Draw("E2same");
    hData_    ->Draw("E1 X0 same");
    double binM = hData_ -> GetBinWidth(1);
    stackPlot_ -> GetXaxis()->SetLabelSize(0.);
    stackPlot_ -> GetYaxis()->SetTitle(Form("Events/(%.2f)",binM));
    stackPlot_ -> GetHistogram() -> SetTitleFont(42,"xyz");
    stackPlot_ -> GetHistogram() -> SetLabelFont(42,"xyz");
    stackPlot_ -> GetHistogram() -> SetLabelSize(0.04,"xyz");
    stackPlot_ -> GetHistogram() -> SetTitleSize(0.04,"xyz");

    if ( isLog_ ) {
       topPad->SetLogy();
       stackPlot_ -> SetMinimum(0.01);
       stackPlot_ -> SetMaximum( 800 * hData_->GetMaximum() );
    }else {
       stackPlot_ -> SetMinimum(0.);
       stackPlot_ -> SetMaximum( 1.6 * hData_->GetMaximum() );
    }
    const int nsig = signalCollection_.size();
    TH1D* splot[nsig];
    if ( isSignal_ ) {
       int i = 0;
       for ( const auto& it : signalCollection_ ) {
           splot[i] = it;
           splot[i] -> Draw("histosame");
           i++;
       }
    }

    legend->Draw();
    ARTKIT::SetCMSandLUMI( Lumi_ );
    /*TLatex lex;
    lex.SetNDC(true);
    lex.SetTextFont(42);
    lex.SetTextSize(0.03);
    lex.DrawLatex(0.17,0.875,"Pt_{#gamma1}/M_{#gamma#gamma}>1/4, Pt_{#gamma2}/M_{#gamma#gamma}>1/5");
    lex.DrawLatex(0.17,0.830,"Pt_{jet1}>30GeV, Pt_{jet2}>20GeV");
    lex.DrawLatex(0.17,0.785,"M_{jj}>100GeV");*/
/*
    TLatex lex;
    lex.SetNDC(true);
    lex.SetTextFont(62);
    lex.SetTextSize(0.04);
    lex.DrawLatex(0.17,0.875,"VBFTag");
    lex.SetTextSize(0.03);
    lex.SetTextFont(42);
    lex.DrawLatex(0.17,0.830,"100 <  M_{#gamma#gamma} < 180 GeV");
    lex.DrawLatex(0.17,0.790,"dijet BDT > 0.4");
    lex.DrawLatex(0.17,0.755,"JEC,JER");
    lex.DrawLatex(0.17,0.720,"Systematics included");
*/
    TLatex lex;
    lex.SetNDC(true);
    lex.SetTextFont(62);
    lex.SetTextSize(0.04);
    lex.DrawLatex(0.17,0.875,"ZPlusJets");
    lex.SetTextSize(0.03);
    lex.SetTextFont(42);
    lex.DrawLatex(0.17,0.830,"70 <  M_{ee} < 110 GeV");
    lex.DrawLatex(0.17,0.790,"dijet BDT > 0.4");
    lex.DrawLatex(0.17,0.755,"JEC,JER");
    lex.DrawLatex(0.17,0.720,"Systematics included");

    canv_->Update();

    canv_->cd();
    TPad* bottomPad = ARTKIT::BottomPad();
    bottomPad->Draw();
    bottomPad->cd();
    TH1D* comparePlot = ARTKIT::ratioPlot( hData_, hBkg_, Form("%s",xtitle.c_str()), "Data/MC" );
    comparePlot -> Draw("E1 X0");
    ErrorPlotr -> Draw("E2 same");

    TLine* horizonline =  ARTKIT::horizontalLine( comparePlot,1,kBlue,7,3 );
    horizonline -> Draw();
    comparePlot -> Draw("E1 X0 same");
    
    canv_->Update();

    end();

    delete ErrorPlot;
    delete ErrorPlotr;
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

void DataMcComparison::SetSystError( std::vector<double> Up, std::vector<double> Down, bool doSysts )
{
    if ( Up.size() != Down.size() ) {
        cout << "[ERROR] Bin number must be the same" << endl;
        exit(0);
    }
    doSysts_ = doSysts;
    UpError_ = Up;
    DownError_ = Down;
    doSysts_ = doSysts;
}

void DataMcComparison::GetTotalMCError(bool AddSysts, double scale){

    nbin_ = hBkg_->GetNbinsX() + 1;
    for ( int i=1; i<nbin_; i++ ) {
        double stats = hBkg_ -> GetBinError(i);
        double binContent = hBkg_ -> GetBinContent(i);
        if (binContent == 0.) {
            TotalError_.emplace_back(0.); 
            CenterValue_.emplace_back(0.); 
            TotalrError_.emplace_back(0.);
            CenterrValue_.emplace_back(0.);
        } else if( AddSysts ) {
            double totalUp = sqrt( pow(stats,2.0) + pow(UpError_[i-1],2.0) );
            double totalDown = sqrt( pow(stats,2.0) + pow(DownError_[i-1],2.0) );
            double UpValue = totalUp + binContent;
            double DownValue =  binContent - totalDown;
            TotalError_.emplace_back( (UpValue-DownValue)/2. );
            CenterValue_.emplace_back( (UpValue+DownValue)/2. );
            TotalrError_.emplace_back( (UpValue-DownValue)/(2.*binContent) );
            CenterrValue_.emplace_back( (UpValue+DownValue)/(2.*binContent) );
        } else {
            TotalError_.emplace_back(stats); 
            TotalrError_.emplace_back(stats/binContent);
            CenterValue_.emplace_back(binContent);
            CenterrValue_.emplace_back(1.);            
        }
    }//i++
}

