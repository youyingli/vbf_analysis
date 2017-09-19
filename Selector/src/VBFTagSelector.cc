#include "TH1F.h"
#include "TLorentzVector.h"

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooWorkspace.h"

#include "vbf_analysis/Selector/interface/VBFTagSelector.h"
#include "vbf_analysis/Utils/interface/PlotMgrLib.h"


#include <iostream>
using namespace std;

VBFTagSelector::VBFTagSelector (const string& indir, const string& infilename, const string& samplename, const string& outdir):
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

VBFTagSelector::~VBFTagSelector ()
{
    _infile->Close();
}

void VBFTagSelector::Initialization ()
{
    cout << "[INFO]: Initializing  " << _infilename + ".root" << "  ..." << endl;
    _intree = (TTree*)_infile->Get(Form("tagsDumper/trees/%s_13TeV_VBFTag%s" ,_samplename.c_str() ,_systlabel.c_str()));
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
    _intree->SetBranchAddress("dijet_centrality_gg"     ,&dijet_centrality_gg      );
    _intree->SetBranchAddress("dijet_minDRJetPho"       ,&dijet_minDRJetPho        );
    _intree->SetBranchAddress("dijet_mva"               ,&dijet_mva                );
    _intree->SetBranchAddress("dipho_mva"               ,&dipho_mva                );
    _intree->SetBranchAddress("dipho_dijet_MVA"         ,&dipho_dijet_MVA          );
    _intree->SetBranchAddress("nvtx"                    ,&nvtx                     );
    _intree->SetBranchAddress("puweight"                ,&puweight                 );
    _intree->SetBranchAddress("weight"                  ,&weight                   );
    _intree->SetBranchAddress("jet1_pt"                 ,&jet1_pt                  );
    _intree->SetBranchAddress("jet1_eta"                ,&jet1_eta                 );
    _intree->SetBranchAddress("jet1_Phi"                ,&jet1_Phi                 );
    _intree->SetBranchAddress("jet1_mass"               ,&jet1_mass                );
    _intree->SetBranchAddress("jet2_pt"                 ,&jet2_pt                  );
    _intree->SetBranchAddress("jet2_eta"                ,&jet2_eta                 );
    _intree->SetBranchAddress("jet2_Phi"                ,&jet2_Phi                 );
    _intree->SetBranchAddress("jet2_mass"               ,&jet2_mass                );
    _intree->SetBranchAddress("jet3_pt"                 ,&jet3_pt                  );
    _intree->SetBranchAddress("jet3_eta"                ,&jet3_eta                 );
    _intree->SetBranchAddress("jet3_Phi"                ,&jet3_Phi                 );
    _intree->SetBranchAddress("jet3_mass"               ,&jet3_mass                );
    //...

    cout << "[INFO]: Initialization is finished." << endl;
}


void VBFTagSelector::selectLoop (bool onlysideband)
{
    Initialization();

    TFile* outfile = TFile::Open(Form("%s/output_%s.root", _outdir.c_str(), (_infilename + _systlabel).c_str()), "recreate");
    
    //Produce branches
    TTree* outTree_MVA = new TTree("MVA_variables",""); 
    outTree_MVA->Branch("dijet_LeadJPt"          ,&_dijet_LeadJPt            ,"dijet_LeadJPt/F"           );
    outTree_MVA->Branch("dijet_SubJPt"           ,&_dijet_SubJPt             ,"dijet_SubJPt/F"            );
    outTree_MVA->Branch("dijet_abs_dEta"         ,&_dijet_abs_dEta           ,"dijet_abs_dEta/F"          );
    outTree_MVA->Branch("dijet_Mjj"              ,&_dijet_Mjj                ,"dijet_Mjj/F"               );
    outTree_MVA->Branch("dijet_centrality_gg"    ,&_dijet_centrality_gg      ,"dijet_centrality_gg/F"     );
    outTree_MVA->Branch("dijet_dipho_dphi_trunc" ,&_dijet_dipho_dphi_trunc   ,"dijet_dipho_dphi_trunc/F"  );
    outTree_MVA->Branch("dijet_dphi"             ,&_dijet_dphi               ,"dijet_dphi/F"              );
    outTree_MVA->Branch("dijet_minDRJetPho"      ,&_dijet_minDRJetPho        ,"dijet_minDRJetPho/F"       );
    outTree_MVA->Branch("dipho_lead_ptoM"        ,&_dipho_lead_ptoM          ,"dipho_lead_ptoM/F"         );
    outTree_MVA->Branch("dipho_sublead_ptoM"     ,&_dipho_sublead_ptoM       ,"dipho_sublead_ptoM/F"      );
    outTree_MVA->Branch("dipho_mva"              ,&_dipho_mva                ,"dipho_mva/F"               );
    outTree_MVA->Branch("dipho_dijet_MVA"        ,&_dipho_dijet_MVA          ,"dipho_dijet_MVA/F"         );

    //Producr workspace for roofit
    RooWorkspace* ws = new RooWorkspace("ws");
    RooRealVar CMS_hgg_mass("CMS_hgg_mass", "CMS_hgg_mass", 100., 180.);
    RooRealVar Dijet_BDT("Dijet_BDT", "Dijet_BDT", 0.2, 1.0);
    RooRealVar Weight("weight", "weight", -10000., 10000.);
    RooDataSet dataset("dataset", "dataset", RooArgSet(CMS_hgg_mass, Dijet_BDT, Weight), RooFit::WeightVar("weight"));
    ws->import(CMS_hgg_mass);
    ws->import(Dijet_BDT);
    ws->import(Weight);

    //Produce plots
    plotmgr::TH1Service<TH1F> th1service;
    th1service.AddNewTH1("h1_Mgg"                     ,80  ,100. ,180.   );    
    th1service.AddNewTH1("h1_lead_phoPt"              ,30  ,20.  ,260.   );    
    th1service.AddNewTH1("h1_lead_phoEta"             ,30  ,-3.  ,3.     );    
    th1service.AddNewTH1("h1_lead_phoId_mva"          ,40  ,-1.  ,1.     );    
    th1service.AddNewTH1("h1_leadPho_PToM"            ,25  ,0.   ,2.5    );    
    th1service.AddNewTH1("h1_subl_phoPt"              ,30  ,20.  ,140.   );    
    th1service.AddNewTH1("h1_subl_phoEta"             ,30  ,-3.  ,3.     );    
    th1service.AddNewTH1("h1_subl_phoId_mva"          ,40  ,-1.  ,1.     );    
    th1service.AddNewTH1("h1_sublPho_PToM"            ,25  ,0.2  ,1.     );    
    th1service.AddNewTH1("h1_dipho_dEta"              ,40  ,0.   ,3.     );    
    th1service.AddNewTH1("h1_dipho_dphi"              ,40  ,0.   ,3.2    );    
    th1service.AddNewTH1("h1_Mjj"                     ,30  ,150. ,3500.  );    
    th1service.AddNewTH1("h1_lead_jetPt"              ,30  ,20.  ,290.   );    
    th1service.AddNewTH1("h1_lead_jetEta"             ,40  ,-5.  ,5.     );    
    th1service.AddNewTH1("h1_subl_jetPt"              ,30  ,10.  ,160.   );    
    th1service.AddNewTH1("h1_subl_jetEta"             ,40  ,-5.  ,5.     );    
    th1service.AddNewTH1("h1_dijet_dEta"              ,30  ,2.   ,8.     );    
    th1service.AddNewTH1("h1_dijet_dPhi"              ,30  ,0.   ,3.3    );    
    th1service.AddNewTH1("h1_dijet_Zep"               ,25  ,0.   ,8.     );    
    th1service.AddNewTH1("h1_dijet_dipho_dphi_trunc"  ,30  ,0.   ,3.     );    
    th1service.AddNewTH1("h1_dijet_centrality_gg"     ,25  ,0.   ,1.     );    
    th1service.AddNewTH1("h1_dijet_minDRJetPho"       ,40  ,0.   ,5.     );    
    th1service.AddNewTH1("h1_dipho_mva"               ,40  ,-1.  ,1.     );    
    th1service.AddNewTH1("h1_dijet_mva"               ,25  ,0.2  ,1.     );    
    th1service.AddNewTH1("h1_dipho_dijet_mva"         ,40  ,-0.2 ,1.     );    
    th1service.AddNewTH1("h1_nvtx"                    ,40  ,0.   ,40.    );    
    th1service.AddNewTH1("h1_dipho_dijet_dPt"         ,40  ,-150.,150.   );    
    th1service.AddNewTH1("h1_dipho_massn"             ,40  ,100. ,180.   );    
    th1service.AddNewTH1("h1_dipho_massp"             ,40  ,100. ,180.   );    

    //Loop start
    cout << "[INFO]: Start dumping " << _infilename + ".root" << endl;
    for (int entry=0; entry < _intree->GetEntries(); entry++) {
        _intree->GetEntry(entry);

//        bool selections = (   dipho_lead_ptoM > (1./4) && dipho_sublead_ptoM > (1./5)
//                           && dijet_LeadJPt > 30.      && dijet_SubJPt > 20.       
//                           && dijet_Mjj > 600.         &&(dipho_mass > 100. && dipho_mass < 180.)
//                          );

        bool selections = (   dipho_lead_ptoM>(1./3) && dipho_sublead_ptoM>(1./4)
                           && dipho_leadIDMVA>0.     && dipho_subleadIDMVA>0.
                           && dijet_LeadJPt>30.      && dijet_SubJPt>20.
                           && fabs(dijet_leadEta)<4.7&& fabs(dijet_subleadEta)<4.7
                           && dijet_Mjj> 150.        && dijet_abs_dEta>2.
                           && dijet_mva>0.2          && (dipho_mass>100. && dipho_mass<180.)
                          );

        if ( !selections ) continue;

        //diPhoton P4   
        TLorentzVector leadphotonP4, subleadphotonP4, diphotonP4;
        leadphotonP4.SetPtEtaPhiE(    dipho_leadPt    , dipho_leadEta    , dipho_leadPhi    , dipho_leadEt   *cosh(dipho_leadEta));
        subleadphotonP4.SetPtEtaPhiE( dipho_subleadPt , dipho_subleadEta , dipho_subleadPhi , dipho_subleadEt*cosh(dipho_subleadEta));
        diphotonP4 = leadphotonP4 + subleadphotonP4;
        //dijet(trijet) P4
        TLorentzVector leadjetP4, subleadjetP4, thirdjetP4, dijetP4, trijetP4;
        leadjetP4.SetPtEtaPhiM   (jet1_pt , jet1_eta , jet1_Phi , jet1_mass);
        subleadjetP4.SetPtEtaPhiM(jet2_pt , jet2_eta , jet2_Phi , jet2_mass);
        thirdjetP4.SetPtEtaPhiM  (jet3_pt , jet3_eta , jet3_Phi , jet3_mass);
        dijetP4 = leadjetP4 + subleadjetP4;
        trijetP4 = leadjetP4 + subleadjetP4 + thirdjetP4;

        double dipho_dEta = fabs(dipho_leadEta - dipho_subleadEta);
        double dipho_dphi = fabs(dipho_leadPhi - dipho_subleadPhi);
        double dipho_dijet_dPt = trijetP4.Pt() - diphotonP4.Pt();

        //Fill values into the new branches
        _dijet_LeadJPt           =  dijet_LeadJPt           ; 
        _dijet_SubJPt            =  dijet_SubJPt            ; 
        _dijet_abs_dEta          =  dijet_abs_dEta          ; 
        _dijet_Mjj               =  dijet_Mjj               ; 
        _dijet_centrality_gg     =  dijet_centrality_gg     ; 
        _dijet_dipho_dphi_trunc  =  dijet_dipho_dphi_trunc  ; 
        _dijet_dphi              =  dijet_dphi              ; 
        _dijet_minDRJetPho       =  dijet_minDRJetPho       ; 
        _dipho_lead_ptoM         =  dipho_lead_ptoM         ; 
        _dipho_sublead_ptoM      =  dipho_sublead_ptoM      ; 
        _dipho_mva               =  dipho_mva               ;
        _dipho_dijet_MVA         =  dipho_dijet_MVA         ;             
        outTree_MVA->Fill();

        //Fill values into dataset
        CMS_hgg_mass.setVal(dipho_mass);
        Dijet_BDT.setVal(dijet_mva);
        Weight.setVal(weight);
        dataset.add(RooArgSet(CMS_hgg_mass, Dijet_BDT, Weight), Weight.getVal());

        bool isSig = (_samplename.find("h_") != string::npos) || (_samplename.find("vbf_") != string::npos);
        if (!isSig && onlysideband)
           if (dipho_mass > _min && dipho_mass < _max)
              continue;

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
        th1service.GetPlot("h1_dipho_dijet_dPt"          )->FillEvent(dipho_dijet_dPt          ,weight); 

    }//entry++
    ws->import(dataset);
    ws->Write();
    outfile->Write();

    cout << "[INFO]: Dumping of " << _infilename << ".root is finished !!!" << endl;
    cout << "[INFO]: Output file " << "output_" + _infilename +  _systlabel << ".root is created !!!" << endl;

    th1service.Close ();
    outfile->Close();

}

void VBFTagSelector::SetSystLabel (const string& systlabel)
{
    _systlabel = systlabel != "" ? "_" + systlabel : "";
}

void VBFTagSelector::SetSignalRegion (double min, double max)
{
    _min = min;
    _max = max;
}
