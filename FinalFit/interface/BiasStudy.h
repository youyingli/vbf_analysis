#ifndef __BIASSTUDY__
#define __BIASSTUDY__

#include "RooAbsPdf.h"
#include "RooAbsReal.h"
#include "RooArgList.h"
#include "RooArgSet.h"
#include "RooDataSet.h"
#include "RooPlot.h"

#include "TString.h"

#include <vector>
#include <map>



class BiasStudy{

    public:

        BiasStudy (RooArgSet vars, RooAbsPdf* TestPdf);
        ~BiasStudy ();

        void GenerateFakeExpt (RooAbsPdf* templatePdf, int neventsperexpt = 0, int nexpt = 1000);
        void TestManager (const RooArgList& tmpParams);
        RooPlot* GetPullDistribution (const RooAbsReal& testParam, int nbin);
        
    private:

        RooArgSet vars_;
        RooAbsPdf* TestPdf_;
        RooDataSet* pullresult_;
        std::vector<RooDataSet*> fakedataset_;
        std::map<TString, RooRealVar*> pullvars_;

};

#endif
