#include "vbf_analysis/FinalFit/interface/BiasStudy.h"

#include "RooFitResult.h"
#include "RooGaussian.h"
#include "RooMsgService.h"
#include "RooRandom.h"
#include "RooRealVar.h"

#include "TDatime.h"

using namespace std;


BiasStudy::BiasStudy (RooArgSet vars, RooAbsPdf* TestPdf):
    vars_(vars),
    TestPdf_(TestPdf)
{
    RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);
    RooMsgService::instance().setSilentMode(true);

    pullresult_ = 0;
}

BiasStudy::~BiasStudy()
{
    delete pullresult_;
}

void BiasStudy::GenerateFakeExpt (RooAbsPdf* templatePdf, int nexpt)
{
    TDatime t;
    unsigned int seed = t.GetDate() + t.GetTime();
    RooRandom::randomGenerator()->SetSeed(seed);

    int nEvent = templatePdf->expectedEvents(vars_);
    cout << "[INFO] : Producing " << nexpt << " fake experiments ..." << endl;
    for (int i = 0; i < nexpt; i++) {
        int PoissonEvent =  RooRandom::randomGenerator()->Poisson(nEvent != 0 ? nEvent : 3000);
        RooDataSet* fakedata = templatePdf->generate(vars_, PoissonEvent);
        fakedataset_.push_back(fakedata);
    }
    cout << "[INFO] : Finished ! ! !" << endl;
}

void BiasStudy::TestManager (const RooArgList& testParams)
{
    cout << "[INFO] : Initialize all test parameters . . ." << endl;
    RooArgSet* tmpParams = TestPdf_->getParameters(vars_);
    RooArgSet originParams;
    tmpParams->snapshot(originParams);

    unsigned int nParams = testParams.getSize();

    vector<TString> paramnames;
    RooArgSet paramset;
    for (unsigned int i = 0; i < nParams; i++) {
        paramnames.emplace_back( ((RooRealVar*)testParams.at(i))->GetName() );
        pullvars_.insert( make_pair( paramnames[i], new RooRealVar(paramnames[i] + "_pull", paramnames[i] +" pull", -4.5, 4.5) ) );
        paramset.add( *pullvars_[paramnames[i]] );
    }
    pullresult_ = new RooDataSet("pullresult", "pullresult", paramset);
    cout << "[INFO] : Done ! ! !" << endl;

    cout << "[INFO] : Start to fit " << fakedataset_.size() <<" fake experiments . . ." << endl;
    int i = 0;
    for (const auto& it : fakedataset_) {
        RooFitResult* res = TestPdf_->fitTo(*it, RooFit::Save(true));
        RooArgSet paramsetcomp; 

        for (const auto& paramnames_it : paramnames) {
            RooRealVar* postVar = (RooRealVar*) res->floatParsFinal().find(paramnames_it);
            RooRealVar* trueVar = (RooRealVar*) originParams.find(paramnames_it);
            pullvars_[paramnames_it]->setVal( (postVar->getVal() - trueVar->getVal()) / postVar->getError() );
            paramsetcomp.add( *pullvars_[paramnames_it] );
        }
        pullresult_->add(paramsetcomp);

        tmpParams->assignValueOnly(originParams);
        i++;
        if (i % 100 == 0) cout << "[INFO] : " << i << "fake experiments have been fitted !!!"<< endl;
    }
    cout << "[INFO] : Finished ! ! !" << endl;
}

RooPlot* BiasStudy::GetPullDistribution (const RooAbsReal& testParam, int nbin)
{
    RooPlot* frame = pullvars_[testParam.GetName()] -> frame(RooFit::Bins(nbin));
    pullresult_->plotOn(frame);

    RooRealVar mean("mean", "mean",0.,-3.,3.);
    RooRealVar sigma("sigma", "sigma",1.,0.,3.);
    RooGaussian gaus("gaus", "gaus", *pullvars_[testParam.GetName()], mean, sigma);
    gaus.fitTo(*pullresult_);
    gaus.plotOn(frame);
    gaus.paramOn(frame);

return frame;
}
