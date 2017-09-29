#ifndef __FITMANAGER__
#define __FITMANAGER__

#include "RooArgSet.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooAbsData.h"
#include "RooFitResult.h"

#include "TH1.h"

#include <string>
#include <vector>
#include <utility>
//#include <tuple>

class FitManager {

    public:

        FitManager (const RooArgSet& varset, RooAbsPdf* pdf);
        ~FitManager ();
        void SetDataSetFitted (RooDataSet* data);
        void Use1DBinnedFitting (int nbin, double minvar, double maxvar);
        void UseUnBinnedFitting (double minvar, double maxvar, bool isExtended = false);
        void SetConstParam (const std::string& paramname, bool flag);
        void SetParamVal (const std::string& paramname, double val);
        double GetParamVal (const std::string& paramname);
        void FixPdfShape (bool flag);
        void SetOriginalPdf ();
        double GetChiSquare () const { return GetChi2OverDof() * (_nbin - _fitres->floatParsFinal().getSize()); }
        double GetChi2OverDof () const;
        double GetMinNLL () const { return _fitres->minNll(); }
        void ShowParamsInfo ();
        RooAbsPdf* GetPostPdf () const { return _pdf; };
        //RooAbsData* GetData () const { return (RooAbsData*)_dset; };

    private:

        std::vector<std::string> _varstrs;
        //std::vector<std::tuple<std::string, double, double>> _initobsset;
        std::pair<double, double> _initobsrange;
        RooDataSet* _dset;
        RooAbsData* _data;
        RooAbsPdf* _pdf;
        RooArgSet* _varset;
        RooArgSet* _params;
        RooArgSet* _orisnapshot;
        RooFitResult* _fitres;
        bool _is1DFit;

        //Binned Fit
        bool _isbinnedfit;
        int _nbin;

        //private function
        void HasParameter (const std::string& name);
        RooAbsData* UnWgtDataSetBuilder (RooRealVar* var, RooDataSet* dset, std::string datatype, double minvar, double maxvar, int nbin = 100);

};

#endif
