#ifndef __TH1SERVICE_IPP__
#define __TH1SERVICE_IPP__

#include "vbf_analysis/Utils/interface/TH1Service.hpp"

#include <iostream>
using namespace std;

template <typename TH1Type>
TH1Service<TH1Type>::TH1Service ()
{
    isfile_ = false;
}

template <typename TH1Type>
TH1Service<TH1Type>::~TH1Service()
{   
    if (isfile_) for( const auto& it : fileset_ ) it->Close();
}

template <typename TH1Type>
void TH1Service<TH1Type>::AddNewTH1 ( string name, int nbin, double min, double max )
{
    th1set_.emplace( name, new ModifiedTH1<TH1Type>( name, nbin, min, max ) );
}

template <typename TH1Type>
void TH1Service<TH1Type>::AddPlotFromFile ( string name, string plotname, string filename )
{
    TFile* file = TFile::Open( filename.c_str() );
    if ( file -> IsZombie() ) {
        cout <<"[ERROR]: " << file << "can not be found !" << endl;
        exit ( EXIT_FAILURE );
    }
    TH1Type* plot = (TH1Type*)file-> Get( plotname.c_str() );
    th1set_.emplace( name, new ModifiedTH1<TH1Type>( plot ) );
    fileset_.emplace_back(file);
    isfile_ = true;
}


template <typename TH1Type>
void TH1Service<TH1Type>::Delete ( string name ) 
{
    delete th1set_[name];
    th1set_[name] = NULL;
    th1set_.erase(name);
}

template <typename TH1Type>
void TH1Service<TH1Type>::Close ()
{
    for ( const auto& it : th1set_ ) delete it.second;
}

#endif
