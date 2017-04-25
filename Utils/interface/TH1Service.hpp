#ifndef __TH1SERVICE_HPP__
#define __TH1SERVICE_HPP__

#include <string>
#include <vector>
#include <map>

#include "TFile.h"
#include "vbf_analysis/Utils/interface/ModifiedTH1.hpp"

template <typename TH1Type>
class TH1Service {

    public:
        TH1Service();
        ~TH1Service();
        void AddNewTH1 ( std::string name, int nbin, double min, double max );
        void AddPlotFromFile ( std::string name, std::string plotname, std::string filename );
        void Delete (std::string name);
        void Close ();
        ModifiedTH1<TH1Type>* GetPlot ( std::string name ) { return th1set_[name]; } 

    private:
        bool isfile_;
        std::map<std::string,ModifiedTH1<TH1Type>*> th1set_;
        std::vector<TFile*> fileset_;
};

#include "vbf_analysis/Utils/src/TH1Service.ipp"

#endif
