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
        void MakeEnvelopePlot (bool isblind = true, bool iscombined = false);
        void SetSignalPdf (std::map<std::string, RooExtendPdf*> exsigpdf);

    private:

        bool _hasSignal;
        std::string _bestpdf;
        int _bestorder;
        RooRealVar* _mass;
        RooDataSet* _data;
        std::string _outdir;

        std::vector<std::string> _bkgpdfset;
        vector_pair _finalbkgcand;
        std::map<std::string, RooExtendPdf*> _exsigpdf;

        //private function
        std::map<std::string, int> FirstFTest (int last_order);
        void FinalFTest (std::map<std::string, int> pdfcand);
        double GetGoodnessOfFit (RooAbsPdf* testPdf);

};

#endif
