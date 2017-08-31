#include "vbf_analysis/DataMcComparison/interface/DataMcComparison.h"
#include "vbf_analysis/DataMcComparison/interface/LatexDraw.h"

#include "TFile.h"

#include <iostream>
#include <cassert>

using namespace std;

DataMcComparison::DataMcComparison(const string& type, const string& plotname, const string& outdir):
    _type(type),
    _plotname(plotname),
    _outdir(outdir)
{
    system(Form("mkdir -p %s", _outdir.c_str()));
    _stackplot = new THStack("hs", "");
    _legend = plotmgr::NewLegend();
    _legend->SetNColumns(2);
    _issignal = false;
    _isbkg    = false;
    _isdata   = false;
    _dosysts  = false;
}

DataMcComparison::~DataMcComparison()
{
    delete _stackplot;
    _stackplot = NULL;
    _th1service.Close();
}

void DataMcComparison::SetData (const string& filename)
{
    _th1service.AddPlotFromFile("hData", _plotname, filename);
    _th1service.GetPlot("hData")->ConvertToPointPlot(kBlack, 20, 0.8);
    _legend->AddEntry(_th1service.GetPlot("hData")->GetSnapShot(), "data", "PL");
    _isdata = true;
}

void DataMcComparison::SetSignal (const string& filename, const string& signame, Color_t color, Style_t style, double shift_factor, bool showfactor)
{
    _th1service.AddPlotFromFile(signame, _plotname, filename);
    _th1service.GetPlot(signame)->SetScaleWeight(shift_factor * _lumi);
    _th1service.GetPlot(signame)->ConvertToLinePlot(color, style);
    string legstr = showfactor ? (signame + " x " + to_string(shift_factor)) : signame;
    _legend->AddEntry(_th1service.GetPlot(signame)->GetSnapShot(), legstr.c_str(), "L");
    _signalcollection.emplace_back(_th1service.GetPlot(signame));
    _issignal = true;

    cout << signame << " (expected) : " << _th1service.GetPlot(signame)->GetEventN() / shift_factor << endl;
}

void DataMcComparison::SetBkgComp (const vector<string>& filenames, const string& bkgname, Color_t color ,Style_t style, double scale_factor, bool showfactor)
{
    bool ismulti = false;
    int count = 0;
    for (const auto& it : filenames) {
        if (ismulti) {
            _th1service.AddPlotFromFile(bkgname + to_string(count), _plotname, it);
            _th1service.GetPlot(bkgname)->AddPlot(_th1service.GetPlot(bkgname + to_string(count))->GetSnapShot());
        } else {
            _th1service.AddPlotFromFile(bkgname, _plotname, it);
            ismulti = true;
        }
        count++;
    }
    _th1service.GetPlot(bkgname)->SetScaleWeight(scale_factor * _lumi);
    _backgroundcollection.emplace_back(_th1service.GetPlot(bkgname)->GetSnapShot());
    _th1service.GetPlot(bkgname)->ConvertToFillPlot(color, style);
    _stackplot->Add(_th1service.GetPlot(bkgname)->GetSnapShot());
    string legstr = showfactor ? (bkgname + " x " + to_string(scale_factor)) : bkgname;
    _legend->AddEntry(_th1service.GetPlot(bkgname)->GetSnapShot(), legstr.c_str(), "F");
    _isbkg = true;
}

void DataMcComparison::SetLogScale (bool islog)
{
    _islog = islog;
}

void DataMcComparison::DrawDriven (const string& xtitle, const string& unit)
{
    assert(_isbkg && _isdata);

    auto xrange = _th1service.GetPlot("hData")->GetXRange();
    _nbin = _th1service.GetPlot("hData")->GetNbinsX();
    _th1service.AddNewTH1("hBkg", _nbin, xrange.first, xrange.second);
    for (const auto& it : _backgroundcollection) {
        _th1service.GetPlot("hBkg")->AddPlot(it); 
    }

    plotmgr::SetGlobalStyle();

    // Add MC Uncertainties (Stat , Syst)    
    //------------------------------------------------------------------------------------------------------------------
    GetTotalMCError(_dosysts);
    _th1service.AddNewTH1("ErrorPlot", _nbin, xrange.first, xrange.second);
    _th1service.AddNewTH1("ErrorPlotr", _nbin, xrange.first, xrange.second);
    _th1service.GetPlot("ErrorPlot")->SetBinContent(_totalcontent);
    _th1service.GetPlot("ErrorPlot")->ConvertToBoxErrorPlot(kGray+2, 0.55);
    _th1service.GetPlot("ErrorPlotr")->SetBinContent(_totalrcontent);
    _th1service.GetPlot("ErrorPlotr")->ConvertToBoxErrorPlot(kGray+2, 0.55);
    string legstr = _dosysts ? "Stat. #oplus Syst." : "Stat. Uncert.";
    _legend->AddEntry(_th1service.GetPlot("ErrorPlot")->GetSnapShot(), legstr.c_str(), "F");
    //------------------------------------------------------------------------------------------------------------------

    //Cancas
    _canv = plotmgr::NewCanvas(); _canv->cd();

    //Top Pad
    //------------------------------------------------------------------------------------------------------------------
    TPad* toppad = plotmgr::NewTopPad(); toppad->Draw(); toppad->cd();
    _stackplot->Draw("histo");
    _th1service.GetPlot("ErrorPlot")->Draw("E2 same");
    if (_issignal) 
        for (const auto& it : _signalcollection) it->Draw("histosame");
    _th1service.GetPlot("hData")->Draw("E1 X0 same");

    _stackplot->GetXaxis()->SetLabelSize(0.);
    _stackplot->GetYaxis()->SetTitle(Form("Events/(%.2f %s)", _stackplot->GetXaxis()->GetBinWidth(1), unit.c_str()));
    _stackplot->GetHistogram()->SetTitleFont(42,"xyz");
    _stackplot->GetHistogram()->SetLabelFont(42,"xyz");
    _stackplot->GetHistogram()->SetLabelSize(0.04,"xyz");
    _stackplot->GetHistogram()->SetTitleSize(0.04,"xyz");

    if (_islog) {
       toppad->SetLogy();
       _stackplot->SetMinimum(0.01);
       _stackplot->SetMaximum(800 * _th1service.GetPlot("hData")->GetMaxContent());
    } else {
       _stackplot->SetMinimum(0.);
       _stackplot->SetMaximum(1.6 * _th1service.GetPlot("hData")->GetMaxContent());
    }
    
    _legend->Draw();
    AddLatexContent(_lumi, _type);
    //------------------------------------------------------------------------------------------------------------------

    _canv->Update();

    //Bottom Plot
    //------------------------------------------------------------------------------------------------------------------
    _canv->cd();
    TPad* bottomPad = plotmgr::NewBottomPad(); bottomPad->Draw(); bottomPad->cd();
    TH1F* comparePlot = plotmgr::RatioPlot(_th1service.GetPlot("hData")->GetSnapShot(), 
                                          _th1service.GetPlot("hBkg")->GetSnapShot(), 
                                          Form("%s",xtitle.c_str()), "Data/MC");
    comparePlot -> Draw("E1 X0");
    _th1service.GetPlot("ErrorPlotr")->Draw("E2 same");
    TLine* horizonline =  plotmgr::NewHorizontalLine(comparePlot, 1, kBlue, 7, 3);
    horizonline -> Draw();
    comparePlot -> Draw("E1 X0 same");
    //------------------------------------------------------------------------------------------------------------------

    _canv->Update();
    end();
}


void DataMcComparison::end ()
{
    system(Form("mkdir -p %s",_outdir.c_str()));
    _canv->Print(Form("%s/%s.pdf",_outdir.c_str(),_plotname.c_str()));
    _canv->Close();
}

void DataMcComparison::SetLumi (double Lumi)
{
    _lumi = Lumi;
}

void DataMcComparison::SetSystError (const PairVector& systerror, bool dosysts)
{
    _systerror = systerror;
    _dosysts = dosysts;
}

void DataMcComparison::GetTotalMCError (bool addsysts)
{
    auto bkgcontent = _th1service.GetPlot("hBkg")->GetBinContent();

    for (int i = 0; i < _nbin; i++) {
        if (bkgcontent[i].first == 0.) {
            _totalcontent.emplace_back(make_pair(0.,0.));
            _totalrcontent.emplace_back(make_pair(0.,0.));
        } else if (addsysts) {
            double totalup = sqrt(pow(bkgcontent[i].second, 2.0) + pow(_systerror[i].first, 2.0));
            double totaldown = sqrt(pow(bkgcontent[i].second, 2.0) + pow(_systerror[i].second, 2.0));
            double upvalue = totalup + bkgcontent[i].first;
            double downvalue =  bkgcontent[i].first - totaldown;
            _totalcontent .emplace_back(make_pair((upvalue + downvalue) / 2., (upvalue - downvalue) / 2.));
            _totalrcontent.emplace_back(make_pair((upvalue + downvalue) / (2. * bkgcontent[i].first), (upvalue - downvalue) / (2. * bkgcontent[i].first)));
        } else {
            _totalcontent .emplace_back(make_pair(bkgcontent[i].first, bkgcontent[i].second));
            _totalrcontent.emplace_back(make_pair(1., bkgcontent[i].second / bkgcontent[i].first));
        }
    }//i++
}

