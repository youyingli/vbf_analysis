#ifndef __ZPLUSJETSVALIDSELECTOR__
#define __ZPLUSJETSVALIDSELECTOR__

#include <string>

#include "TFile.h"
#include "TTree.h"

class ZPlusJetsValidSelector{

    public:
        ZPlusJetsValidSelector(const std::string& indir, const std::string& infilename, const std::string& samplename, const std::string& outdir);
        ~ZPlusJetsValidSelector();
        void SelectLoop(const std::string& state = "");
        void SetSystLabel(const std::string& systlabel = "");

    private:

        TFile* _infile;
        TTree* _intree;
        std::string _indir;
        std::string _infilename;
        std::string _samplename;
        std::string _systlabel;
        std::string _outdir;

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

};

#endif
