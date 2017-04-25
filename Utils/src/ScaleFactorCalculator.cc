#include "vbf_analysis/Utils/interface/ScaleFactorCalculator.h"

#include <iostream>
using namespace std;

ScaleFactorCalculator::ScaleFactorCalculator(string controlplot):
    controlplot_(controlplot)
{
}

ScaleFactorCalculator::~ScaleFactorCalculator()
{
    th1service_.Close();
}

void ScaleFactorCalculator::SetRemoveRange(double min, double max)
{
    RemoveRange_.emplace_back(make_pair(min, max));
}

void ScaleFactorCalculator::SetKeepRange(double min, double max)
{
    KeepRange_ = make_pair(min, max);
}

void ScaleFactorCalculator::SetMCSamples(vector<string> infiles)
{
    int i = 1;
    for (const auto& it : infiles) {
        if ( i==1 ) {
            th1service_.AddPlotFromFile("TotalMC",controlplot_,it);
            i++;
        } else {
            th1service_.AddPlotFromFile(to_string(i),controlplot_,it);
            th1service_.GetPlot("TotalMC")->AddPlot(th1service_.GetPlot(to_string(i))->GetObject());
            i++;
        }
    }
}

void ScaleFactorCalculator::SetData(string infile)
{
    th1service_.AddPlotFromFile("data",controlplot_,infile);
}

double ScaleFactorCalculator::GetScaleFactor()
{
    double NrMC = 0;
    double Nrdata = 0;
    if ( !(RemoveRange_.empty()) )
    for (const auto& it : RemoveRange_) {
        NrMC += th1service_.GetPlot("TotalMC")->GetWeightEventN(it.first, it.second);
        Nrdata += th1service_.GetPlot("data")->GetWeightEventN(it.first, it.second);
    }
    double NMC = th1service_.GetPlot("TotalMC")->GetWeightEventN(KeepRange_.first,KeepRange_.second) - NrMC;
    double Ndata = th1service_.GetPlot("data")->GetWeightEventN(KeepRange_.first,KeepRange_.second) - Nrdata;
    double scalefactor = (NMC==0)? 0.: Ndata/NMC;   

    cout << "Scale factor = " << scalefactor/weight_ << endl;
    return scalefactor/weight_;
}

void ScaleFactorCalculator::SetExtraWeight(double weight)
{
    weight_ = weight;
}
