#ifndef __DATAMCCOMPARISON__
#define __DATAMCCOMPARISON__

#include "vbf_analysis/Utils/interface/PlotLibrary.h"
#include "vbf_analysis/Utils/interface/TH1Service.hpp"

class DataMcComparison{

    typedef std::vector<std::pair<double,double>> PairVector;

    public:
        DataMcComparison(std::string type, std::string plotName, std::string outDir);
        ~DataMcComparison();

        void SetData(std::string fileName);
        void SetSignal( std::string fileName, std::string SigName,  Color_t color, Style_t style, double shift_factor=1. );
        void SetBkgComp( std::vector<std::string> fileNames, std::string BkgName, Color_t color ,Style_t style, double scale_weight=1. );
        void SetLogScale( bool isLog = false );
        void SetLumi( double Lumi );
        void SetSystError( PairVector SystError, bool doSysts );
        void DrawDriven( std::string xtitle );


    private:
        TH1Service<TH1D> th1service;

        bool isSignal_;
        bool isBkg_;
        bool isData_;
        bool isLog_;
        bool gflag_;
        bool doSysts_;
        int nbin_;
        double Lumi_;
        double scale_weight_;
        std::string type_;
        std::string plotName_;
        std::string outDir_;
        TCanvas* canv_;
        THStack* stackPlot_;
        TLegend* legend;
        std::vector<ModifiedTH1<TH1D>*> signalCollection_;
        std::vector<ModifiedTH1<TH1D>*> backgroundCollection_;
        PairVector SystError_;
        PairVector TotalContent_;
        PairVector TotalrContent_;
        void GetTotalMCError(bool AddSysts);
        void end();

};


#endif 
