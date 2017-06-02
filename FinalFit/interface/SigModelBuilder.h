#ifndef __SIGMODELBUILDER__
#define __SIGMODELBUILDER__

#include <vector>
#include <string>

#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooExtendPdf.h"
#include "RooDataSet.h"

#define MH 125

class SigModelBuilder {

    public:

        SigModelBuilder (RooRealVar* mass, RooDataSet* data, std::string samplename, std::string outdir);
        ~SigModelBuilder ();

        void FTestSelector ();
        void AlterFTestSelector ();
        void MakeFTestPlot ();
        RooExtendPdf* GetBestOfFitPdf ();


    private:

        int bestorder_;
        RooRealVar* mass_;
        RooDataSet* data_;
        std::string samplename_;
        std::string outdir_;

        std::vector<RooAbsPdf*> SigPdfSet_;

};

#endif
