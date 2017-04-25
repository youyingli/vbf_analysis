import FWCore.ParameterSet.Config as cms

inFilePath = '/wk_cms/youying/CMSSW_8_0_26_patch1/src/VBFTag/'

process = cms.PSet()

process.GlobalSet = cms.PSet(
        
    ControlPlot = cms.string('h1_Mgg'),
    isSideBand = cms.bool(False),
    isLogScale = cms.bool(True)

)

process.PlotInfo = cms.VPSet(
        
    cms.PSet( PlotName = cms.string('h1_Mgg'                   ),  XaxisName = cms.string('M_{#gamma#gamma} (GeV)'          )    ), 
    cms.PSet( PlotName = cms.string('h1_lead_phoPt'            ),  XaxisName = cms.string('Pt_{#gamma 1} (GeV)'             )    ), 
    cms.PSet( PlotName = cms.string('h1_lead_phoEta'           ),  XaxisName = cms.string('#eta_{#gamma 1}'                 )    ), 
    cms.PSet( PlotName = cms.string('h1_lead_phoId_mva'        ),  XaxisName = cms.string('Photon1 ID MVA'                  )    ), 
    cms.PSet( PlotName = cms.string('h1_leadPho_PToM'          ),  XaxisName = cms.string('Pt_{#gamma 1}/M_{#gamma#gamma}'  )    ), 
    cms.PSet( PlotName = cms.string('h1_subl_phoPt'            ),  XaxisName = cms.string('Pt_{#gamma 2} (GeV)'             )    ), 
    cms.PSet( PlotName = cms.string('h1_subl_phoEta'           ),  XaxisName = cms.string('#eta_{#gamma 2}'                 )    ), 
    cms.PSet( PlotName = cms.string('h1_subl_phoId_mva'        ),  XaxisName = cms.string('Photon2 ID MVA'                  )    ), 
    cms.PSet( PlotName = cms.string('h1_sublPho_PToM'          ),  XaxisName = cms.string('Pt_{#gamma 2}/M_{#gamma#gamma}'  )    ), 
    cms.PSet( PlotName = cms.string('h1_dipho_dEta'            ),  XaxisName = cms.string('#Delta#eta(#gamma#gamma)'        )    ), 
    cms.PSet( PlotName = cms.string('h1_dipho_dphi'            ),  XaxisName = cms.string('#Delta#phi(#gamma#gamma)'        )    ),                                 
    cms.PSet( PlotName = cms.string('h1_Mjj'                   ),  XaxisName = cms.string('M_{jj} (GeV)'                    )    ), 
    cms.PSet( PlotName = cms.string('h1_lead_jetPt'            ),  XaxisName = cms.string('Pt_{jet 1} (GeV)'                )    ), 
    cms.PSet( PlotName = cms.string('h1_lead_jetEta'           ),  XaxisName = cms.string('#eta_{jet 1}'                    )    ), 
    cms.PSet( PlotName = cms.string('h1_subl_jetPt'            ),  XaxisName = cms.string('Pt_{jet 2} (GeV)'                )    ), 
    cms.PSet( PlotName = cms.string('h1_subl_jetEta'           ),  XaxisName = cms.string('#eta_{jet 2}'                    )    ), 
    cms.PSet( PlotName = cms.string('h1_dijet_dEta'            ),  XaxisName = cms.string('#Delta#eta(jj)'                  )    ), 
    cms.PSet( PlotName = cms.string('h1_dijet_dPhi'            ),  XaxisName = cms.string('#Delta#phi(jj)'                  )    ), 
    cms.PSet( PlotName = cms.string('h1_dijet_Zep'             ),  XaxisName = cms.string('Zeppenfeld'                      )    ), 
    cms.PSet( PlotName = cms.string('h1_dijet_dipho_dphi_trunc'),  XaxisName = cms.string('#Delta(jj,#gamma#gamma)_{trunc}' )    ), 
    cms.PSet( PlotName = cms.string('h1_dijet_centrality_gg'   ),  XaxisName = cms.string('C_{#gamma#gamma}'                )    ), 
    cms.PSet( PlotName = cms.string('h1_dijet_minDRJetPho'     ),  XaxisName = cms.string('min(#DeltaR(#gamma,jet))'        )    ), 
    cms.PSet( PlotName = cms.string('h1_dipho_mva'             ),  XaxisName = cms.string('diphoton MVA'                    )    ), 
    cms.PSet( PlotName = cms.string('h1_dijet_mva'             ),  XaxisName = cms.string('dijet BDT'                       )    ), 
    cms.PSet( PlotName = cms.string('h1_dipho_dijet_mva'       ),  XaxisName = cms.string('combined MVA'                    )    ),                                 
    cms.PSet( PlotName = cms.string('h1_nvtx'                  ),  XaxisName = cms.string('Nvtx'                            )    ), 
    cms.PSet( PlotName = cms.string('h1_dipho_dijet_dPt'       ),  XaxisName = cms.string('Pt_{jj or jjj}-Pt_{#gamma#gamma}')    ), 
    cms.PSet( PlotName = cms.string('h1_dipho_massn'           ),  XaxisName = cms.string('M_{#gamma#gamma} (GeV)'          )    ), 
    cms.PSet( PlotName = cms.string('h1_dipho_massp'           ),  XaxisName = cms.string('M_{#gamma#gamma} (GeV)'          )    )  

)

process.SigMCSamples = cms.PSet(

    mH = cms.uint32(125),
    VBFFile = cms.string(inFilePath + 'output_VBFHToGG_M125_13TeV_amcatnlo_pythia8.root'),
    GGFFile = cms.string(inFilePath + 'output_GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8.root')
        
)

process.BkgOption = cms.PSet(
        
    useGJ     = cms.bool(True),
    useDipho  = cms.bool(True),
    useQCD    = cms.bool(True),
    useDYjets = cms.bool(False)
 
)

process.BkgMCSamples = cms.PSet(

    GJFile = cms.vstring(inFilePath + 'output_GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8_promptfake.root',
                         inFilePath + 'output_GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8_fakefake.root',
                         inFilePath + 'output_GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_promptfake.root',
                         inFilePath + 'output_GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_fakefake.root',
                         inFilePath + 'output_GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_promptfake.root',
                         inFilePath + 'output_GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_fakefake.root'        
    ),

    DiphoFile = cms.vstring(inFilePath + 'output_DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa.root'),

    QCDFile = cms.vstring(inFilePath + 'output_QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_promptfake.root',
                          inFilePath + 'output_QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_fakefake.root',
                          inFilePath + 'output_QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_promptfake.root',
                          inFilePath + 'output_QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_fakefake.root',
                          inFilePath + 'output_QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8_promptfake.root',
                          inFilePath + 'output_QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8_fakefake.root'
    ),

    DYjetsFile = cms.vstring(inFilePath + 'output_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root')    
             
)

process.Data = cms.PSet(
        
    File = cms.string(inFilePath + 'output_DoubleEG.root'),
    Lumi = cms.double(35.87)
#    File = cms.string(inFilePath + 'output_DoubleEG_RunBCD.root'),
#    Lumi = cms.double(12.609)
#    File = cms.string(inFilePath + 'output_DoubleEG_RunEFGH.root'),
#    Lumi = cms.double(23.261)
        
)

process.Systematics = cms.PSet(
    
    AddSyst = cms.bool(True),
    SystLabel = cms.vstring('JEC','JER') 
     
)
