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
        void SetSignalPdf (std::map<std::string, RooExtendPdf*> exsigpdf);
        void BuilderCore (bool isblind = true, bool iscombined =false);
    
    private:

        bool hasSignal_;
        double resolution_;
        double min_;
        double max_;
        RooRealVar* _var;
        RooAbsPdf* _bestpdf;
        RooDataSet* _data;
        std::vector<RooAbsPdf*> _multipdf;
        std::string _outdir;
        std::map<std::string, RooExtendPdf*> exsigpdf_;

        double GetNormNLL (double norm, double varlow, double varhigh);
        double GetAsymmetryError (double bestPoint ,double nllbest, double boundary, double varlow, double varhigh, double diff);
};

#endif
