#ifndef __MODIFIEDTH1_IPP__
#define __MODIFIEDTH1_IPP__

#include "vbf_analysis/Utils/interface/ModifiedTH1.hpp"

#include <iostream>
using namespace std;

template <typename T>
ModifiedTH1<T>::ModifiedTH1 ( string name, int nbin, double min, double max ):
    name_( name ),
    nbin_( nbin ),
    min_( min ),
    max_( max )
{
    plot_ = new T( name_.c_str(), "", nbin_, min_, max );
    plot_ -> Sumw2();
}

template <typename T>
ModifiedTH1<T>::ModifiedTH1 ( T* plot )
{
    nbin_ = plot -> GetNbinsX();
    plot_ = new T(*plot);
}

template <typename T>
ModifiedTH1<T>::~ModifiedTH1 ()
{
    delete plot_;
}

//copy constructor
template <typename T>
ModifiedTH1<T>::ModifiedTH1 ( const ModifiedTH1<T>& modifiedTH1 )
{
    nbin_  = modifiedTH1.nbin_;
    min_   = modifiedTH1.min_;
    max_   = modifiedTH1.max_;
    name_  = modifiedTH1.name_;
    plot_ = new T();
    *plot_ = *(modifiedTH1.plot_);
}

template <typename T>
void ModifiedTH1<T>::ResetArtStyle ()
{
    plot_ -> ResetAttLine ();//111
    plot_ -> ResetAttFill ();//10
    plot_ -> ResetAttMarker ();//111
}

template <typename T>
void ModifiedTH1<T>::ConvertToLinePlot ( Color_t color, Style_t style )
{
    ResetArtStyle ();
    plot_ -> SetLineColor(color);
    plot_ -> SetLineStyle(style);
    plot_ -> SetLineWidth(3);//Fixed
    plot_ -> SetTitleFont(42,"xyz");
    plot_ -> SetLabelFont(42,"xyz");
    plot_ -> SetLabelSize(0.04,"xyz");
    plot_ -> SetTitleSize(0.04,"xyz");
}

template <typename T>
void ModifiedTH1<T>::ConvertToFillPlot ( Color_t color, Style_t style )
{
    ResetArtStyle ();
    plot_ -> SetLineWidth(2);//Fixed
    plot_ -> SetFillColor(color);
    plot_ -> SetFillStyle(style);
    plot_ -> SetTitleFont(42,"xyz");
    plot_ -> SetLabelFont(42,"xyz");
    plot_ -> SetLabelSize(0.04,"xyz");
    plot_ -> SetTitleSize(0.04,"xyz");
}

template <typename T>
void ModifiedTH1<T>::ConvertToPointPlot ( Color_t color, Style_t style, Size_t size )
{
    ResetArtStyle ();
    plot_ -> SetLineColor(color);
    plot_ -> SetMarkerColor(color);
    plot_ -> SetMarkerStyle(style);
    plot_ -> SetMarkerSize(size);
    plot_ -> SetTitleFont(42,"xyz");
    plot_ -> SetLabelFont(42,"xyz");
    plot_ -> SetLabelSize(0.04,"xyz");
    plot_ -> SetTitleSize(0.04,"xyz");
}

template <typename T>
void ModifiedTH1<T>::ConvertToBoxErrorPlot ( Color_t color, double transparency ) 
{
    ResetArtStyle();
    plot_ -> SetLineWidth(2);
    plot_ -> SetFillStyle(1001);
    plot_ -> SetFillColorAlpha( color, 1. - transparency );
    plot_ -> SetMarkerColorAlpha( color, 0. );
}

template <typename T>
void ModifiedTH1<T>::Draw ( string DrawOption )
{
    plot_ -> Draw( DrawOption.c_str() );
}

template <typename T>
void ModifiedTH1<T>::SetScaleWeight ( double scaleweight )
{
    plot_ -> Scale( scaleweight );
}

template <typename T>
void ModifiedTH1<T>::NormalizeToOne ()
{
    double factor = plot_->Integral();
    if (factor > 0.) plot_ -> Scale(1./factor);
    else cout <<"Have zero or minus entries, please check it" << endl;
}

template <typename T>
void ModifiedTH1<T>::SetXYaxis ( string xLabel, string yLabel, string invisible )
{
    plot_ -> GetXaxis() -> SetTitle( xLabel.c_str() );
    plot_ -> GetXaxis() -> SetTitle( yLabel.c_str() );
    if (invisible == "X" ) plot_ -> GetXaxis()->SetLabelSize(0.);
    if (invisible == "Y" ) plot_ -> GetYaxis()->SetLabelSize(0.);
    if (invisible == "XY" ) {
        plot_ -> GetXaxis()->SetLabelSize(0.);
        plot_ -> GetYaxis()->SetLabelSize(0.);
    }
}

template <typename T>
const double ModifiedTH1<T>::GetWeightEventN ( double min, double max )
{
    int minbin = plot_ -> FindBin( min );
    int maxbin = plot_ -> FindBin( max );
    return plot_ -> Integral( minbin, maxbin );
}

template <typename T>
void ModifiedTH1<T>::FillEvent (double value, double weight)
{
    plot_ -> Fill( value, weight );
}

template <typename T>
void ModifiedTH1<T>::SetYaxisRange (double ymin, double ymax)
{
    plot_ -> SetMinimum( ymin );
    plot_ -> SetMaximum( ymax );
}

//Content
template <typename T>
void ModifiedTH1<T>::WriteInFile ()
{
    plot_ -> Write();
}

template <typename T>
void ModifiedTH1<T>::Reset ()
{
    ResetArtStyle();
    plot_ -> Reset();
}

//Add
template <typename T>
void ModifiedTH1<T>::AddPlot (T* plot)
{
    plot_ -> Add( plot );
}

template <typename T>
vector<pair<double,double>> ModifiedTH1<T>::GetBinContent ()
{
    vector<pair<double,double>> contentset;
    for ( int i = 0; i < nbin_; i++ ) 
        contentset.emplace_back( make_pair( plot_->GetBinContent(i+1), plot_->GetBinError(i+1) ) );
    return contentset;
}

template <typename T>
void ModifiedTH1<T>::SetBinContent ( vector<pair<double,double>> contentset )
{
    if (contentset.size()!=nbin_) {
        cout << "[ERROR] : Nbin of input contents doesn't match Nbin of this plot." << endl;
        cout << contentset.size() << "    "<< nbin_ << endl;
        exit(1);
    }
    int i = 0;
    for ( const auto& it : contentset ) {
        plot_->SetBinContent(i+1,it.first);
        plot_->SetBinError(i+1,it.second);
        i++;
    }
}

#endif
