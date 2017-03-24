//JSON file http://www.boost.org/doc/libs/1_61_0/doc/html/property_tree/accessing.html
//          http://zenol.fr/blog/boost-property-tree/en.html

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
namespace pt = boost::property_tree;

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <iostream>
#include <string>
using namespace std;

#include "vbf_analysis/DataMcComparison/interface/DataMcComparison.h"
#include "vbf_analysis/DataMcComparison/interface/SystematicsManager.h"

struct info_tuple{
    string sample;
    string dir;
    string file;
    double weight;
};

int main( int argc, char* argv[] ){

    string jsonfile_;
    string plotInfoJson_;
    string outdir_;
    string SystLabel_;
    int mH_;
    double lumi_;
    bool DataMcCompare_ = false;
    bool Validation_ = false;
    bool AddSystematics_ = false;
    bool isLogScale_ = false;
    bool isSideBand_ = false;

    vector<string> SystLabelSet;

    string default_dir;
    default_dir = std::string( getenv("CMSSW_BASE") ) + "/src/";

    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("json,J", po::value<string>(&jsonfile_)                                             ,"Input a json file which records input root file infomations" )
            ("plotInfo,In", po::value<string>(&plotInfoJson_)->default_value(default_dir+"/vbf_analysis/DataMcComparison/data/plotsInfo.json")
                                                                                                 ,"Input plot info which records plots you want to draw"        )
            ("lumi,L", po::value<double>(&lumi_)->default_value(1.)                              ,"luminosity"                                                  )
            ("mH", po::value<int>(&mH_)->default_value(125)                                      ,"Higgs mass"                                                  )
            ("output,o", po::value<string>(&outdir_)->default_value(default_dir+"/VBFTag/")      ,"Output directory"                                            )
            ("AddSystematics", po::value<string>(&SystLabel_)                                    ,"Set Log Scale on y axis"                                     )
            ("DataMcCompare"                                                                     ,"Data and backgroumd comparison"                              )
            ("Validation"                                                                        ,"Do Validation plot"                                          )
            ("isLogScale"                                                                        ,"Set Log Scale on y axis"                                     )
            ("isSideBand"                                                                        ,"Normalize to SB region"                                      )
        ;

        po::variables_map vm;
        po::store( po::parse_command_line( argc, argv, desc ), vm );
        po::notify( vm );

        if (vm.count("help")){ cout << desc << "\n"; return 0; }        
        if (vm.count("DataMcCompare")) DataMcCompare_ = true;
        if (vm.count("Validation")) Validation_ = true;
        if (vm.count("AddSystematics")) AddSystematics_ = true;
        if (vm.count("isLogScale"))    isLogScale_    = true;
        if (vm.count("isSideBand"))    isSideBand_    = true;
        //Split
        boost::split(SystLabelSet, SystLabel_, boost::is_any_of(","));

    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }
    system(Form("mkdir -p %s",outdir_.c_str()));

    struct info_tuple info;
    vector<struct info_tuple> infoSet;
    pt::ptree sampleInfo;
    pt::read_json(jsonfile_, sampleInfo);

    for ( const auto& it : sampleInfo ) {
        info.sample = it.first;
        info.dir  = it.second.get<string>("dir");
        info.file = it.second.get<string>("file");
        info.weight = it.second.get<double>("weight");
        infoSet.emplace_back(info);
    }

    if ( DataMcCompare_ ) {
        pt::ptree plotsInfo;
        pt::read_json( plotInfoJson_, plotsInfo );

        for ( const auto& it_plots : plotsInfo ) {
            double bkgsb =0.;
            double datasb=0.;
            double sbweight = 1.;
            double shift = 1.;
            vector<string> BkgSampleCollection;
            if ( isLogScale_ ) shift = 20.;
            DataMcComparison comparePlot( it_plots.first, outdir_+"DataMcComparison/" );
            vector<string> dipho, gjet, DYjet, QCD;
            string vbf, ggf, data;
            for ( const auto& it_sample : infoSet ) {
                string infile = it_sample.dir+it_sample.file;
                if ( it_sample.sample=="vbf_"+boost::lexical_cast<string>(mH_) ) 
                    //comparePlot.SetSignal( infile+".root", "VBF(m_{H}="+boost::lexical_cast<string>(mH_)+")", kRed, 1, shift );
                    vbf = infile+".root";

                if ( it_sample.sample=="ggh_"+boost::lexical_cast<string>(mH_) )
                    //comparePlot.SetSignal( infile+".root", "ggH(m_{H}="+boost::lexical_cast<string>(mH_)+")", kYellow, 1, shift );
                    ggf = infile+".root";

                if ( it_sample.sample=="dipho" ) {
                    dipho.emplace_back( infile+".root" ); bkgsb += it_sample.weight;
                    BkgSampleCollection.emplace_back( infile );
                }
                if ( it_sample.sample=="gjet_promptfake" || it_sample.sample=="gjet_fakefake" ) {
                    gjet.emplace_back( infile+".root" ); bkgsb += it_sample.weight;
                    BkgSampleCollection.emplace_back( infile );
                }
                //if ( it_sample.sample=="DYjets" ) {
                //  DYjet.emplace_back( infile+".root" ); bkgsb += it_sample.weight;
                //    BkgSampleCollection.emplace_back( infile );
                //}
                //if ( it_sample.sample=="qcd_promptfake" || it_sample.sample=="qcd_fakefake" ) {
                //  QCD.emplace_back( infile+".root" ); bkgsb += it_sample.weight;
                //    BkgSampleCollection.emplace_back( infile );
                //  }
                if ( it_sample.sample=="Data" ) {
                    //comparePlot.SetData( infile+".root" ); datasb += it_sample.weight;
                    data = infile + ".root"; datasb += it_sample.weight;
                }
            }
            if (AddSystematics_) {
                SystematicsManager SystManager( it_plots.first );
                SystManager.SetMcCollection( BkgSampleCollection );
                for ( const auto& it : SystLabelSet ) SystManager.AddSystLabel(it);
                SystManager.ErrorPropagator(1.);
                comparePlot.SetSystError(SystManager.GetSystUpError(),SystManager.GetSystDownError(),AddSystematics_);
            }

            if( isSideBand_ ) sbweight = datasb / bkgsb;
            cout << "scale factor = " << sbweight << endl;
            comparePlot.SetSignal( vbf, "VBF(m_{H}="+std::to_string(mH_)+")", kRed, 1, shift );
            comparePlot.SetSignal( ggf, "ggH(m_{H}="+std::to_string(mH_)+")", kYellow, 1, shift );
            comparePlot.SetBkgComp( dipho, "#gamma #gamma", kAzure-3, 1001, sbweight );
            comparePlot.SetBkgComp( gjet, "#gamma Jet", kTeal+8, 1001, sbweight );
            comparePlot.SetData( data ); 
            //comparePlot.SetBkgComp( DYjet, "Drell-Yan", kViolet+6, 1001, sbweight );
            //comparePlot.SetBkgComp( QCD, "QCD", kOrange+7, 1001, sbweight );
            comparePlot.SetLogScale( isLogScale_ );
            comparePlot.SetLumi( lumi_ );
            //comparePlot.SetSystError(SystManager.GetSystUpError(),SystManager.GetSystDownError(),AddSystematics_);
            comparePlot.DrawDriven( it_plots.second.data() );       
        }
    }//DataMC

    if (Validation_) {

        pt::ptree plotsInfo;
        pt::read_json( default_dir+"/vbf_analysis/DataMcComparison/data/ZJetsplotInfo.json", plotsInfo );
                                                                                                                                   
        for ( const auto& it_plots : plotsInfo ) {
            double bkgsb =0.;
            double datasb=0.;
            double sbweight = 1.;
            double shift = 1.;
            vector<string> BkgSampleCollection;
            if ( isLogScale_ ) shift = 10.;
            DataMcComparison comparePlot( it_plots.first, outdir_+"Validation/" );
            vector<string> ZJets;
            string jet1fake, jet2fake, bothfake, bothmatch, data;
            for ( const auto& it_sample : infoSet ) {
                string infile = it_sample.dir+it_sample.file;
                if ( it_sample.sample=="DYjetsjet1fake" ) jet1fake = infile + ".root";
                if ( it_sample.sample=="DYjetsjet2fake" ) jet2fake = infile + ".root";
                if ( it_sample.sample=="DYjetsbothfakes" ) bothfake = infile + ".root";
                if ( it_sample.sample=="DYjetsbothmatch" ) bothmatch = infile + ".root";
                if ( it_sample.sample=="DYjets" ) {
                    ZJets.emplace_back( infile+".root" ); bkgsb += it_sample.weight;
                    BkgSampleCollection.emplace_back( infile );
                }
                if ( it_sample.sample=="Data" ) {
                    data = infile + ".root"; datasb += it_sample.weight;
                }
            }
            if( isSideBand_ ) sbweight = datasb / bkgsb;
            if (AddSystematics_) {
                SystematicsManager SystManager( it_plots.first );
                SystManager.SetMcCollection( BkgSampleCollection );
                for ( const auto& it : SystLabelSet ) SystManager.AddSystLabel(it);
                SystManager.ErrorPropagator(sbweight);
                comparePlot.SetSystError(SystManager.GetSystUpError(),SystManager.GetSystDownError(),AddSystematics_);
            }                                                                                                                                   
            cout << "scale factor = " << sbweight << endl;
            shift = sbweight;
            comparePlot.SetBkgComp( ZJets, "All", kOrange, 1001, sbweight );
            comparePlot.SetData(data);
            comparePlot.SetSignal( bothmatch, "both match", kAzure-3, 1, shift );
            comparePlot.SetSignal( jet1fake, "jet_{1} fake", kViolet+6, 1, shift );
            comparePlot.SetSignal( jet2fake, "jet_{2} fake", kTeal+8, 1, shift );
            comparePlot.SetSignal( bothfake, "both fake", kRed+1, 1, shift );
            comparePlot.SetLogScale( isLogScale_ );
            comparePlot.SetLumi( lumi_ );
            comparePlot.DrawDriven( it_plots.second.data() );       
        }


    }//Validation


return 0;
}


//To do 
