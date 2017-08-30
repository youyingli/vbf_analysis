#ifndef __SYSTEMATICSMANAGER__
#define __SYSTEMATICSMANAGER__

#include "vbf_analysis/Utils/interface/PlotMgrLib.h"

#include <string>
#include <vector>
#include <utility>

class SystematicsManager{

    typedef std::vector<std::pair<double,double>> PairVector;

    public:
        SystematicsManager(const std::string& plotname);
        ~SystematicsManager();
        void SetMcCollection(std::vector<std::string> collection);
        void AddSystLabel(const std::string& systlabel);
        void ErrorPropagator(double scale);
        
        PairVector GetSystError() const {return _systerror;}

    private:
        std::string _plotname;
        std::vector<std::string> _collection;
        std::vector<std::string> _systlabels;
        PairVector _systerror;

};

#endif
