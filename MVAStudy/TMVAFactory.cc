
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
namespace pt = boost::property_tree;

#include <iostream>
using namespace std;

#include "TFile.h"

#include "vbf_analysis/analysis/interface/TMVAFactory.h"


TMVAFactory::TMVAFactory( string name ):
    name_( name )
{
    outMVAfile_ = TFile::Open("TMVA.root","RECREATE");
    factory_ = new TMVA::Factory( "TMVAClassification", outMVAfile_,"!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
    string jsonfile;
    jsonfile = jsonfile.assign( getenv("CMSSW_BASE") ) + "/src/makePlot/Validation/data/TMVAvariables.json";
    pt::ptree InfoRoot;
    pt::read_json(jsonfile, InfoRoot);
    for ( const auto& vars : InfoRoot.get_child( name_ ) ) factory_->AddVariable( vars.first.c_str(), vars.second.data().c_str(), "", 'F' );
}

TMVAFactory::~TMVAFactory()
{
    delete factory_;
}


void TMVAFactory::SetSignalSamples( vector<string> sig )
{
    double weightS = 1.0;
    for ( const auto& it : sig ) {

        TFile*sfile = TFile::Open(Form("%s",it.c_str()));
        TTree* stree = (TTree*)sfile->Get("MVA_variables");
        factory_ ->AddSignalTree( (TTree*)stree->Clone(), weightS );
    }
}

void TMVAFactory::SetBackgroundSamples( vector<string> bkg )
{
    double weightB = 1.0;
    for ( const auto& it : bkg ){

        TFile*sfile = TFile::Open(Form("%s",it.c_str()));
        TTree* stree = (TTree*)sfile->Get("MVA_variables");
        factory_ ->AddBackgroundTree( (TTree*)stree->Clone(), weightB );
    }

}

void TMVAFactory::BeginTrainingTest()
{

    if (useWeight_) factory_ ->SetBackgroundWeightExpression( "tmvaWgt" );

    factory_ ->PrepareTrainingAndTestTree( "", "",
                                         "SplitMode=Random:NormMode=NumEvents:!V" );
    if ( algorithm_ == "BDTG" )
        factory_ ->BookMethod( TMVA::Types::kBDT, "BDTG",
                         "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.30:UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=20:NNodesMax=5:MaxDepth=3" );
    if ( algorithm_ == "BDT" )
        factory_ ->BookMethod( TMVA::Types::kBDT, "BDT",
               "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );



    // ---- Now you can tell the factory to train, test, and evaluate the MVAs
    //
    // Train MVAs using the set of training events
    factory_ ->TrainAllMethods();
    
    // ---- Evaluate all MVAs using the set of test events
    factory_ ->TestAllMethods();
    
    // ----- Evaluate and compare performance of all configured MVAs
    factory_ ->EvaluateAllMethods();

    // Save the output
    outMVAfile_ ->Close();

    std::cout << "==> Wrote root file: " << outMVAfile_ ->GetName() << std::endl
              << "==> TMVAClassification is done!" << std::endl
              << std::endl
              << "==> To view the results, launch the GUI: \"root -l ./TMVAGui.C\"" << std::endl
              << std::endl;
}

void TMVAFactory::useWeight( bool useWgt )
{
    useWeight_ = useWgt;
}
void TMVAFactory::BookMVA( string algorithm )
{
    algorithm_ = algorithm;
}
