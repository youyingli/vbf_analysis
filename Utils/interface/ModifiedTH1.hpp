#ifndef __MODIFIEDTH1_HPP__
#define __MODIFIEDTH1_HPP__

#include <string>
#include <vector>
#include <utility>

#include "TH1.h"
#include "TH1F.h"
#include "TH1D.h"

template <typename T>
class ModifiedTH1 {

    public:
        ModifiedTH1 ( std::string name, int nbin, double min, double max );
        ModifiedTH1 ( T* plot );
        ModifiedTH1 ( const ModifiedTH1& modifiedTH1 );
        ~ModifiedTH1 ();

        void ResetArtStyle ();
        void ConvertToLinePlot ( Color_t color, Style_t style );
        void ConvertToFillPlot ( Color_t color, Style_t style );
        void ConvertToPointPlot ( Color_t color, Style_t style, Size_t size );
        void ConvertToBoxErrorPlot ( Color_t color, double transparency );
        void Draw( std::string DrawOption );
        void SetScaleWeight ( double scaleweight );
        void SetXYaxis ( std::string xLabel, std::string yLabel, std::string invisible = "" );
        void FillEvent ( double value, double weight );
        void SetYaxisRange ( double ymin, double ymax );
        void WriteInFile ();
        void AddPlot ( T* plot );
        void Reset ();
        void SetBinContent ( std::vector<std::pair<double,double>> contentset );


        T* GetObject () { return plot_; }
        const double GetWeightEventN ( double min, double max );
        const double GetEventN ();
        const int GetNbinsX () { return nbin_; }
        const double GetMaxContent () { return plot_ -> GetMaximum(); }
        std::vector<std::pair<double,double>> GetBinContent ();
        std::pair<double,double> GetXRange () { return std::make_pair(plot_->GetXaxis()-> GetXmin(),plot_->GetXaxis()-> GetXmax()); }
    private:
        std::string name_;
        double nbin_;
        double min_;
        double max_;
        T* plot_;

};

#include "vbf_analysis/Utils/src/ModifiedTH1.ipp"

#endif
