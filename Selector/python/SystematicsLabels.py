#Systematics Labels

systlabels = []
jetsystlabels = []
phosystlabels = []

for direction in ['Up','Down']:
#    phosystlabels.append('MvaShift%s01sigma' % direction)
##    phosystlabels.append('MvaLinearSyst%s01sigma' % direction)
#    phosystlabels.append('SigmaEOverEShift%s01sigma' % direction)
#    phosystlabels.append("MaterialCentralBarrel%s01sigma" % direction)
#    phosystlabels.append("MaterialOuterBarrel%s01sigma" % direction)
#    phosystlabels.append('MaterialForward%s01sigma' % direction)
#    phosystlabels.append('FNUFEB%s01sigma' % direction)
#    phosystlabels.append('FNUFEE%s01sigma' % direction)
#    phosystlabels.append('MCScaleGain6EB%s01sigma' % direction)
#    phosystlabels.append('MCScaleGain1EB%s01sigma' % direction)
    jetsystlabels.append('JEC%s01sigma' % direction)
    jetsystlabels.append('JER%s01sigma' % direction)
#    jetsystlabels.append('PUJIDShift%s01sigma' % direction)
#    variablesToUse.append('UnmatchedPUWeight%s01sigma' % (direction))
#    variablesToUse.append('MvaLinearSyst%s01sigma' % (direction))
#    variablesToUse.append('LooseMvaSF%s01sigma' % (direction))
#    variablesToUse.append('PreselSF%s01sigma' % (direction))
#    variablesToUse.append('electronVetoSF%s01sigma' % (direction))
#    variablesToUse.append('TriggerWeight%s01sigma' % (direction))
#    variablesToUse.append('FracRVWeight%s01sigma' % (direction))
#    variablesToUse.append('FracRVNvtxWeight%s01sigma' % (direction))
#    variablesToUse.append('ElectronWeight%s01sigma' % (direction))
#    for r9 in ['HighR9','LowR9']:
#        for region in ['EB','EE']:
#            phosystlabels.append('ShowerShape%s%s%s01sigma'%(r9,region,direction))
##            phosystlabels.append('MCSmear%s%s%s01sigma' % (r9,region,direction))
#            phosystlabels.append('MCScale%s%s%s01sigma' % (r9,region,direction))
#            for var in ['Rho','Phi']:
#                phosystlabels.append('MCSmear%s%s%s%s01sigma' % (r9,region,var,direction))
systlabels += phosystlabels
systlabels += jetsystlabels





