#include <boost/program_options.hpp>
namespace po = boost::program_options;

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
            ("PSet",po::value<string>(&inPSet_)->required()          ,"Input Parameter Set python file!")
            ("outdir,o",po::value<string>(&outdir_)->required()   ,"Output directory"                                            )
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
    const edm::ParameterSet DYjetsSample = process.getParameter<edm::ParameterSet>("DYjetsSample");
    string Name = DYjetsSample.getParameter<string>("Name");
    string File = DYjetsSample.getParameter<string>("File");
    vector<string> jetstate = DYjetsSample.getParameter<vector<string>>("jetstate");
    const vector<edm::ParameterSet> data = process.getParameter<vector<edm::ParameterSet>>("data");
    const edm::ParameterSet &Systematics = process.getParameter<edm::ParameterSet>("Systematics");
    bool dumpSysts = Systematics.getParameter<bool>("dumpSysts");
    vector<string> SystLabels = Systematics.getParameter<vector<string>>("SystLabels");

//------------------------------------------------------------------------------------------------------------------
    //MC
    ZPlusJetsValidSelector MCselector(indir, File, Name, outdir_);
    for (const auto& it : jetstate) MCselector.SelectLoop(it);
    
    for (const auto& SystLabels_it : SystLabels) {
        ZPlusJetsValidSelector MCselectors(indir, File, Name, outdir_);
        MCselectors.SetSystLabel(SystLabels_it);
        MCselectors.SelectLoop();
    }
    //data
    for (const auto& it : data) {
        string Name = it.getParameter<string>("Name");
        string File = it.getParameter<string>("File");
        ZPlusJetsValidSelector Selector(indir, File, Name, outdir_);
        Selector.SelectLoop();
    }

    return 0;
}


