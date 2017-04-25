#ifndef __SYSTEMATICSMANAGER__
#define __SYSTEMATICSMANAGER__

#include "vbf_analysis/Utils/interface/TH1Service.hpp"

#include <string>
#include <vector>
#include <utility>

class SystematicsManager{

    typedef std::vector<std::pair<double,double>> PairVector;

    public:
        SystematicsManager( std::string plotname );
        ~SystematicsManager();
        void SetMcCollection( std::vector<std::string> Collection );
        void AddSystLabel(std::string SystLabel);
        void ErrorPropagator(double scale);
        
        PairVector GetSystError(){ return SystError; }

    private:
        std::string plotname_;
        std::vector<std::string> Collection_;
        std::vector<std::string> SystLabels_;
        PairVector SystError;

};

#endif
