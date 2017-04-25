#ifndef __SCALEFACTORCALCULATOR__
#define __SCALEFACTORCALCULATOR__

#include <vector>
#include <utility>

#include "TH1D.h"
#include "vbf_analysis/Utils/interface/TH1Service.hpp"
class ScaleFactorCalculator{

    public:
        ScaleFactorCalculator(std::string controlplot);
        ~ScaleFactorCalculator();
        void SetRemoveRange(double min, double max);
        void SetKeepRange(double min, double max);
        void SetMCSamples(std::vector<std::string> infiles);
        void SetData(std::string infile);
        void SetExtraWeight(double weight);
        double GetScaleFactor();


    private:
        double RemoveEvents;
        double weight_;
        std::string controlplot_;
        TH1Service<TH1D> th1service_;
        std::vector<std::pair<double,double>> RemoveRange_;
        std::pair<double,double> KeepRange_;

};

#endif 
