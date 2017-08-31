#include "vbf_analysis/DataMcComparison/interface/SystematicsManager.h"

#include <iostream>
#include <cmath>
using namespace std;

SystematicsManager::SystematicsManager(const string& plotname):
    _plotname(plotname)
{
}

SystematicsManager::~SystematicsManager()
{
}

void SystematicsManager::SetMcCollection(vector<string> collection)
{
    for (auto& it : collection) {
        size_t dot = it.find_last_of(".");
        _collection.emplace_back(it.erase(dot, 5));
    }
}

void SystematicsManager::AddSystLabel(const string& systlabel)
{
    _systlabels.emplace_back(systlabel);
}

void SystematicsManager::ErrorPropagator(double scale)
{
    TH1Service<TH1F> th1service;
    th1service.AddPlotFromFile("htest", _plotname, _collection[0] + ".root");
    const int nbin = th1service.GetPlot("htest")->GetNbinsX();
    unique_ptr<double[]> uperror2(new double[nbin]);
    unique_ptr<double[]> downerror2(new double[nbin]);

    for (int i = 0; i < nbin; i++) { uperror2[i] = 0; downerror2[i] = 0;}
    for (const auto& it : _systlabels) {
        for (const auto& its : _collection) {
            th1service.AddPlotFromFile("hnormal", _plotname, its + ".root");
            th1service.AddPlotFromFile("hup", _plotname, its + "_" +it + "Up01sigma.root");
            th1service.AddPlotFromFile("hdown", _plotname, its + "_" + it + "Down01sigma.root");
            th1service.GetPlot("hnormal")->SetScaleWeight(scale);
            th1service.GetPlot("hup")->SetScaleWeight(scale);
            th1service.GetPlot("hdown")->SetScaleWeight(scale);

            auto normal = th1service.GetPlot("hnormal")->GetBinContent();
            auto up   = th1service.GetPlot("hup")->GetBinContent();
            auto down = th1service.GetPlot("hdown")->GetBinContent();

            if (normal.size() != up.size() && normal.size() != down.size()) {
                cout << "[ERROR] : Up Down Normal Nbin must be the same!!" <<endl;
                exit (1);
            }
            for (unsigned int i = 0; i < normal.size(); i++) {
                if (up[i].first - normal[i].first >= 0.) uperror2[i] += pow(up[i].first - normal[i].first, 2.0);
                if (normal[i].first - down[i].first >= 0.) downerror2[i] += pow(normal[i].first - down[i].first, 2.0);
            }
            th1service.Delete("hnormal"); 
            th1service.Delete("hup"); 
            th1service.Delete("hdown"); 
        }
    }
    for(int i = 0; i < nbin; i++) _systerror.emplace_back(make_pair(sqrt(uperror2[i]), sqrt(downerror2[i])));

    th1service.Close();
}

