#include "vbf_analysis/Utils/interface/ScaleFactorCalculator.h"

#include <iostream>
using namespace std;


ScaleFactorCalculator::ScaleFactorCalculator (const string& controlplot):
    _controlplot(controlplot)
{
}

ScaleFactorCalculator::~ScaleFactorCalculator ()
{
    _th1service.Close();
}

void ScaleFactorCalculator::SetRemoveRange (double min, double max)
{
    _RemoveRange.emplace_back(make_pair(min, max));
}

void ScaleFactorCalculator::SetKeepRange (double min, double max)
{
    _KeepRange = make_pair(min, max);
}

void ScaleFactorCalculator::SetMCSamples (const vector<string>& infiles)
{
    int count = 1;
    for (const auto& it_infile : infiles) {
        if (count == 1) {
            _th1service.AddPlotFromFile("TotalMC", _controlplot, it_infile);
        } else {
            _th1service.AddPlotFromFile(to_string(count), _controlplot, it_infile);
            _th1service.GetPlot("TotalMC")->AddPlot(_th1service.GetPlot(to_string(count))->GetSnapShot());
        }
        count++;
    }
}

void ScaleFactorCalculator::SetData(const string& infile)
{
    _th1service.AddPlotFromFile("data", _controlplot, infile);
}

double ScaleFactorCalculator::GetScaleFactor() 
{
    double NrMC = 0;
    double Nrdata = 0;
    if ( !(_RemoveRange.empty()) )
        for (const auto& it : _RemoveRange) {
            NrMC += _th1service.GetPlot("TotalMC")->GetWeightEventN(it.first, it.second);
            Nrdata += _th1service.GetPlot("data")->GetWeightEventN(it.first, it.second);
        }
    double NMC = _th1service.GetPlot("TotalMC")->GetWeightEventN(_KeepRange.first, _KeepRange.second) - NrMC;
    double Ndata = _th1service.GetPlot("data")->GetWeightEventN(_KeepRange.first, _KeepRange.second) - Nrdata;
    double scalefactor = (NMC==0)? 0.: Ndata/NMC;   

    cout << "Scale factor = " << scalefactor/_weight << endl;
    return scalefactor/_weight;
}

void ScaleFactorCalculator::SetExtraWeight(double weight)
{
    _weight = weight;
}
