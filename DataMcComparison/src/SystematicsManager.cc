#include "vbf_analysis/DataMcComparison/interface/SystematicsManager.h"

#include <iostream>
#include <cmath>
using namespace std;

SystematicsManager::SystematicsManager( string plotname ):
    plotname_( plotname )
{
}

SystematicsManager::~SystematicsManager()
{
}

void SystematicsManager::SetMcCollection( vector<string> Collection )
{
    for (auto& it : Collection) {
        size_t dot = it.find_last_of(".");
        Collection_.emplace_back(it.erase(dot,5));
    }
}

void SystematicsManager::AddSystLabel( string SystLabel )
{
    SystLabels_.emplace_back(SystLabel);
}

void SystematicsManager::ErrorPropagator( double scale )
{
    TH1Service<TH1D> th1service;
    th1service.AddPlotFromFile("htest",plotname_, Collection_[0]+".root" );
    const int nbin = th1service.GetPlot("htest")->GetNbinsX();
    double* UpError2 = new double[nbin];  
    double* DownError2 = new double[nbin];
    for (int i = 0; i < nbin; i++) {UpError2[i]=0;DownError2[i]=0;}

    for ( const auto& it : SystLabels_ ) {
        for ( const auto& its : Collection_ ) {
            th1service.AddPlotFromFile("hnormal",plotname_, its+".root" );
            th1service.AddPlotFromFile("hup",plotname_, its+"_"+it+"Up01sigma.root" );
            th1service.AddPlotFromFile("hdown",plotname_, its+"_"+it+"Down01sigma.root" );
            th1service.GetPlot("hnormal")->SetScaleWeight(scale);
            th1service.GetPlot("hup")->SetScaleWeight(scale);
            th1service.GetPlot("hdown")->SetScaleWeight(scale);

            auto normal = th1service.GetPlot("hnormal")->GetBinContent();
            auto up   = th1service.GetPlot("hup")  ->GetBinContent();
            auto down = th1service.GetPlot("hdown")->GetBinContent();

            if ( normal.size() != up.size() && normal.size() != down.size() ) {
                cout << "[ERROR] : Up Down Normal Nbin must be the same!!" <<endl;
                exit (1);
            }
            for (unsigned int i = 0; i< normal.size(); i++) {
                if (up[i].first-normal[i].first >= 0.) UpError2[i] += pow( up[i].first-normal[i].first, 2.0 );
                if (normal[i].first-down[i].first >= 0.) DownError2[i] += pow( normal[i].first-down[i].first, 2.0 );
            }
            th1service.Delete("hnormal"); 
            th1service.Delete("hup"); 
            th1service.Delete("hdown"); 
        }
    }
    for( int i = 0; i<nbin; i++ ) SystError.emplace_back( make_pair( sqrt(UpError2[i]), sqrt(DownError2[i]) ) );

    delete[] UpError2;
    delete[] DownError2;
    th1service.Close();
}

