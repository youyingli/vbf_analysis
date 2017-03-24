//JSON file http://www.boost.org/doc/libs/1_61_0/doc/html/property_tree/accessing.html
//          http://zenol.fr/blog/boost-property-tree/en.html
//
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
namespace pt = boost::property_tree;

#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <string>
using namespace std;

#include "vbf_analysis/Selector/interface/VBFTagSelector.h"
#include "vbf_analysis/Selector/interface/ZPlusJetsValidSelector.h"

struct info_tuple{
    string sampleType;
    string sampleName;
    string file;
    double weight;
};

int main( int argc, char* argv[] ) {

    string jsonfile_;
    string indir_;
    string outdir_;
    string level_;
    string SystLabel_ = "";
    double lumi_;
    bool isSideBand_      = false;
    bool isGenWeight_     = false;
    bool doMVAvars_       = false;
    bool doSystematics_   = false;
    bool VBFTag_      = false;
    bool ZPlusJets_   = false;
//  bool doWorkspace_ = false;

    string default_dir;
    default_dir = default_dir.assign( getenv("CMSSW_BASE") ) + "/src/VBFTag/";
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("json,J",po::value<string>(&jsonfile_)                               ,"Input a json file which records input root file infomations" )
            ("lumi,L",po::value<double>(&lumi_)->default_value(35.85)             ,"luminosity"                                                  )
            ("indir,i",po::value<string>(&indir_)                                 ,"Output file directory"                                       )
            ("outdir,o",po::value<string>(&outdir_)->default_value(default_dir)   ,"Output directory"                                            )
            ("CutLevel",po::value<string>(&level_)->default_value("Loose")        ,"Cut level, has Loose, Medium and Tight"                      )
            ("doSystematics",po::value<string>(&SystLabel_)->default_value("")    ,"Do background syst"                                          )
            ("isSideBand"                                                         ,"Only sideband region for Mgg in backgroumd and data"         )
            ("isGenWeight"                                                        ,"Use MC Gen weight"                                           )
            ("doMVAvars"                                                          ,"Output MVA vars as a tree in output rootfiles"               )
            ("VBFTag"                                                             ,"Select VBF like events"                                      )
            ("ZPlusJets"                                                          ,"Select ZPlusJet validation"                                  )
//          ("doWorkspace"                                                        ,"Make a workspace(roofit) in output rootfiles"                )
        ;

        po::variables_map vm;
        po::store( po::parse_command_line( argc, argv, desc ), vm );
        po::notify( vm );

        if (vm.count("help")){ cout << desc << "\n"; return 0; }
        if (vm.count("isSideBand"))  isSideBand_  = true;
        if (vm.count("isGenWeight")) isGenWeight_ = true;
        if (vm.count("doMVAvars"))   doMVAvars_   = true;
        if (vm.count("doSystematics"))   doSystematics_   = true;
        if (vm.count("VBFTag"))     VBFTag_   = true;
        if (vm.count("ZPlusJets"))   ZPlusJets_   = true;
//      if (vm.count("doWorkspace")) doWorkspace_ = true;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }
    system(Form("mkdir -p %s",outdir_.c_str()));

    struct info_tuple sampleInfo;
    vector<struct info_tuple> infoSet;
    pt::ptree InfoRoot;
    pt::read_json( jsonfile_, InfoRoot );

    for ( const auto& it : InfoRoot ) {
        sampleInfo.sampleType = it.first;
        for ( const auto& its : it.second ) {
            sampleInfo.sampleName = its.first;
            sampleInfo.file = its.second.get<string>("file");
            double xs =  its.second.get<double>("xs") ;
            double br =  its.second.get<double>("br") ;
            double nGen =  its.second.get<double>("nGen") ;
            if ( nGen == 0 ) assert( nGen != 0 );
            sampleInfo.weight = xs*1e3*br*lumi_/nGen;   
            infoSet.push_back( sampleInfo );
        }
    }
    pt::ptree outjson;
    //Work start
    //________________________________________________________________________________
    if (VBFTag_) {

        for( const auto&it : infoSet ){
            if( (doSystematics_ && it.sampleType == "bkg") || (SystLabel_=="") ){
                VBFTagSelector dumper( indir_+"/"+it.file, it.sampleType, it.sampleName, outdir_ );
                dumper.setEventWeight( it.weight, isGenWeight_ );
                dumper.setLumi(lumi_);
                dumper.setSystLabel(SystLabel_);
                dumper.outputMVAvars( doMVAvars_ );
                dumper.selectLoop( level_, isSideBand_ );
                pt::ptree node;
                if( SystLabel_ == "" ){
                    node.put( "dir", outdir_ );
                    node.put( "file", dumper.GetOutFileName() );
                    node.put( "weight", dumper.GetSBenents() );
                    outjson.add_child( it.sampleName, node );
                }
            }
        }
        if(SystLabel_ == "") pt::write_json( outdir_+"sampleInfo.json", outjson );
    }
    //_______________________________________________________________________________
    if (ZPlusJets_) {

        for( const auto&it : infoSet ){
            if( (doSystematics_ && it.sampleType == "ZJets") || (SystLabel_=="") ){
                ZPlusJetsValidSelector ZPlusJets(indir_+"/"+it.file, it.sampleType, it.sampleName, outdir_ );
                ZPlusJets.setEventWeight( it.weight, isGenWeight_ );
                ZPlusJets.setLumi(lumi_);
                ZPlusJets.setSystLabel(SystLabel_);
                if (SystLabel_ == "") {
                    vector<string> jetstate;
                    jetstate.emplace_back("");
                    jetstate.emplace_back("jet1fake");
                    jetstate.emplace_back("jet2fake");
                    jetstate.emplace_back("bothfakes");
                    jetstate.emplace_back("bothmatch");
                    for ( const auto& its : jetstate ) {
                        if( it.sampleType =="data" && its != "") continue;
                        if(its!="")cout << its <<endl;
                        ZPlusJets.selectLoop( level_, its  );
                        pt::ptree node;
                        node.put( "dir", outdir_ );
                        node.put( "file", ZPlusJets.GetOutFileName() );
                        node.put( "weight", ZPlusJets.GetSBenents() );
                        outjson.add_child( it.sampleName+its, node );
                    }//its++
                } else {
                    ZPlusJets.selectLoop( level_, "" );
                }
            }
        }
        if(SystLabel_ == "") pt::write_json( outdir_+"ZPlusJetsInfo.json", outjson );

    }

return 0;
}


//To do 
//1.parallel computation
