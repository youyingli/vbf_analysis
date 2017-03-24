#ifndef __ZPLUSJETSVALIDSELECTOR__
#define __ZPLUSJETSVALIDSELECTOR__

#include <string>

#include "TFile.h"
#include "TTree.h"

class ZPlusJetsValidSelector{

    public:
        ZPlusJetsValidSelector( std::string infilename, std::string samepletype, std::string sampleName, std::string outdir );
        ~ZPlusJetsValidSelector();
        void selectLoop( std::string level, std::string state );
        void setLumi(double lumi = 1);
        void setEventWeight( double wgt, bool isGenWeight = false );
        void setSystLabel( std::string SystLabel );
        double GetSBenents();
        std::string GetOutFileName();


    private:

        TFile* infile_;
        TTree* inTree_;
        std::string infilename_;
        std::string sampleType_;
        std::string sampleName_;
        std::string SystLabel_;
        std::string outdir_;

        double eventWeight_;
        double lumi_;
        bool isGenWeight_;

        void Initialization();
        Float_t dipho_mass              ; 
        Float_t dipho_leadPt            ; 
        Float_t dipho_leadEta           ; 
        Float_t dipho_leadPhi           ; 
        Float_t dipho_leadEt            ; 
        Float_t dipho_lead_ptoM         ; 
        Float_t dipho_leadIDMVA         ; 
        Float_t dipho_subleadPt         ; 
        Float_t dipho_subleadEta        ; 
        Float_t dipho_subleadPhi        ; 
        Float_t dipho_subleadEt         ; 
        Float_t dipho_sublead_ptoM      ;
        Float_t dipho_subleadIDMVA      ;
        Float_t dijet_Mjj               ; 
        Float_t dijet_LeadJPt           ; 
        Float_t dijet_leadEta           ; 
        Float_t dijet_SubJPt            ; 
        Float_t dijet_subleadEta        ; 
        Float_t dijet_abs_dEta          ; 
        Float_t dijet_dphi              ; 
        Float_t dijet_Zep               ; 
        Float_t dijet_dipho_dphi_trunc  ;
        Float_t dijet_minDRJetPho       ;
        Float_t dijet_centrality_gg     ;
        Float_t dijet_mva               ; 
        Float_t dipho_mva               ; 
        Float_t dipho_dijet_MVA         ; 
        Int_t nvtx                      ; 
        Float_t puweight                ; 
        Float_t weight                  ;

        Float_t dijet_jet1_match        ;
        Float_t dijet_jet2_match        ;

        double sbevents_;
        std::string outfilename_;


};

#endif
