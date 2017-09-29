#include "vbf_analysis/FinalFit/interface/SigModelBuilder.h"
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

    string outdir = "/wk_cms2/youying/myWork/CMSSW_9_2_8/src/Fit/";
    system(Form("mkdir -p %s", outdir.c_str()));
    string infile = "/wk_cms2/youying/myWork/CMSSW_9_2_8/src/FullDataSetPacked.root";

    map<string, RooExtendPdf*> exsig;
    vector<string> sigdata;
    sigdata.push_back("vbf_125_dataset");
    sigdata.push_back("ggh_125_dataset");

    TFile* file = TFile::Open(infile.c_str());
    RooWorkspace* ws = (RooWorkspace*) file -> Get("workspace");
    RooRealVar* mass = (RooRealVar*) ws->var("CMS_hgg_mass");
    
    for (const auto& it : sigdata) {

        RooDataSet* data = (RooDataSet*) ws->data(it.c_str());
        SigModelBuilder sigModelBuilder(mass, data, it.c_str(), outdir);
        sigModelBuilder.FTestSelector();
        //sigModelBuilder.AlterFTestSelector();
        sigModelBuilder.MakeFTestPlot();

        exsig.insert(make_pair(it.substr(0,7), sigModelBuilder.GetBestOfFitPdf()));
    }
    mass->Print("v");

    vector<string> aa;
    aa.push_back("Bernstein");
    aa.push_back("Laurent");
    aa.push_back("PowerLaw2");
    aa.push_back("Exponential2");

    RooDataSet* data = (RooDataSet*) ws->data("DoubleEG_2016");
    RooDataSet* reducedata = (RooDataSet*) data->reduce(RooArgSet(*mass));

    BkgModelBuilder bkgmodelbuilder(mass,reducedata,outdir);
    bkgmodelbuilder.SetBkgPdfSource(aa);
    bkgmodelbuilder.FTestSelector(7);
    bkgmodelbuilder.SetSignalPdf(exsig);
    bkgmodelbuilder.MakeEnvelopePlot(false, true);

return 0;
}

