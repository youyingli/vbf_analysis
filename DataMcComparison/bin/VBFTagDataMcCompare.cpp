//JSON file http://www.boost.org/doc/libs/1_61_0/doc/html/property_tree/accessing.html
//          http://zenol.fr/blog/boost-property-tree/en.html

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "vbf_analysis/DataMcComparison/interface/DataMcComparison.h"
#include "vbf_analysis/DataMcComparison/interface/SystematicsManager.h"
#include "vbf_analysis/Utils/interface/ScaleFactorCalculator.h"
#include "vbf_analysis/Utils/interface/VectorTool.hpp"

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
            ("PSet",po::value<string>(&inPSet_)->required()          ,"Input Parameter Set python file!")
            ("output,o", po::value<string>(&outdir_)->required()     ,"Output directory"  )
        ;

        po::variables_map vm;
        po::store( po::parse_command_line( argc, argv, desc ), vm );
        po::notify( vm );

        if (vm.count("help")){ cout << desc << "\n"; return 0; }        
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

    const edm::ParameterSet &SigMCSamples = process.getParameter<edm::ParameterSet>("SigMCSamples");
    string VBFFile = SigMCSamples.getParameter<string>("VBFFile");
    string GGFFile = SigMCSamples.getParameter<string>("GGFFile");
    unsigned mH  = SigMCSamples.getParameter<unsigned>("mH");

    const edm::ParameterSet &BkgOption = process.getParameter<edm::ParameterSet>("BkgOption");
    bool useGJ     = BkgOption.getParameter<bool>("useGJ");
    bool useDipho  = BkgOption.getParameter<bool>("useDipho");
    bool useQCD    = BkgOption.getParameter<bool>("useQCD");
    bool useDYjets = BkgOption.getParameter<bool>("useDYjets");

    const edm::ParameterSet &BkgMCSamples = process.getParameter<edm::ParameterSet>("BkgMCSamples");
    vector<string> GJFile     = BkgMCSamples.getParameter<vector<string>>("GJFile");
    vector<string> DiphoFile  = BkgMCSamples.getParameter<vector<string>>("DiphoFile");
    vector<string> QCDFile    = BkgMCSamples.getParameter<vector<string>>("QCDFile");
    vector<string> DYjetsFile = BkgMCSamples.getParameter<vector<string>>("DYjetsFile");

    const edm::ParameterSet &Data = process.getParameter<edm::ParameterSet>("Data");
    string File = Data.getParameter<string>("File");
    double Lumi = Data.getParameter<double>("Lumi");

    const edm::ParameterSet &Systematics = process.getParameter<edm::ParameterSet>("Systematics");
    bool AddSyst = Systematics.getParameter<bool>("AddSyst");
    vector<string> SystLabels = Systematics.getParameter<vector<string>>("SystLabel");
//-------------------------------------------------------------------------------------------------------------

    VectorTool<string> vectortool;
    if (useGJ)    vectortool.Append(GJFile);
    if (useDipho) vectortool.Append(DiphoFile);
    if (useQCD)   vectortool.Append(QCDFile);
    if (useDYjets)vectortool.Append(DYjetsFile);
    vector<string> BkgSampleCollection = vectortool.Output();

    //Scale_weight
    ScaleFactorCalculator scalefactorcalculator(ControlPlot);
    scalefactorcalculator.SetMCSamples(BkgSampleCollection);
    scalefactorcalculator.SetData(File);
    scalefactorcalculator.SetExtraWeight(Lumi);
    scalefactorcalculator.SetKeepRange(100., 180.);
    scalefactorcalculator.SetRemoveRange(120., 130.);
    double scale_factor = 1.;
    if(isSideBand) scale_factor = scalefactorcalculator.GetScaleFactor();

    double shift = 1.;
    if (isLogScale) shift = 20.;
    for ( const auto& pset : PlotInfo ) {
        string PlotName = pset.getParameter<string>("PlotName");
        string XaxisName = pset.getParameter<string>("XaxisName");

        DataMcComparison comparePlot("VBFTag", PlotName, outdir_+"DataMcComparison/");
        if (AddSyst) {
            SystematicsManager SystManager(PlotName);
            SystManager.SetMcCollection(BkgSampleCollection);
            for ( const auto& it : SystLabels ) SystManager.AddSystLabel(it);
            SystManager.ErrorPropagator(scale_factor*Lumi);
            comparePlot.SetSystError(SystManager.GetSystError(),AddSyst);
        }
        comparePlot.SetLumi(Lumi);
        comparePlot.SetSignal(VBFFile, "VBF(m_{H}="+std::to_string(mH)+")", kRed, 1, shift);
        comparePlot.SetSignal(GGFFile, "ggH(m_{H}="+std::to_string(mH)+")", kYellow, 1, shift);
        if (useDipho) comparePlot.SetBkgComp(DiphoFile, "#gamma #gamma", kAzure-3, 1001, scale_factor);
        if (useGJ) comparePlot.SetBkgComp(GJFile, "#gamma Jet", kTeal+8, 1001, scale_factor);
        if (useDYjets) comparePlot.SetBkgComp( DYjetsFile, "Drell-Yan", kViolet+6, 1001, scale_factor);
        if (useQCD) comparePlot.SetBkgComp( QCDFile, "QCD", kOrange+7, 1001, scale_factor );
        comparePlot.SetData(File); 
        comparePlot.SetLogScale(isLogScale);
        comparePlot.DrawDriven(XaxisName,"");
    }

return 0;
}

