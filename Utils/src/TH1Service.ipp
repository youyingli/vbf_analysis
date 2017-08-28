#ifndef __TH1SERVICE_IPP__
#define __TH1SERVICE_IPP__

#include "vbf_analysis/Utils/interface/TH1Service.hpp"

#include <iostream>
using namespace std;
using namespace plotmgr;

template <typename TH1Type>
TH1Service<TH1Type>::TH1Service ()
{
    _isdelete = true;
}

template <typename TH1Type>
TH1Service<TH1Type>::~TH1Service()
{   
    if (!_isdelete) Close();
}

template <typename TH1Type>
void TH1Service<TH1Type>::AddNewTH1 (const string& name, int nbin, double min, double max)
{
    _th1set.emplace(name, new ModifiedTH1<TH1Type>(name, nbin, min, max)); _isdelete = false;
}

template <typename TH1Type>
void TH1Service<TH1Type>::AddPlotFromFile (const string& name, const string& plotname, const string& filename)
{
    TFile* file = TFile::Open(filename.c_str());
    if (file->IsZombie()) {
        cout << "[ERROR]: " << file << "can not be found !" << endl;
        exit(EXIT_FAILURE);
    }
    TH1Type* plot = (TH1Type*)file->Get(plotname.c_str());
    plot->SetDirectory(0);
    file->Close();
    _th1set.emplace(name, new ModifiedTH1<TH1Type>(plot)); _isdelete = false;
    plot->Delete();
}

template <typename TH1Type>
void TH1Service<TH1Type>::Delete (const string& name) 
{
    delete _th1set[name];
    _th1set[name] = NULL;
    _th1set.erase(name);
}

template <typename TH1Type>
void TH1Service<TH1Type>::Close ()
{
    for (const auto& it : _th1set) delete it.second;
    _isdelete = true;
}

#endif
