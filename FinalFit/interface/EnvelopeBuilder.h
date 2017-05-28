#ifndef __ENVELOPEBUILDER__
#define __ENVELOPEBUILDER__

#include <vector>

#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"

class EnvelopeBuilder {

    public:

        EnvelopeBuilder (RooRealVar* var, RooDataSet* data, std::vector<RooAbsPdf*> multipdf, std::string outdir);
        ~EnvelopeBuilder ();

        void SetBestPdf (RooAbsPdf* bestpdf);
        void SetRangeAndRes (double resolution, double min, double max);
        void BuilderCore (bool isblind = true, bool combine =true);
    
    private:


        double resolution_;
        double min_;
        double max_;
        RooRealVar* var_;
        RooAbsPdf* bestpdf_;
        RooDataSet* data_;
        std::vector<RooAbsPdf*> multipdf_;
        std::string outdir_;

        double GetNormNLL (double norm, double varlow, double varhigh);
        double GetAsymmetryError (double bestPoint ,double nllbest, double boundary, double varlow, double varhigh, double diff);
};

#endif
