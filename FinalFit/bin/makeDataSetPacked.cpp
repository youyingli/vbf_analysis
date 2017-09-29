#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooWorkspace.h"

#include "TFile.h"
#include "TTree.h"

#include <iostream>

using namespace std;
using namespace RooFit;


RooDataSet* GetWeightDataSet (const string& infilename, int lumi = 1.) {

    TFile* infile = TFile::Open(infilename.c_str());
    RooWorkspace* ws = (RooWorkspace*) infile->Get("ws");
    RooDataSet* dataset      = (RooDataSet*) ws->data("dataset");
    RooRealVar* CMS_hgg_mass = (RooRealVar*) ws->var("CMS_hgg_mass");
    RooRealVar* Dijet_BDT    = (RooRealVar*) ws->var("Dijet_BDT");
    RooRealVar* weight       = (RooRealVar*) ws->var("weight");
                                                                                                          
    RooDataSet* updatedataset = (RooDataSet*) dataset->emptyClone()->reduce(RooArgSet(*CMS_hgg_mass, *Dijet_BDT));
    for (int i = 0; i < dataset->numEntries(); i++) {
        CMS_hgg_mass->setVal(dataset->get(i)->getRealValue("CMS_hgg_mass"));
        Dijet_BDT   ->setVal(dataset->get(i)->getRealValue("Dijet_BDT"   ));
        weight      ->setVal(dataset->weight() * lumi);
        updatedataset->add(RooArgSet(*CMS_hgg_mass, *Dijet_BDT, *weight), weight->getVal());
    }

    return updatedataset;
}

RooDataSet* GetUnWeightDataSet (const string& infilename) {

    TFile* infile = TFile::Open(infilename.c_str());
    RooWorkspace* ws = (RooWorkspace*) infile->Get("ws");
    RooDataSet* dataset      = (RooDataSet*) ws->data("dataset");
    RooRealVar* CMS_hgg_mass = (RooRealVar*) ws->var("CMS_hgg_mass");
    RooRealVar* Dijet_BDT    = (RooRealVar*) ws->var("Dijet_BDT");
         
    RooDataSet* updatedataset = new RooDataSet("_unweightdset", "", RooArgSet(*CMS_hgg_mass, *Dijet_BDT));
    for (int i = 0; i < dataset->numEntries(); i++) {
        CMS_hgg_mass->setVal(dataset->get(i)->getRealValue("CMS_hgg_mass"));
        Dijet_BDT   ->setVal(dataset->get(i)->getRealValue("Dijet_BDT"   ));
        updatedataset->add(RooArgSet(*CMS_hgg_mass, *Dijet_BDT));
    }

    return updatedataset;
}

int main(){

    const double lumi = 35.89;
    string filepath = "/wk_cms2/youying/myWork/CMSSW_9_2_8/src/VBF/";
    
    map<string, string> signames;
    signames["vbf_125_dataset"]= filepath + "output_VBFHToGG_M125_13TeV_amcatnlo_pythia8.root";
    signames["ggh_125_dataset"]= filepath + "output_GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8.root";

    vector<string> bkgnames;
    bkgnames.push_back(filepath + "output_DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa.root");
    bkgnames.push_back(filepath + "output_GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_fakefake.root");
    bkgnames.push_back(filepath + "output_GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_promptfake.root");
    bkgnames.push_back(filepath + "output_GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8_fakefake.root");
    bkgnames.push_back(filepath + "output_GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8_promptfake.root");
    bkgnames.push_back(filepath + "output_GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_fakefake.root");
    bkgnames.push_back(filepath + "output_GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_promptfake.root");

    string realdataname = filepath + "output_DoubleEG.root";


    RooWorkspace* ws = new RooWorkspace("workspace");
    for (const auto& it : signames) {
        RooDataSet* tmp = GetWeightDataSet(it.second, lumi);
        tmp->SetName(it.first.c_str());
        ws->import(*tmp);
    }
    bool isfirst = true;
    RooDataSet* bkgdataset = new RooDataSet();
    for (const auto& it : bkgnames) {
        if (isfirst) { bkgdataset = GetWeightDataSet(it, lumi); isfirst = false; bkgdataset->SetName("Bkg_13TeV_dataset"); }
        else {
            RooDataSet* tmp = GetWeightDataSet(it, lumi);
            bkgdataset->append(*tmp);
        }
    }
    ws->import(*bkgdataset);

    RooDataSet* data = GetUnWeightDataSet(realdataname.c_str());
    data->SetName("DoubleEG_2016");
    ws->import(*data);

    ws->writeToFile("FullDataSetPacked.root", true);
    ws->Print("v");

    delete ws;

    return 0;
}
