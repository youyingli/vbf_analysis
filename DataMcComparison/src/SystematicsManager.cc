#include <iostream>
#include <cmath>

#include "TFile.h"
#include "TH1D.h"

#include "vbf_analysis/DataMcComparison/interface/SystematicsManager.h"

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
    Collection_ = Collection;
}

void SystematicsManager::AddSystLabel( string SystLabel )
{
    SystLabels_.emplace_back(SystLabel);
}

void SystematicsManager::ErrorPropagator( double scale )
{
    string test_it = Collection_[0];
    TFile* testfile = TFile::Open( (test_it+".root").c_str() );
    TH1D* testplot = (TH1D*)testfile -> Get( plotname_.c_str() );
    int nbin = testplot->GetNbinsX()+1;
    double* UpError2 = new double[nbin];  
    double* DownError2 = new double[nbin];
    for (int i = 1; i < nbin; i++) {UpError2[i]=0;DownError2[i]=0;}

    for ( const auto& it : SystLabels_ ) {
        for ( const auto& its : Collection_ ) {
            TFile* normalFile = TFile::Open( (its+".root").c_str() );
            TFile* UpFile   = TFile::Open( (its+"_"+it+"Up01sigma.root").c_str() );
            TFile* DownFile = TFile::Open( (its+"_"+it+"Down01sigma.root").c_str() );
            TH1D* Normal = (TH1D*)normalFile -> Get( plotname_.c_str() );
            TH1D* Up   = (TH1D*)UpFile -> Get( plotname_.c_str() );
            TH1D* Down = (TH1D*)DownFile -> Get( plotname_.c_str() );
            Normal -> Scale(scale);
            Up -> Scale(scale);
            Down -> Scale(scale);

            for (int i = 1; i<nbin ; i++) {
                double UpError = Up->GetBinContent(i) - Normal->GetBinContent(i);
                double DownError = Normal->GetBinContent(i) - Down->GetBinContent(i);
                if (UpError >= 0.) UpError2[i] += pow( UpError, 2.0 );
                else cout << "[WARNING]Up is lower than normal" << endl;
                if (DownError >= 0.) DownError2[i] += pow( DownError, 2.0 );
                else cout << "[WARNING]Down is higher than normal" << endl;
            }
            normalFile -> Close();
            UpFile -> Close();
            DownFile -> Close();
        }
    }
    for( int i = 1; i<nbin; i++ ) {
        UpError_.emplace_back(sqrt(UpError2[i]));
        DownError_.emplace_back(sqrt(DownError2[i]));
    }
    delete[] UpError2;
    delete[] DownError2;

}

