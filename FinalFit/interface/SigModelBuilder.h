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

        SigModelBuilder (RooRealVar* mass, RooDataSet* data, const std::string& samplename, const std::string& outdir);
        ~SigModelBuilder ();

        void FTestSelector ();
        void AlterFTestSelector ();
        void MakeFTestPlot ();
        RooExtendPdf* GetBestOfFitPdf ();


    private:

        RooRealVar* _mass;
        RooDataSet* _data;
        std::string _samplename;
        std::string _outdir;
        RooDataSet* _unweightdset;

        std::vector<RooAbsPdf*> _sigpdfset;

};

#endif
