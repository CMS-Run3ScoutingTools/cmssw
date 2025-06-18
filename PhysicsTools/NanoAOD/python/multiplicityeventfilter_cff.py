import FWCore.ParameterSet.Config as cms


def oneelectron_or_onephoton(process):

    process.MessageLogger.cerr.FwkReport.reportEvery = 10000

    process.singleelectronfilter = cms.EDFilter("FlatTableMultiplicityFilter",
                                                src = cms.InputTag("scoutingElectronTable"),
                                                minN = cms.uint32(1))
    process.singlephotonfilter = cms.EDFilter("FlatTableMultiplicityFilter",
                                            src = cms.InputTag("scoutingPhotonTable"),
                                            minN = cms.uint32(1))
    process.singleelectronfilter_step = cms.Path(process.singleelectronfilter)
    process.singlephotonfilter_step = cms.Path(process.singlephotonfilter)

    process.schedule = cms.Schedule(process.nanoAOD_step,process.singleelectronfilter_step,process.singlephotonfilter_step,process.endjob_step,process.NANOAODoutput_step)

    process.NANOAODoutput.SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('singleelectronfilter_step', 'singlephotonfilter_step')
    )

    return process


def diel_or_dipho(process):

    process.MessageLogger.cerr.FwkReport.reportEvery = 10000

    process.dielefilt = cms.EDFilter("FlatTableMultiplicityFilter",
                                     src = cms.InputTag("scoutingElectronTable"),
                                     minN = cms.uint32(2))
    process.diphofilt = cms.EDFilter("FlatTableMultiplicityFilter",
                                     src = cms.InputTag("scoutingPhotonTable"),
                                     minN = cms.uint32(2))
    process.dielefilt_step = cms.Path(process.dielefilt)
    process.diphofilt_step = cms.Path(process.diphofilt)

    process.schedule = cms.Schedule(process.nanoAOD_step,
                                    process.dielefilt_step,
                                    process.diphofilt_step,
                                    process.endjob_step,
                                    process.NANOAODoutput_step)

    process.NANOAODoutput.SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('dielefilt_step', 'diphofilt_step')
    )

    return process


def diel(process):

    process.MessageLogger.cerr.FwkReport.reportEvery = 10000

    process.dielefilt = cms.EDFilter("FlatTableMultiplicityFilter",
                                     src = cms.InputTag("scoutingElectronTable"),
                                     minN = cms.uint32(2))
    process.dielefilt_step = cms.Path(process.dielefilt)

    process.schedule = cms.Schedule(process.nanoAOD_step,
                                    process.dielefilt_step,
                                    process.endjob_step,
                                    process.NANOAODoutput_step)

    process.NANOAODoutput.SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('dielefilt_step')
    )

    return process


def dipho(process):

    process.MessageLogger.cerr.FwkReport.reportEvery = 10000

    process.diphofilt = cms.EDFilter("FlatTableMultiplicityFilter",
                                     src = cms.InputTag("scoutingPhotonTable"),
                                     minN = cms.uint32(2))
    process.diphofilt_step = cms.Path(process.diphofilt)

    process.schedule = cms.Schedule(process.nanoAOD_step,
                                    process.diphofilt_step,
                                    process.endjob_step,
                                    process.NANOAODoutput_step)

    process.NANOAODoutput.SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('diphofilt_step')
    )

    return process
