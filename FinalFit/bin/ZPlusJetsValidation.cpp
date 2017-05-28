#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "vbf_analysis/FinalFit/interface/BkgModelBuilder.h"

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

    const edm::ParameterSet &DataInfo = process.getParameter<edm::ParameterSet>("DataInfo");
    string ControlPlot = GlobalSet.getParameter<string>("ControlPlot");
    bool isSideBand = GlobalSet.getParameter<bool>("isSideBand");
    bool isLogScale = GlobalSet.getParameter<bool>("isLogScale");


//-------------------------------------------------------------------------------------------------------------
    f
    BkgModelBuilder bkgmodelbuilder();
    bkgmodelbuilder.FTestSelector(7);
    bkgmodelbuilder.MakeEnvelopePlot();

return 0;
}

