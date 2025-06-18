import FWCore.ParameterSet.Config as cms


def addNanoEDMOutputModule(process):

    outputfilename = process.NANOAODoutput.fileName.value()
    outputfilename = outputfilename.replace('.root', '_edm.root')

    process.NanoAODEDMoutput = cms.OutputModule("PoolOutputModule",
        compressionAlgorithm = cms.untracked.string('LZMA'),
        compressionLevel = cms.untracked.int32(9),
        dataset = cms.untracked.PSet(
            dataTier = cms.untracked.string('NANOAODEDM'),
            filterName = cms.untracked.string('')
        ),
        fileName = cms.untracked.string(outputfilename),
        outputCommands = process.NanoAODEDMEventContent.outputCommands
    )

    process.NANOAODoutput_step = cms.EndPath(process.NANOAODoutput + process.NanoAODEDMoutput)

    return process
