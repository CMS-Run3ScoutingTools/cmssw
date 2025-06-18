def customise_messagelogger(process):
    """
    Customizes the message logger settings for the process.
    """
    # Set the message logger to use the 'NanoScoutMaker' category
    process.MessageLogger.cerr.FwkReport.reportEvery = 100
    # process.MessageLogger.cerr.threshold = 'ERROR'
    # process.MessageLogger.cerr.NanoScoutMaker = cms.untracked.PSet(
    #     limit=cms.untracked.int32(0),
    #     reportEvery=cms.untracked.int32(1000)
    # )
    
    return process
