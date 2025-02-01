// -*- C++ -*-
//
// Package:    ScoutingAnalysis/EGammaTools
// Class:      ScoutingElectronBestTrack
//
/**

 Description: Choose the most suitable track for a given scouting electron

 Implementation:
     Allows for ID selections on the tracks before associating them to the electrons
*/
//
// Original Author:  Abanti Ranadhir Sahasransu
//         Created:  Fri, 31 Jan 2025 14:43:20 GMT
//
//

// system include files
#include <limits>
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Scouting/interface/Run3ScoutingElectron.h"

//
// class declaration
//

class ScoutingElectronBestTrack : public edm::stream::EDProducer<> {
public:
  explicit ScoutingElectronBestTrack(const edm::ParameterSet&);
  static void fillDescriptions(edm::ConfigurationDescriptions&);

private:
  void produce(edm::Event&, const edm::EventSetup&) override;

  const edm::EDGetTokenT<std::vector<Run3ScoutingElectron>> run3ScoutingElectronToken_;
  std::vector<double> trackPtMin_;
  std::vector<double> trackChi2OverNdofMax_;
  std::vector<double> relativeEnergyDifferenceMax_;
  std::vector<double> deltaPhiMax_;
};

//
// constructors and destructor
//
ScoutingElectronBestTrack::ScoutingElectronBestTrack(const edm::ParameterSet& iConfig)
    : run3ScoutingElectronToken_(
          consumes<std::vector<Run3ScoutingElectron>>(iConfig.getParameter<edm::InputTag>("Run3ScoutingElectron"))) {
  produces<int>("BestTrackIndex");

  trackPtMin_ = iConfig.getParameter<std::vector<double>>("TrackPtMin");
  trackChi2OverNdofMax_ = iConfig.getParameter<std::vector<double>>("TrackChi2OverNdofMax");
  relativeEnergyDifferenceMax_ = iConfig.getParameter<std::vector<double>>("RelativeEnergyDifferenceMax");
  deltaPhiMax_ = iConfig.getParameter<std::vector<double>>("DeltaPhiMax");

  if (trackPtMin_.size() != 2) {
    throw cms::Exception("ScoutingElectronBestTrack")
        << "TrackPtMin must have exactly 2 elements for EB and EE respectively!";
  }
  if (trackChi2OverNdofMax_.size() != 2) {
    throw cms::Exception("ScoutingElectronBestTrack")
        << "TrackChi2OverNdofMax must have exactly 2 elements for EB and EE respectively!";
  }
  if (relativeEnergyDifferenceMax_.size() != 2) {
    throw cms::Exception("ScoutingElectronBestTrack")
        << "RelativeEnergyDifferenceMax must have exactly 2 elements for EB and EE respectively!";
  }
  if (deltaPhiMax_.size() != 2) {
    throw cms::Exception("ScoutingElectronBestTrack")
        << "DeltaPhiMax must have exactly 2 elements for EB and EE respectively!";
  }
}

//
// member functions
//

// ------------ method called to produce the data  ------------
void ScoutingElectronBestTrack::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<std::vector<Run3ScoutingElectron>> run3ScoutingElectronHandle;
  iEvent.getByToken(run3ScoutingElectronToken_, run3ScoutingElectronHandle);

  if (!run3ScoutingElectronHandle.isValid()) {
    // Handle the absence as a warning
    edm::LogWarning("ScoutingElectronBestTrack") << "No Run3ScoutingElectron collection found in the event!";
    return;
  }

  int besttrkidx = -1;
  double besttrack_ediff = std::numeric_limits<double>::max();
  ;
  for (const auto& electron : *run3ScoutingElectronHandle) {
    const math::PtEtaPhiMLorentzVector cluster(electron.pt(), electron.eta(), electron.phi(), 0.0005);

    for (unsigned int i = 0; i < electron.trkpt().size(); ++i) {
      const unsigned int eta_idx = (abs(electron.trketa().at(i)) < 1.479) ? 0 : 1;
      if (electron.trkpt().at(i) < trackPtMin_.at(eta_idx))
        continue;
      if (electron.trkchi2overndf().at(i) > trackChi2OverNdofMax_.at(eta_idx))
        continue;

      const math::PtEtaPhiMLorentzVector gsftrack(
          electron.trkpt().at(i), electron.trketa().at(i), electron.trkphi().at(i), 0.0005);

      if (deltaPhi(cluster.phi(), gsftrack.phi()) > deltaPhiMax_.at(eta_idx))
        continue;

      const double track_ediff = abs((cluster.energy() - gsftrack.energy()) / cluster.energy());
      if (track_ediff > relativeEnergyDifferenceMax_.at(eta_idx))
        continue;

      if (track_ediff < besttrack_ediff) {
        besttrack_ediff = track_ediff;
        besttrkidx = i;
      }
    }
  }

  iEvent.put(std::make_unique<int>(besttrkidx), "BestTrackIndex");
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void ScoutingElectronBestTrack::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>(("Run3ScoutingElectron"), edm::InputTag("hltScoutingEgammaPacker"));
  desc.add<std::vector<double>>(("TrackPtMin"), {0.0, 0.0});
  desc.add<std::vector<double>>(("TrackChi2OverNdofMax"), {9999.0, 9999.0});
  desc.add<std::vector<double>>(("RelativeEnergyDifferenceMax"), {9999.0, 9999.0});
  desc.add<std::vector<double>>(("DeltaPhiMax"), {9999.0, 9999.0});
  descriptions.add("scoutingElectronBestTrack", desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ScoutingElectronBestTrack);
