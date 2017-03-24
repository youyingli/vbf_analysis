#ifndef __DATAMCCOMPARISON__
#define __DATAMCCOMPARISON__

#include "vbf_analysis/Utils/interface/PlotLibrary.h"

class DataMcComparison{

    public:
        DataMcComparison( std::string plotName, std::string outDir );
        ~DataMcComparison();

        void SetData( std::string fileName );
        void SetSignal( std::string fileName, std::string leg,  Color_t color, Style_t style, double shift_factor=1. );
        void SetBkgComp( std::vector<std::string> fileNames, std::string leg, Color_t color ,Style_t style, double scale_weight=1. );
        void SetLogScale( bool isLog = false );
        void SetLumi( double Lumi );
        void SetSystError( std::vector<double> Up, std::vector<double> Down, bool doSysts );
        void DrawDriven( std::string xtitle );


    private:
        bool isSignal_;
        bool isBkg_;
        bool isData_;
        bool isLog_;
        bool gflag_;
        bool doSysts_;
        int nbin_;
        double Lumi_;
        double scale_weight_;
        std::string plotName_;
        std::string outDir_;
        TCanvas* canv_;
        THStack* stackPlot_;
        TLegend* legend;
        TH1D* hData_;
        TH1D* hBkg_;
        std::vector<TH1D*> signalCollection_;
        std::vector<double> UpError_;
        std::vector<double> DownError_;
        std::vector<double> TotalError_;
        std::vector<double> TotalrError_;
        std::vector<double> CenterValue_;
        std::vector<double> CenterrValue_;
        void GetTotalMCError(bool AddSysts, double scale);
        void end();

};


#endif 
