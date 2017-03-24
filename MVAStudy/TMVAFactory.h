#include <string>

#include "TTree.h"

#include "TMVA/Factory.h"
#include "TMVA/Tools.h"

class TMVAFactory {
    
    public:

        TMVAFactory( std::string name );
        ~TMVAFactory();

        void BookMVA( std::string algorithm );
        void SetSignalSamples( std::vector<std::string> sig );
        void SetBackgroundSamples( std::vector<std::string> bkg );
        void useWeight( bool useWgt );
        void BeginTrainingTest();

    private:
        TFile* outMVAfile_;
        TMVA::Factory *factory_;

        bool useWeight_;
        std::string algorithm_;
        std::string name_;












};
