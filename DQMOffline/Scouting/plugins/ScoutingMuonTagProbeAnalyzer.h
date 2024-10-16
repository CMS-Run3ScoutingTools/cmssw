#ifndef DQMOffline_Scouting_ScoutingMuonTagProbeAnalyzer_h
#define DQMOffline_Scouting_ScoutingMuonTagProbeAnalyzer_h

#include <string>
#include <vector>

// user include files
#include "DQMServices/Core/interface/DQMGlobalEDAnalyzer.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Scouting/interface/Run3ScoutingMuon.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

/////////////////////////
//  Class declaration  //
/////////////////////////

struct kProbeKinematicMuonHistos{
  dqm::reco::MonitorElement* hEta;
  dqm::reco::MonitorElement* hPhi;
  dqm::reco::MonitorElement* hInvMass;
};

struct kTagProbeMuonHistos {
  kProbeKinematicMuonHistos resonanceZ;
  kProbeKinematicMuonHistos resonanceJ;
  kProbeKinematicMuonHistos resonanceY;
  kProbeKinematicMuonHistos resonanceAll;
};

class ScoutingMuonTagProbeAnalyzer: public DQMGlobalEDAnalyzer<kTagProbeMuonHistos> {
      public: 
        explicit ScoutingMuonTagProbeAnalyzer(const edm::ParameterSet& conf);
        ~ScoutingMuonTagProbeAnalyzer() override;
        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

      private:
        void dqmAnalyze(const edm::Event & e, const edm::EventSetup & c, kTagProbeMuonHistos const&) const override;
        void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &, kTagProbeMuonHistos &) const override;
        void bookHistograms_resonance(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &, kProbeKinematicMuonHistos &, const std::string &) const;
        void fillHistograms_resonance(const kProbeKinematicMuonHistos histos, const Run3ScoutingMuon mu, const float inv_mass) const;
        bool scoutingMuonID(const Run3ScoutingMuon mu) const;

        // --------------------- member data  ----------------------
        std::string outputInternalPath_;
        edm::EDGetTokenT<std::vector<pat::Muon>> muonCollection_;
        edm::EDGetTokenT<std::vector<Run3ScoutingMuon>> scoutingMuonCollection_;

    };

#endif
