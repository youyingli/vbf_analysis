import FWCore.ParameterSet.Config as cms

inFilePath = '/wk_cms/youying/CMSSW_8_0_26_patch1/src/VBFTag/'

process = cms.PSet()

process.GlobalSet = cms.PSet(
        
    ControlPlot = cms.string('h1_Mgg'),
    isSideBand = cms.bool(True),
    isLogScale = cms.bool(False)

)

process.PlotInfo = cms.VPSet(
        
    cms.PSet( PlotName = cms.string('h1_Mgg'                   ),  XaxisName = cms.string('M_{ee} (GeV)'          )    ), 
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
    cms.PSet( PlotName = cms.string('h1_nvtx'                  ),  XaxisName = cms.string('Nvtx'                            )    ) 

)

process.DYjetsSamples = cms.PSet(

    Total = cms.string(inFilePath + 'output_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ZPlusJets.root'),
    Jet1Fake  = cms.string(inFilePath + 'output_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ZPlusJets_jet1fake.root'),
    Jet2Fake  = cms.string(inFilePath + 'output_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ZPlusJets_jet2fake.root'),
    BothFake  = cms.string(inFilePath + 'output_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ZPlusJets_bothfake.root'),
    BothMatch = cms.string(inFilePath + 'output_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ZPlusJets_bothmatch.root')

)

process.Data = cms.PSet(
        
    File = cms.string(inFilePath + 'output_DoubleEG_ZPlusJets.root'),
    Lumi = cms.double(35.87)
        
)

process.Systematics = cms.PSet(
    
    AddSyst = cms.bool(True),
    SystLabel = cms.vstring('JEC','JER') 
     
)
