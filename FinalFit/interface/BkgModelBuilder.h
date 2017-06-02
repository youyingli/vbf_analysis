#ifndef __BKGMODELBUILDER__
#define __BKGMODELBUILDER__

#include <vector>
#include <map>
#include <string>
#include <utility>

#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"

typedef std::vector<std::pair<std::string,int>> vector_pair;

class BkgModelBuilder {

    public:

        BkgModelBuilder (RooRealVar* mass, RooDataSet* data, std::string outdir);
        ~BkgModelBuilder ();

        void SetBkgPdfSource (std::vector<std::string> bkgpdf);
        void FTestSelector (int last_order);
        void MakeEnvelopePlot ();
        void SetSignalPdf (std::map<std::string, RooExtendPdf*> exsigpdf);

    private:

        bool hasSignal_;
        std::string bestpdf_;
        int bestorder_;
        RooRealVar* mass_;
        RooDataSet* data_;
        std::string outdir_;

        std::vector<std::string> bkgpdfset_;
        vector_pair finalbkgcand_;
        std::map<std::string, RooExtendPdf*> exsigpdf_;

        std::map<std::string, int> FirstFTest (int last_order);
        void FinalFTest (std::map<std::string, int> pdfcand);
        double GetGoodnessOfFit (RooAbsPdf* testPdf);

};

#endif
