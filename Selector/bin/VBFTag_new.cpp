#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "vbf_analysis/Selector/interface/VBFTagSelector.h"
#include "vbf_analysis/Selector/interface/ZPlusJetsValidSelector.h"
#include "vbf_analysis/Utils/interface/VectorTool.hpp"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"

#include <iostream>
#include <string>
using namespace std;

int main( int argc, char* argv[] ) {

    string inPSet_;
    string outdir_;

    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("PSet",po::value<string>(&inPSet_)->required()       ,"Input Parameter Set python file!")
            ("outdir,o",po::value<string>(&outdir_)->required()   ,"Output directory"                )
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
    
//------------------------------------------------------------------------------------------------------------------    
    const edm::ParameterSet &process = edm::readPSetsFrom(inPSet_)->getParameter<edm::ParameterSet>("process");
    const edm::ParameterSet &GlobalSet = process.getParameter<edm::ParameterSet>("GlobalSet");
    string indir = GlobalSet.getParameter<string>("indir");
    bool isSideBand = GlobalSet.getParameter<bool>("isSideBand");
    const double MHmin = GlobalSet.getParameter<double>("MHmin");
    const double MHmax = GlobalSet.getParameter<double>("MHmax");
    const vector<edm::ParameterSet> SigMCsamples = process.getParameter<vector<edm::ParameterSet>>("SigMCsamples");
    const vector<edm::ParameterSet> BkgMCsamples = process.getParameter<vector<edm::ParameterSet>>("BkgMCsamples");
    const vector<edm::ParameterSet> data = process.getParameter<vector<edm::ParameterSet>>("data");
    const edm::ParameterSet &Systematics = process.getParameter<edm::ParameterSet>("Systematics");
    bool dumpSysts = Systematics.getParameter<bool>("dumpSysts");
    vector<string> SystLabels = Systematics.getParameter<vector<string>>("SystLabels");

//------------------------------------------------------------------------------------------------------------------
    VectorTool<edm::ParameterSet> vectortool;
    vectortool.Append(SigMCsamples);
    vectortool.Append(BkgMCsamples);
    vectortool.Append(data);
    vector<edm::ParameterSet> Total = vectortool.Output();
    Total.emplace_back("");//Still need normal 

    for (const auto& it : Total) {
        string Name = it.getParameter<string>("Name");
        string File = it.getParameter<string>("File");

        VBFTagSelector selector(indir, File, Name, outdir_);
        selector.SetSignalRegion(MHmin, MHmax);
        selector.selectLoop(isSideBand);
        if (dumpSysts && Name != "Data")
            for (const auto& SystLabel_it : SystLabels) {
                 selector.SetSystLabel(SystLabel_it);
                 selector.selectLoop(isSideBand);
            }
    }

    return 0;
}


