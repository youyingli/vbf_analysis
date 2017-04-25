import FWCore.ParameterSet.Config as cms
from vbf_analysis.Selector.SystematicsLabels import jetsystlabels 

process = cms.PSet()

process.GlobalSet = cms.PSet(

    indir = cms.string('/wk_cms2/youying/ZPlusJets/')
       
)

process.DYjetsSample = cms.PSet(

    Name = cms.string('DYjets'),  
    File = cms.string('DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ZPlusJets'),
    jetstate = cms.vstring('','jet1fake','jet2fake','bothfake','bothmatch')

)

process.data = cms.VPSet(
    
    cms.PSet(  Name = cms.string('Data'),  File = cms.string('DoubleEG_ZPlusJets')  )
        
)

process.Systematics = cms.PSet(
        
    dumpSysts = cms.bool(True),     
    SystLabels = cms.vstring(jetsystlabels)
        
)

