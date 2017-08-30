#include <iostream>

#include "TH1F.h"
#include "TLorentzVector.h"

#include "vbf_analysis/Selector/interface/ZPlusJetsValidSelector.h"
#include "vbf_analysis/Utils/interface/PlotMgrLib.h"


using namespace std;

ZPlusJetsValidSelector::ZPlusJetsValidSelector (const string& indir, const string& infilename, const string& samplename, const string& outdir):
    _indir(indir),
    _infilename(infilename),
    _samplename(samplename),
    _outdir(outdir)
{
    string openfilename = _indir + "/" + _infilename + ".root";
    _infile = TFile::Open(openfilename.c_str());
    if ( !(_infile->IsZombie()) ) cout << "[INFO]: Successfully open " << openfilename << endl;
    else {
        cout << "[ERROR]: Can't find out " << openfilename << endl;
        exit(0);
    }
}

ZPlusJetsValidSelector::~ZPlusJetsValidSelector ()
{
    _infile->Close();
}

void ZPlusJetsValidSelector::Initialization ()
{
    cout << "[INFO]: Initializing  " << _infilename + ".root" << "  ..." << endl;
    _intree = (TTree*)_infile->Get(Form("vbfTagDumper/trees/%s_13TeV_VBFDiJet%s" ,_samplename.c_str(), _systlabel.c_str()));
    _intree->SetBranchAddress("dipho_mass"              ,&dipho_mass               );
    _intree->SetBranchAddress("dipho_leadPt"            ,&dipho_leadPt             );
    _intree->SetBranchAddress("dipho_leadEta"           ,&dipho_leadEta            );
    _intree->SetBranchAddress("dipho_leadPhi"           ,&dipho_leadPhi            );
    _intree->SetBranchAddress("dipho_leadEt"            ,&dipho_leadEt             ); 
    _intree->SetBranchAddress("dipho_lead_ptoM"         ,&dipho_lead_ptoM          );
    _intree->SetBranchAddress("dipho_leadIDMVA"         ,&dipho_leadIDMVA          ); 
    _intree->SetBranchAddress("dipho_subleadPt"         ,&dipho_subleadPt          );
    _intree->SetBranchAddress("dipho_subleadEta"        ,&dipho_subleadEta         );
    _intree->SetBranchAddress("dipho_subleadPhi"        ,&dipho_subleadPhi         );
    _intree->SetBranchAddress("dipho_subleadEt"         ,&dipho_subleadEt          ); 
    _intree->SetBranchAddress("dipho_sublead_ptoM"      ,&dipho_sublead_ptoM       );
    _intree->SetBranchAddress("dipho_subleadIDMVA"      ,&dipho_subleadIDMVA       );   
    _intree->SetBranchAddress("dijet_Mjj"               ,&dijet_Mjj                );
    _intree->SetBranchAddress("dijet_LeadJPt"           ,&dijet_LeadJPt            );
    _intree->SetBranchAddress("dijet_leadEta"           ,&dijet_leadEta            );
    _intree->SetBranchAddress("dijet_SubJPt"            ,&dijet_SubJPt             );
    _intree->SetBranchAddress("dijet_subleadEta"        ,&dijet_subleadEta         );
    _intree->SetBranchAddress("dijet_abs_dEta"          ,&dijet_abs_dEta           );
    _intree->SetBranchAddress("dijet_dphi"              ,&dijet_dphi               );
    _intree->SetBranchAddress("dijet_Zep"               ,&dijet_Zep                );
    _intree->SetBranchAddress("dijet_dipho_dphi_trunc"  ,&dijet_dipho_dphi_trunc   );
    _intree->SetBranchAddress("dijet_minDRJetPho"       ,&dijet_minDRJetPho        );
    _intree->SetBranchAddress("dijet_centrality_gg"     ,&dijet_centrality_gg      );
    _intree->SetBranchAddress("dijet_mva"               ,&dijet_mva                );
    _intree->SetBranchAddress("dipho_mva"               ,&dipho_mva                );
    _intree->SetBranchAddress("dipho_dijet_MVA"         ,&dipho_dijet_MVA          );
    _intree->SetBranchAddress("nvtx"                    ,&nvtx                     );
    _intree->SetBranchAddress("puweight"                ,&puweight                 );
    _intree->SetBranchAddress("weight"                  ,&weight                   );
    if (_samplename != "Data") {
        _intree -> SetBranchAddress("dijet_jet1_match"        ,&dijet_jet1_match         );
        _intree -> SetBranchAddress("dijet_jet2_match"        ,&dijet_jet2_match         );
    }
    cout << "[INFO]: Initialization is finished." << endl;
}


void ZPlusJetsValidSelector::SelectLoop (const string& state)
{
    Initialization();
    string State = state != "" ? "_" + state : "";
    TFile* outfile = TFile::Open(Form("%s/output_%s.root", _outdir.c_str(), (_infilename + State + _systlabel).c_str()), "recreate");

    //Produce branches
    plotmgr::TH1Service<TH1F> th1service;
    th1service.AddNewTH1("h1_Mgg"                     ,80  ,70.  ,110.   );    
    th1service.AddNewTH1("h1_lead_phoPt"              ,30  ,20.  ,260.   );    
    th1service.AddNewTH1("h1_lead_phoEta"             ,30  ,-3.  ,3.     );    
    th1service.AddNewTH1("h1_lead_phoId_mva"          ,40  ,-1.  ,1.     );    
    th1service.AddNewTH1("h1_leadPho_PToM"            ,25  ,0.   ,4.     );    
    th1service.AddNewTH1("h1_subl_phoPt"              ,30  ,20.  ,140.   );    
    th1service.AddNewTH1("h1_subl_phoEta"             ,30  ,-3.  ,3.     );    
    th1service.AddNewTH1("h1_subl_phoId_mva"          ,40  ,-1.  ,1.     );    
    th1service.AddNewTH1("h1_sublPho_PToM"            ,25  ,0.   ,1.5    );    
    th1service.AddNewTH1("h1_dipho_dEta"              ,40  ,0.   ,3.     );    
    th1service.AddNewTH1("h1_dipho_dphi"              ,40  ,0.   ,3.2    );    
    th1service.AddNewTH1("h1_Mjj"                     ,30  ,150. ,3500.  );    
    th1service.AddNewTH1("h1_lead_jetPt"              ,50  ,20.  ,270.   );    
    th1service.AddNewTH1("h1_lead_jetEta"             ,40  ,-5.  ,5.     );    
    th1service.AddNewTH1("h1_subl_jetPt"              ,50  ,20.  ,270.   );    
    th1service.AddNewTH1("h1_subl_jetEta"             ,40  ,-5.  ,5.     );    
    th1service.AddNewTH1("h1_dijet_dEta"              ,40  ,2.   ,8.     );    
    th1service.AddNewTH1("h1_dijet_dPhi"              ,30  ,0.   ,3.3    );    
    th1service.AddNewTH1("h1_dijet_Zep"               ,25  ,0.   ,8.     );    
    th1service.AddNewTH1("h1_dijet_dipho_dphi_trunc"  ,30  ,0.   ,3.     );    
    th1service.AddNewTH1("h1_dijet_centrality_gg"     ,25  ,0.   ,1.     );    
    th1service.AddNewTH1("h1_dijet_minDRJetPho"       ,40  ,0.   ,5.     );    
    th1service.AddNewTH1("h1_dipho_mva"               ,40  ,-1.  ,1.     );    
    th1service.AddNewTH1("h1_dijet_mva"               ,25  ,0.2  ,1.     );    
    th1service.AddNewTH1("h1_dipho_dijet_mva"         ,40  ,-1. ,1.      );    
    th1service.AddNewTH1("h1_nvtx"                    ,40  ,0.   ,40.    );    


    cout << "[INFO]: Start dumping " << _infilename + ".root" << endl;
    for (int entry = 0; entry < _intree->GetEntries(); entry++) {
        _intree->GetEntry(entry);
        
        bool selections =    dipho_lead_ptoM > (1./3) && dipho_sublead_ptoM > (1./4)
                          && dijet_LeadJPt > 40.      && dijet_SubJPt > 30.       
                          && dijet_Mjj > 250.         &&(dipho_mass > 80. && dipho_mass < 100.);

        bool jetcut = true;
        if (_samplename != "Data") {
            if      (State == "_jet1fake")   jetcut = (dijet_jet1_match == 0. && dijet_jet2_match == 1.);
            else if (State == "_jet2fake")   jetcut = (dijet_jet1_match == 1. && dijet_jet2_match == 0.);
            else if (State == "_bothfake")   jetcut = (dijet_jet1_match == 0. && dijet_jet2_match == 0.);
            else if (State == "_bothmatch")  jetcut = (dijet_jet1_match == 1. && dijet_jet2_match == 1.);
        }
        
        if (!(selections && jetcut)) continue;

        double dipho_dEta = fabs(dipho_leadEta - dipho_subleadEta);
        double dipho_dphi = fabs(dipho_leadPhi - dipho_subleadPhi);

        th1service.GetPlot("h1_Mgg"                      )->FillEvent(dipho_mass               ,weight); 
        th1service.GetPlot("h1_lead_phoPt"               )->FillEvent(dipho_leadPt             ,weight); 
        th1service.GetPlot("h1_lead_phoEta"              )->FillEvent(dipho_leadEta            ,weight); 
        th1service.GetPlot("h1_lead_phoId_mva"           )->FillEvent(dipho_leadIDMVA          ,weight); 
        th1service.GetPlot("h1_leadPho_PToM"             )->FillEvent(dipho_lead_ptoM          ,weight); 
        th1service.GetPlot("h1_subl_phoPt"               )->FillEvent(dipho_subleadPt          ,weight); 
        th1service.GetPlot("h1_subl_phoEta"              )->FillEvent(dipho_subleadEta         ,weight); 
        th1service.GetPlot("h1_subl_phoId_mva"           )->FillEvent(dipho_subleadIDMVA       ,weight); 
        th1service.GetPlot("h1_sublPho_PToM"             )->FillEvent(dipho_sublead_ptoM       ,weight); 
        th1service.GetPlot("h1_dipho_dEta"               )->FillEvent(dipho_dEta               ,weight); 
        th1service.GetPlot("h1_dipho_dphi"               )->FillEvent(dipho_dphi               ,weight); 
        th1service.GetPlot("h1_Mjj"                      )->FillEvent(dijet_Mjj                ,weight); 
        th1service.GetPlot("h1_lead_jetPt"               )->FillEvent(dijet_LeadJPt            ,weight); 
        th1service.GetPlot("h1_lead_jetEta"              )->FillEvent(dijet_leadEta            ,weight); 
        th1service.GetPlot("h1_subl_jetPt"               )->FillEvent(dijet_SubJPt             ,weight); 
        th1service.GetPlot("h1_subl_jetEta"              )->FillEvent(dijet_subleadEta         ,weight); 
        th1service.GetPlot("h1_dijet_dEta"               )->FillEvent(dijet_abs_dEta           ,weight); 
        th1service.GetPlot("h1_dijet_dPhi"               )->FillEvent(dijet_dphi               ,weight); 
        th1service.GetPlot("h1_dijet_Zep"                )->FillEvent(dijet_Zep                ,weight); 
        th1service.GetPlot("h1_dijet_dipho_dphi_trunc"   )->FillEvent(dijet_dipho_dphi_trunc   ,weight); 
        th1service.GetPlot("h1_dijet_centrality_gg"      )->FillEvent(dijet_centrality_gg      ,weight); 
        th1service.GetPlot("h1_dijet_minDRJetPho"        )->FillEvent(dijet_minDRJetPho        ,weight); 
        th1service.GetPlot("h1_dipho_mva"                )->FillEvent(dipho_mva                ,weight); 
        th1service.GetPlot("h1_dijet_mva"                )->FillEvent(dijet_mva                ,weight); 
        th1service.GetPlot("h1_dipho_dijet_mva"          )->FillEvent(dipho_dijet_MVA          ,weight); 
        th1service.GetPlot("h1_nvtx"                     )->FillEvent(nvtx                     ,weight); 

    }//entry++
    outfile->Write();

    cout << "[INFO]: Dumping of " << _infilename << ".root is finished !!!" << endl;
    cout << "[INFO]: Output file " << "output_" + _infilename + State + _systlabel << ".root is created !!!" << endl;

    th1service.Close();
    outfile->Close();

}

void ZPlusJetsValidSelector::SetSystLabel (const string& systlabel)
{
    _systlabel = systlabel != "" ? "_" + systlabel : "";
}

