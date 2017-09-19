#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "vbf_analysis/DataMcComparison/interface/DataMcComparison.h"
#include "vbf_analysis/DataMcComparison/interface/SystematicsManager.h"
#include "vbf_analysis/Utils/interface/ScaleFactorCalculator.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"

#include <iostream>
#include <string>
using namespace std;

int main( int argc, char* argv[] ){

    string inPSet_;
    string outdir_;

    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("PSet",po::value<string>(&inPSet_)          ,"Input Parameter Set python file!")
            ("output,o", po::value<string>(&outdir_)     ,"Output directory"  )
        ;

        po::variables_map vm;
        po::store( po::parse_command_line( argc, argv, desc ), vm );
        po::notify( vm );

        if (vm.count("help")) { cout << desc << "\n"; return 0; }
        if (outdir_.empty()) {cout << "Please input your output path !" << endl; return 0;}
        if (inPSet_.empty()) {cout << "Please input PSet !" << endl; return 0;}
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }
    system(Form("mkdir -p %s",outdir_.c_str()));

//-----------------------------------------------------------------------------------------------------------------------------------
// Parameter Set
    const edm::ParameterSet &process = edm::readPSetsFrom(inPSet_)->getParameter<edm::ParameterSet>("process");

    const edm::ParameterSet &GlobalSet = process.getParameter<edm::ParameterSet>("GlobalSet");
    string ControlPlot = GlobalSet.getParameter<string>("ControlPlot");
    bool isSideBand = GlobalSet.getParameter<bool>("isSideBand");
    bool isLogScale = GlobalSet.getParameter<bool>("isLogScale");

    const vector<edm::ParameterSet> PlotInfo = process.getParameter<vector<edm::ParameterSet>>("PlotInfo");

    const edm::ParameterSet &DYjetsSamples = process.getParameter<edm::ParameterSet>("DYjetsSamples");
    string Total     = DYjetsSamples.getParameter<string>("Total");
    string Jet1Fake  = DYjetsSamples.getParameter<string>("Jet1Fake");
    string Jet2Fake  = DYjetsSamples.getParameter<string>("Jet2Fake");
    string BothFake  = DYjetsSamples.getParameter<string>("BothFake");
    string BothMatch = DYjetsSamples.getParameter<string>("BothMatch");

    const edm::ParameterSet &Data = process.getParameter<edm::ParameterSet>("Data");
    string File = Data.getParameter<string>("File");
    double Lumi = Data.getParameter<double>("Lumi");

    const edm::ParameterSet &Systematics = process.getParameter<edm::ParameterSet>("Systematics");
    bool AddSyst = Systematics.getParameter<bool>("AddSyst");
    vector<string> SystLabels = Systematics.getParameter<vector<string>>("SystLabel");
//-------------------------------------------------------------------------------------------------------------
    vector<string> ZPlusJets;
    ZPlusJets.emplace_back(Total);

    //Scale_weight
    ScaleFactorCalculator scalefactorcalculator(ControlPlot);
    scalefactorcalculator.SetMCSamples(ZPlusJets);
    scalefactorcalculator.SetData(File);
    scalefactorcalculator.SetKeepRange(70., 110.);
    scalefactorcalculator.SetExtraWeight(Lumi);
    double scale_factor = 1.;
    if(isSideBand) scale_factor = scalefactorcalculator.GetScaleFactor();

    for ( const auto& pset : PlotInfo ) {
        string PlotName = pset.getParameter<string>("PlotName");
        string XaxisName = pset.getParameter<string>("XaxisName");

        DataMcComparison comparePlot("ZPlusJets", PlotName, outdir_+"/ZPlusJetsValidation/");
        if (AddSyst) {
            SystematicsManager SystManager(PlotName);
            SystManager.SetMcCollection(ZPlusJets);
            for ( const auto& it : SystLabels ) SystManager.AddSystLabel(it);
            SystManager.ErrorPropagator(scale_factor*Lumi);
            comparePlot.SetSystError(SystManager.GetSystError(),AddSyst);
        }
        comparePlot.SetLumi(Lumi);
        comparePlot.SetBkgComp(ZPlusJets, "All", kOrange, 1001, scale_factor);
        comparePlot.SetData(File); 
        comparePlot.SetSignal(BothMatch, "both match", kAzure-3, 1, scale_factor);
        comparePlot.SetSignal(Jet1Fake, "jet_{1} fake", kViolet+6, 1, scale_factor);
        comparePlot.SetSignal(Jet2Fake, "jet_{2} fake", kTeal+8, 1, scale_factor);
        comparePlot.SetSignal(BothFake, "both fake", kRed+1, 1, scale_factor);
        comparePlot.SetLogScale(isLogScale);
        comparePlot.DrawDriven(XaxisName,"");
    }

return 0;
}

