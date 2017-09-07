#ifndef __FITMANAGER__
#define __FITMANAGER__

#include "RooArgSet.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooFitResult.h"

#include "TH1.h"

#include <string>
#include <vector>

class FitManager {

    public:

        FitManager (const RooArgSet& varset, RooAbsPdf* pdf);
        ~FitManager ();
        void SetDataSetFitted (RooDataSet* data, bool isbinned = false, int nbin = 1);
        void ImportTH1 (TH1* histfitted);
        void Use1DBinnedFitting (double minvar, double maxvar);
        void UseUnBinnedFitting (double minvar, double maxvar);
        void SetConstParam (const std::string& paramname, bool flag);
        void SetParamVal (const std::string& paramname, double val);
        double GetParamVal (const std::string& paramname);
        void FixPdfShape (bool flag);
        void SetOriginalPdf ();
        double GetChi2OverDof () const;
        double GetMinNLL () const { return _fitres->minNll(); }
        void ShowParamsInfo ();
        RooAbsPdf* GetPostPdf () { return _pdf; };

    private:

        std::vector<std::string> _varstrs;
        RooDataSet* _dset;
        RooDataHist* _dhist;
        RooAbsPdf* _pdf;
        RooArgSet* _varset;
        RooArgSet* _params;
        RooArgSet* _orisnapshot;
        RooFitResult* _fitres;

        //Binned Fit
        int _nbin;
        double _fitrange;
        double _fullrange;

        //private function
        void HasParameter (const std::string& name);

};

#endif
