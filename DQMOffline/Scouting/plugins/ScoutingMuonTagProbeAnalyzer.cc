#include "ScoutingMuonTagProbeAnalyzer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

ScoutingMuonTagProbeAnalyzer::ScoutingMuonTagProbeAnalyzer(const edm::ParameterSet& iConfig)
  : outputInternalPath_(iConfig.getParameter<std::string>("OutputInternalPath")),
    muonCollection_(consumes<std::vector<pat::Muon>>(iConfig.getParameter<edm::InputTag>("MuonCollection"))),
    scoutingMuonCollection_(consumes<std::vector<Run3ScoutingMuon>>(iConfig.getParameter<edm::InputTag>("ScoutingMuonCollection"))){
}

ScoutingMuonTagProbeAnalyzer::~ScoutingMuonTagProbeAnalyzer(){
}

void ScoutingMuonTagProbeAnalyzer::dqmAnalyze(edm::Event const& iEvent,
                                                  edm::EventSetup const& iSetup,
                                                  kTagProbeMuonHistos const& histos) const {

  edm::Handle<std::vector<pat::Muon>> patMuons;
  iEvent.getByToken(muonCollection_, patMuons);
  if (patMuons.failedToGet()) {
    edm::LogWarning("ScoutingMonitoring")
        << "pat::Muon collection not found.";
    return;
  }

  edm::Handle<std::vector<Run3ScoutingMuon> > sctMuons;
  iEvent.getByToken(scoutingMuonCollection_, sctMuons);
  if (sctMuons.failedToGet()) {
    edm::LogWarning("ScoutingMonitoring")
        << "Run3ScoutingMuon collection not found.";
    return;
  }

  edm::LogInfo("ScoutingMonitoring")
      << "Process pat::Muons: " << patMuons->size();
  edm::LogInfo("ScoutingMonitoring")
      << "Process Run3ScoutingMuons: " << sctMuons->size();

  /*
  for (const auto& sct_el : *sctEls){
    if (!scoutingMuonID(sct_el)) continue;
    edm::LogInfo("ScoutingMuonTagProbeAnalyzer") 
      << "Process Run3ScoutingMuons: " << sct_el.sigmaIetaIeta();
   
    math::PtEtaPhiMLorentzVector tag_sct_el(sct_el.pt(), sct_el.eta(), sct_el.phi(), sct_el.m());
    for (const auto& sct_el_second : *sctEls){
      if(&sct_el_second == &sct_el) continue;
      math::PtEtaPhiMLorentzVector probe_sct_el(sct_el_second.pt(), sct_el_second.eta(), sct_el_second.phi(), sct_el_second.m());
      float invMass = (tag_sct_el + probe_sct_el).mass();
      edm::LogInfo("ScoutingMonitoring")
          << "Inv Mass: " << invMass;
      if((80 < invMass) && (invMass < 100)){  fillHistograms_resonance(histos.resonanceZ,  sct_el_second, invMass);
                                              fillHistograms_resonance(histos.resonanceAll,sct_el_second, invMass);}
      if((2.8 < invMass) && (invMass < 3.8)){ fillHistograms_resonance(histos.resonanceJ,  sct_el_second, invMass); // J/Psi mass: 3.3 +/- 0.2 GeV
                                              fillHistograms_resonance(histos.resonanceAll,sct_el_second, invMass);}
      if((9.0 < invMass) && (invMass < 12.6)){ fillHistograms_resonance(histos.resonanceY,  sct_el_second, invMass); // Y mass: 9.8 +/- 0.4 GeV & 10.6 +/- 1 GeV
                                               fillHistograms_resonance(histos.resonanceAll,sct_el_second, invMass);}
      
    }
  }
  */
}

bool ScoutingMuonTagProbeAnalyzer::scoutingMuonID(const Run3ScoutingMuon mu) const{

  return true;
}

void ScoutingMuonTagProbeAnalyzer::fillHistograms_resonance(const kProbeKinematicMuonHistos histos, 
                                                                const Run3ScoutingMuon mu,
                                                                const float inv_mass) const{
  histos.hEta->Fill(mu.eta());
  histos.hPhi->Fill(mu.phi());
  histos.hInvMass->Fill(inv_mass);

}

void ScoutingMuonTagProbeAnalyzer::bookHistograms(DQMStore::IBooker& ibook,
                                                       edm::Run const& run,
                                                       edm::EventSetup const& iSetup,
                                                       kTagProbeMuonHistos& histos) const{
    ibook.setCurrentFolder(outputInternalPath_);
    bookHistograms_resonance(ibook, run, iSetup, histos.resonanceZ, "resonanceZ");
    bookHistograms_resonance(ibook, run, iSetup, histos.resonanceJ, "resonanceJ");
    bookHistograms_resonance(ibook, run, iSetup, histos.resonanceY, "resonanceY");
    bookHistograms_resonance(ibook, run, iSetup, histos.resonanceAll, "resonanceAll");
 }

void  ScoutingMuonTagProbeAnalyzer::bookHistograms_resonance(DQMStore::IBooker& ibook,
                                                                 edm::Run const& run,
                                                                 edm::EventSetup const& iSetup,
                                                                 kProbeKinematicMuonHistos& histos,
                                                                 const std::string& name) const{
     ibook.setCurrentFolder(outputInternalPath_);

     histos.hEta = 
       ibook.book1D(name + "_Probe_sctMuon_Eta",
                    name + "_Probe_sctMuon_Eta", 100, -5.0, 5.0);
     histos.hPhi = 
       ibook.book1D(name + "_Probe_sctMuon_Phi",
                    name + "_Probe_sctMuon_Phi", 100, -3.3, 3.3);

     histos.hInvMass = 
       ibook.book1D(name + "_sctMuon_Invariant_Mass",
                    name + "_sctMuon_Invariant_Mass", 800, 0, 200);
}


// ------------ method fills 'descriptions' with the allowed parameters for the
// module  ------------
void ScoutingMuonTagProbeAnalyzer::fillDescriptions(
    edm::ConfigurationDescriptions& descriptions) {
  // The following says we do not know what parameters are allowed so do no
  // validation Please change this to state exactly what you do use, even if it
  // is no parameters
  edm::ParameterSetDescription desc;
  desc.add<std::string>("OutputInternalPath", "MY_FOLDER");
  desc.add<edm::InputTag>("MuonCollection",
                          edm::InputTag("slimmedMuons"));
  desc.add<edm::InputTag>("ScoutingMuonCollection",
                          edm::InputTag("Run3ScoutingMuons"));
  descriptions.add("ScoutingMuonTagProbeAnalyzer", desc);
}

DEFINE_FWK_MODULE(ScoutingMuonTagProbeAnalyzer);
