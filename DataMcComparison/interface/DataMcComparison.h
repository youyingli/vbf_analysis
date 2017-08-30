#ifndef __DATAMCCOMPARISON__
#define __DATAMCCOMPARISON__

#include "vbf_analysis/Utils/interface/PlotMgrLib.h"

class DataMcComparison{

    typedef std::vector<std::pair<double,double>> PairVector;

    public:
        DataMcComparison (const std::string& type, const std::string& plotname, const std::string& outdir);
        ~DataMcComparison();

        void SetData(const std::string& filename);
        void SetSignal(const std::string& filename, const std::string& signame,  Color_t color, Style_t style, double shift_factor = 1., bool showfactor = false);
        void SetBkgComp(const std::vector<std::string>& filenames, const std::string& bkgname, Color_t color ,Style_t style, double scale_factor = 1., bool showfactor = false);
        void SetLogScale(bool islog = false);
        void SetLumi(double lumi = 1.);
        void SetSystError(const PairVector& systerror, bool dosysts);
        void DrawDriven(const std::string& xtitle, const std::string& unit);


    private:
        plotmgr::TH1Service<TH1D> _th1service;

        bool _issignal;
        bool _isbkg;
        bool _isdata;
        bool _islog;
        bool _dosysts;
        int _nbin;
        double _lumi;
        std::string _type;
        std::string _plotname;
        std::string _outdir;
        TCanvas* _canv;
        THStack* _stackplot;
        TLegend* _legend;
        std::vector<ModifiedTH1<TH1D>*> _signalcollection;
        std::vector<TH1D*> _backgroundcollection;
        PairVector _systerror;
        PairVector _totalcontent;
        PairVector _totalrcontent;
        //private function
        void GetTotalMCError(bool addsysts);
        void end();

};


#endif 
