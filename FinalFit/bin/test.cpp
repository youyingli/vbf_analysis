#include "vbf_analysis/FinalFit/interface/BkgModelBuilder.h"

#include "TFile.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooDataSet.h"

#include <iostream>
#include <string>
#include <vector>
using namespace std;
using namespace RooFit;

int main(){

    RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);
    RooMsgService::instance().setSilentMode(true);

    string outdir_ = "/wk_cms/youying/CMSSW_8_0_26_patch1/src/VBFTag/FinalFit/";
    system(Form("mkdir -p %s",outdir_.c_str()));
    string infile = "/wk_cms/youying/CMSSW_8_0_26_patch1/src/VBFTag/output_DoubleEG.root";

    TFile* file = TFile::Open(infile.c_str());
    RooWorkspace* ws = (RooWorkspace*) file -> Get("ws");
    RooRealVar* mass = (RooRealVar*) ws->var("CMS_hgg_mass");
    RooDataSet* data = (RooDataSet*) ws->data("dataset");


    vector<string> aa;
    aa.push_back("Bernstein");
    aa.push_back("Laurent");
    aa.push_back("PowerLaw2");
    aa.push_back("Exponential2");

    BkgModelBuilder bkgmodelbuilder(mass,data,outdir_);
    bkgmodelbuilder.SetBkgPdfSource(aa);
    bkgmodelbuilder.FTestSelector(7);
    bkgmodelbuilder.MakeEnvelopePlot();

return 0;
}

