import FWCore.ParameterSet.Config as cms
from vbf_analysis.Selector.SystematicsLabels import jetsystlabels 

process = cms.PSet()

process.GlobalSet = cms.PSet(

    indir = cms.string('/wk_cms2/youying/latest/'),    
    isSideBand = cms.bool(True),
    MHmin = cms.double(120.),
    MHmax = cms.double(130.)
       
)

process.SigMCsamples = cms.VPSet(
    #VBF    
#    cms.PSet(  Name = cms.string('vbf_120'),  File = cms.string('VBFHToGG_M120_13TeV_amcatnlo_pythia8')  ),
#    cms.PSet(  Name = cms.string('vbf_123'),  File = cms.string('VBFHToGG_M123_13TeV_amcatnlo_pythia8')  ),
#    cms.PSet(  Name = cms.string('vbf_124'),  File = cms.string('VBFHToGG_M124_13TeV_amcatnlo_pythia8')  ),
    cms.PSet(  Name = cms.string('vbf_125'),  File = cms.string('VBFHToGG_M125_13TeV_amcatnlo_pythia8')  ),
#    cms.PSet(  Name = cms.string('vbf_126'),  File = cms.string('VBFHToGG_M126_13TeV_amcatnlo_pythia8')  ),
#    cms.PSet(  Name = cms.string('vbf_127'),  File = cms.string('VBFHToGG_M127_13TeV_amcatnlo_pythia8')  ),
#    cms.PSet(  Name = cms.string('vbf_130'),  File = cms.string('VBFHToGG_M130_13TeV_amcatnlo_pythia8')  ),

    #GGF
#    cms.PSet(  Name = cms.string('ggh_120'),  File = cms.string('GluGluHToGG_M120_13TeV_amcatnloFXFX_pythia8')  ),
#    cms.PSet(  Name = cms.string('ggh_123'),  File = cms.string('GluGluHToGG_M123_13TeV_amcatnloFXFX_pythia8')  ),
#    cms.PSet(  Name = cms.string('ggh_124'),  File = cms.string('GluGluHToGG_M124_13TeV_amcatnloFXFX_pythia8')  ),
#    cms.PSet(  Name = cms.string('ggh_125'),  File = cms.string('GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8')  ),
    cms.PSet(  Name = cms.string('ggh_125'),  File = cms.string('GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8')  )
#    cms.PSet(  Name = cms.string('ggh_126'),  File = cms.string('GluGluHToGG_M126_13TeV_amcatnloFXFX_pythia8')  ),
#    cms.PSet(  Name = cms.string('ggh_127'),  File = cms.string('GluGluHToGG_M127_13TeV_amcatnloFXFX_pythia8')  ),
#    cms.PSet(  Name = cms.string('ggh_130'),  File = cms.string('GluGluHToGG_M130_13TeV_amcatnloFXFX_pythia8')  ),

    #ZH
    #cms.PSet(  Name = cms.string('zh_120'),  File = cms.string('ZHToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('zh_123'),  File = cms.string('ZHToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('zh_124'),  File = cms.string('ZHToGG_M124_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('zh_125'),  File = cms.string('ZHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('zh_126'),  File = cms.string('ZHToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('zh_127'),  File = cms.string('ZHToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('zh_130'),  File = cms.string('ZHToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8')  ),

    ##WH
    #cms.PSet(  Name = cms.string('wh_120'),  File = cms.string('WHToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('wh_123'),  File = cms.string('WHToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('wh_124'),  File = cms.string('WHToGG_M124_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('wh_125'),  File = cms.string('WHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('wh_126'),  File = cms.string('WHToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('wh_127'),  File = cms.string('WHToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('wh_130'),  File = cms.string('WHToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8')  ),

    ##TTH
    #cms.PSet(  Name = cms.string('tth_120'),  File = cms.string('ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('tth_123'),  File = cms.string('ttHJetToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('tth_124'),  File = cms.string('ttHJetToGG_M124_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('tth_125'),  File = cms.string('ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('tth_126'),  File = cms.string('ttHJetToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('tth_127'),  File = cms.string('ttHJetToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8')  ),
    #cms.PSet(  Name = cms.string('tth_130'),  File = cms.string('ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8')  ),
        
)

process.BkgMCsamples = cms.VPSet(
    #promptprompt (Dipho)
    cms.PSet(  Name = cms.string('dipho'),  File = cms.string('DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa')  ),

    #promptfake (GJ, QCD)
    cms.PSet(  Name = cms.string('gjet_promptfake'),  File = cms.string('GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8_promptfake' )  ),
    cms.PSet(  Name = cms.string('gjet_promptfake'),  File = cms.string('GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_promptfake' )  ),
    cms.PSet(  Name = cms.string('gjet_promptfake'),  File = cms.string('GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_promptfake')  ),
    cms.PSet(  Name = cms.string('qcd_promptfake'),  File = cms.string('QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8_promptfake'  )  ),
    cms.PSet(  Name = cms.string('qcd_promptfake'),  File = cms.string('QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_promptfake'  )  ),
    cms.PSet(  Name = cms.string('qcd_promptfake'),  File = cms.string('QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_promptfake' )  ),
    
    #fakefake (GJ, QCD)
    cms.PSet(  Name = cms.string('gjet_fakefake'),  File = cms.string('GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8_fakefake' )  ),
    cms.PSet(  Name = cms.string('gjet_fakefake'),  File = cms.string('GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_fakefake' )  ),
    cms.PSet(  Name = cms.string('gjet_fakefake'),  File = cms.string('GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_fakefake')  ),
    cms.PSet(  Name = cms.string('qcd_fakefake'),  File = cms.string('QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8_fakefake'  )  ),
    cms.PSet(  Name = cms.string('qcd_fakefake'),  File = cms.string('QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_fakefake'  )  ),
    cms.PSet(  Name = cms.string('qcd_fakefake'),  File = cms.string('QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_fakefake' )  ),

    #DYjets                   
    cms.PSet(  Name = cms.string('DYjets'),  File = cms.string('DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8')  )

)

process.data = cms.VPSet(
    
    cms.PSet(  Name = cms.string('Data'),  File = cms.string('DoubleEG')  )
    #cms.PSet(  Name = cms.string('Data'),  File = cms.string('DoubleEG_RunBCD')  ),
    #cms.PSet(  Name = cms.string('Data'),  File = cms.string('DoubleEG_RunEFGH')  )
        
)

process.Systematics = cms.PSet(
        
       dumpSysts = cms.bool(True),     
       SystLabels = cms.vstring(jetsystlabels)
        
)

