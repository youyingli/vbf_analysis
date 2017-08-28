#ifndef __SCALEFACTORCALCULATOR__
#define __SCALEFACTORCALCULATOR__

#include <vector>
#include <utility>

#include "TH1D.h"
#include "vbf_analysis/Utils/interface/plotmgrlib.h"


class ScaleFactorCalculator{

    public:
        ScaleFactorCalculator (const std::string& controlplot);
        ~ScaleFactorCalculator ();
        void SetRemoveRange (double min, double max);
        void SetKeepRange (double min, double max);
        void SetMCSamples (const std::vector<std::string>& infiles);
        void SetData (const std::string& infile);
        void SetExtraWeight (double weight = 1.);
        double GetScaleFactor ();


    private:
        double _weight;
        std::string _controlplot;
        plotmgr::TH1Service<TH1D> _th1service;
        std::vector<std::pair<double,double>> _RemoveRange;
        std::pair<double,double> _KeepRange;

};

#endif 
