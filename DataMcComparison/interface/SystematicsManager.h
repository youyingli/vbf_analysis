#ifndef __SYSTEMATICSMANAGER__
#define __SYSTEMATICSMANAGER__

#include <string>
#include <vector>


class SystematicsManager{


    public:
        SystematicsManager( std::string plotname );
        ~SystematicsManager();
        void SetMcCollection( std::vector<std::string> Collection );
        void AddSystLabel(std::string SystLabel);
        void ErrorPropagator(double scale);
        

        std::vector<double> GetSystUpError(){ return UpError_; }
        std::vector<double> GetSystDownError(){ return DownError_; }



    private:
        std::string plotname_;
        std::vector<double> UpError_;
        std::vector<double> DownError_;
        std::vector<std::string> Collection_;
        std::vector<std::string> SystLabels_;

};

#endif
