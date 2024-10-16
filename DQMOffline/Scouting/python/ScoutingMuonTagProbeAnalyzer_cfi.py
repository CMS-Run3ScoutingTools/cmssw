import FWCore.ParameterSet.Config as cms
from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer

ScoutingMuonTagProbeAnalysis = DQMEDAnalyzer('ScoutingMuonTagProbeAnalyzer',

    OutputInternalPath = cms.string('ScoutingMonitoring'),
    MuonCollection = cms.InputTag('slimmedMuons'),
    ScoutingMuonCollection = cms.InputTag('hltScoutingMuonPackerNoVtx')

)

scoutingMonitoringTagProbeMuon = cms.Sequence(ScoutingMuonTagProbeAnalysis)
