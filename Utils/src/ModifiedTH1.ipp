#ifndef __MODIFIEDTH1_IPP__
#define __MODIFIEDTH1_IPP__

#include "vbf_analysis/Utils/interface/ModifiedTH1.hpp"

#include <iostream>
using namespace std;

template <typename T>
ModifiedTH1<T>::ModifiedTH1 (const string& name, int nbin, double min, double max):
    _plot( new T(name.c_str(), "", nbin, min, max) )
{
    _plot->Sumw2();
}

template <typename T>
ModifiedTH1<T>::ModifiedTH1 (T* plot):
    _plot ( new T(*plot) )
{
}

template <typename T>
ModifiedTH1<T>::~ModifiedTH1 ()
{
    delete _plot;
}

//copy constructor
template <typename T>
ModifiedTH1<T>::ModifiedTH1 (const ModifiedTH1<T>& modifiedTH1)
{
    _plot = new T();
    *_plot = *(modifiedTH1._plot);
}

template <typename T>
void ModifiedTH1<T>::ResetArtStyle ()
{
    _plot->ResetAttLine();  //Default:111
    _plot->ResetAttFill();  //Default:10
    _plot->ResetAttMarker();//Default:111
}

template <typename T>
void ModifiedTH1<T>::ConvertToLinePlot (Color_t color, Style_t style)
{
    ResetArtStyle();
    _plot->SetLineColor(color);
    _plot->SetLineStyle(style);
    _plot->SetLineWidth(3);//Fixed
    SetCommonAxis();
}

template <typename T>
void ModifiedTH1<T>::ConvertToFillPlot (Color_t color, Style_t style)
{
    ResetArtStyle ();
    _plot->SetLineWidth(2);//Fixed
    _plot->SetFillColor(color);
    _plot->SetFillStyle(style);
    SetCommonAxis();
}

template <typename T>
void ModifiedTH1<T>::ConvertToPointPlot (Color_t color, Style_t style, Size_t size)
{
    ResetArtStyle ();
    _plot->SetLineColor(color);
    _plot->SetMarkerColor(color);
    _plot->SetMarkerStyle(style);
    _plot->SetMarkerSize(size);
    _plot->GetYaxis()->SetTitleOffset(1.2);
}

template <typename T>
void ModifiedTH1<T>::ConvertToBoxErrorPlot (Color_t color, double transparency) 
{
    ResetArtStyle();
    _plot->SetLineWidth(2);
    _plot->SetFillStyle(1001);
    _plot->SetFillColorAlpha(color, 1. - transparency);
    _plot->SetMarkerColorAlpha(color, 0.);
}

template <typename T>
void ModifiedTH1<T>::Draw (const string& DrawOption)
{
    _plot->Draw(DrawOption.c_str());
}

template <typename T>
void ModifiedTH1<T>::SetScaleWeight (double scaleweight)
{
    _plot->Scale(scaleweight);
}

template <typename T>
void ModifiedTH1<T>::NormalizeToOne ()
{
    double factor = _plot->Integral();
    if (factor > 0.) _plot->Scale(1./factor);
    else cout << "Have zero or minus entries, please check it" << endl;
}

template <typename T>
void ModifiedTH1<T>::SetXYaxis (const string& xLabel, const string& yLabel, const string& invisible)
{
    _plot->GetXaxis()->SetTitle(xLabel.c_str());
    _plot->GetXaxis()->SetTitle(yLabel.c_str());
    if (invisible == "X") _plot->GetXaxis()->SetLabelSize(0.);
    if (invisible == "Y") _plot->GetYaxis()->SetLabelSize(0.);
    if (invisible == "XY") {
        _plot->GetXaxis()->SetLabelSize(0.);
        _plot->GetYaxis()->SetLabelSize(0.);
    }
}

template <typename T>
double ModifiedTH1<T>::GetWeightEventN (double min, double max) const
{
    int minbin = _plot->FindBin(min);
    int maxbin = _plot->FindBin(max);
    return _plot->Integral(minbin, maxbin);
}

template <typename T>
void ModifiedTH1<T>::FillEvent (double value, double weight)
{
    _plot->Fill(value, weight);
}

template <typename T>
void ModifiedTH1<T>::SetYaxisRange (double ymin, double ymax)
{
    _plot->SetMinimum(ymin);
    _plot->SetMaximum(ymax);
}

template <typename T>
void ModifiedTH1<T>::WriteInFile ()
{
    _plot->Write();
}

template <typename T>
void ModifiedTH1<T>::Reset ()
{
    _plot->Reset();
    ResetArtStyle();
}

//Add
template <typename T>
void ModifiedTH1<T>::AddPlot (T* plot)
{
    _plot->Add(plot);
}

template <typename T>
vector<pair<double,double>> ModifiedTH1<T>::GetBinContent () const
{
    vector<pair<double,double>> contentset;
    for (int i = 0; i < _plot->GetNbinsX(); i++) 
        contentset.emplace_back(make_pair(_plot->GetBinContent(i+1), _plot->GetBinError(i+1)));
    return contentset;
}

template <typename T>
void ModifiedTH1<T>::SetBinContent (const vector<pair<double,double>>& contentset)
{
    if (contentset.size() != (unsigned int)_plot->GetNbinsX()) {
        cout << "[ERROR] : Nbin of input contents doesn't match Nbin of this plot." << endl;
        exit(1);
    }
    int i = 1;
    for (const auto& it : contentset) {
        _plot->SetBinContent(i, it.first);
        _plot->SetBinError(i, it.second);
        i++;
    }
}

template <typename T>
void ModifiedTH1<T>::SetCommonAxis ()
{
    _plot->SetTitleFont(42,"xyz");
    _plot->SetLabelFont(42,"xyz");
    _plot->SetLabelSize(0.04,"xyz");
    _plot->SetTitleSize(0.04,"xyz");
    _plot->GetYaxis()->SetTitleOffset(1.2);
}


#endif
