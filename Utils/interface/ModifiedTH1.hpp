#ifndef __MODIFIEDTH1_HPP__
#define __MODIFIEDTH1_HPP__

#include <string>
#include <vector>
#include <utility>

#include "TH1.h"
#include "TH1F.h"
#include "TH1D.h"

namespace plotmgr 
{

    template <typename T>
    class ModifiedTH1 {
    
        public:
            ModifiedTH1 (const std::string& name, int nbin, double min, double max);
            ModifiedTH1 (T* plot);
            ModifiedTH1 (const ModifiedTH1& modifiedTH1);
            ~ModifiedTH1 ();
    
            void ResetArtStyle ();
            void ConvertToLinePlot (Color_t color, Style_t style);
            void ConvertToFillPlot (Color_t color, Style_t style);
            void ConvertToPointPlot (Color_t color, Style_t style, Size_t size);
            void ConvertToBoxErrorPlot (Color_t color, double transparency);
            void Draw(const std::string& DrawOption);
            void SetScaleWeight (double scaleweight);
            void SetXYaxis (const std::string& xLabel, const std::string& yLabel, const std::string& invisible = "");
            void FillEvent (double value, double weight);
            void SetYaxisRange (double ymin, double ymax);
            void WriteInFile ();
            void AddPlot (T* plot);
            void Reset ();
            void SetBinContent (const std::vector<std::pair<double,double>>& contentset);
            void NormalizeToOne ();
    
            T* GetObject () const {return _plot;}
            double GetWeightEventN (double min, double max) const;
            double GetEventN () const {return _plot->Integral();}
            double GetBinWidth () const {return _plot->GetXaxis()->GetBinWidth(1);}
            int GetNbinsX () const {return _plot->GetNbinsX();}
            double GetMaxContent () const {return _plot -> GetMaximum();}
            std::vector<std::pair<double,double>> GetBinContent () const;
            std::pair<double,double> GetXRange () const {return std::make_pair(_plot->GetXaxis()->GetXmin(), _plot->GetXaxis()->GetXmax());}
        private:
            T* _plot;
    
            //private function
            void SetCommonAxis();
    };

}

#include "vbf_analysis/Utils/src/ModifiedTH1.ipp"

#endif
