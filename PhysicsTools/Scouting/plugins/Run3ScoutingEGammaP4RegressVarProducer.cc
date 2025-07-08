// -*- C++ -*-
//
// Package:    PhysicsTools/Scouting
// Class:      Run3ScoutingEGammaP4RegressVarProducer
//
/**
 Description: Produce variables required to apply regression
 Implementation:
     In development currently
*/
//
// Original Author:  Abanti Ranadhir Sahasransu
//         Created:  Fri, 07 July 2025 20:11:15 GMT
//
//

// system include files
// #include <limits>
// #include <memory>
// #include <vector>

// user include files
// #include "FWCore/Framework/interface/Frameworkfwd.h"
// #include "FWCore/Framework/interface/stream/EDProducer.h"
// #include "FWCore/Framework/interface/Event.h"
// #include "FWCore/Framework/interface/MakerMacros.h"
// #include "FWCore/ParameterSet/interface/ParameterSet.h"
// #include "FWCore/Utilities/interface/Exception.h"
// #include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/StreamID.h"
// #include "FWCore/ServiceRegistry/interface/Service.h"

// #include "DataFormats/Common/interface/ValueMap.h"
// #include "DataFormats/Common/interface/Wrapper.h"
// #include "DataFormats/DetId/interface/DetId.h"
// #include "DataFormats/EcalDetId/interface/EBDetId.h"
// #include "DataFormats/EcalDetId/interface/EEDetId.h"
// #include "DataFormats/HepMCCandidate/interface/GenParticle.h"
// #include "DataFormats/Math/interface/LorentzVector.h"
// #include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Scouting/interface/Run3ScoutingElectron.h"

#include "PhysicsTools/Scouting/interface/Run3ScoutingEGammaMakeShowerStruct.h"

// #include <tuple>


//
// class declaration
//

class Run3ScoutingEGammaP4RegressVarProducer : public edm::stream::EDProducer<> {
public:
  explicit Run3ScoutingEGammaP4RegressVarProducer(const edm::ParameterSet&);
  static void fillDescriptions(edm::ConfigurationDescriptions&);

private:
  void produce(edm::Event&, const edm::EventSetup&) override;

  template <typename T>
  void putValueMap(edm::Event&, edm::Handle<Run3ScoutingElectronCollection>&, const std::vector<T>&, const std::string&);

  const edm::EDGetTokenT<std::vector<Run3ScoutingElectron>> run3ScoutingElectronToken_;
  edm::EDGetTokenT<edm::ValueMap<int>> run3SctEle_bestTrkToken_;
};

//
// constructors and destructor
//
Run3ScoutingEGammaP4RegressVarProducer::Run3ScoutingEGammaP4RegressVarProducer(const edm::ParameterSet& iConfig)
    : run3ScoutingElectronToken_(
          consumes<std::vector<Run3ScoutingElectron>>(iConfig.getParameter<edm::InputTag>("Run3ScoutingElectron"))),
    run3SctEle_bestTrkToken_(
          consumes<edm::ValueMap<int>>(iConfig.getParameter<edm::InputTag>("run3ScoutingElectronBestTrk"))) {

  // produces<edm::ValueMap<float>>("Run3ScoutingElectronTrackd0");
  // produces<edm::ValueMap<float>>("Run3ScoutingElectronTrackdz");
  // produces<edm::ValueMap<float>>("Run3ScoutingElectronTrackpt");
  // produces<edm::ValueMap<float>>("Run3ScoutingElectronTracketa");
  // produces<edm::ValueMap<float>>("Run3ScoutingElectronTrackphi");
  // produces<edm::ValueMap<float>>("Run3ScoutingElectronTrackpMode");
  // produces<edm::ValueMap<float>>("Run3ScoutingElectronTracketaMode");
  // produces<edm::ValueMap<float>>("Run3ScoutingElectronTrackphiMode");
  // produces<edm::ValueMap<float>>("Run3ScoutingElectronTrackqoverpModeError");
  // produces<edm::ValueMap<float>>("Run3ScoutingElectronTrackchi2overndf");
  // produces<edm::ValueMap<int>>("Run3ScoutingElectronTrackcharge");
}

//
// member functions
//

// ------------ method called to produce the data  ------------
void Run3ScoutingEGammaP4RegressVarProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<std::vector<Run3ScoutingElectron>> run3ScoutingElectronHandle;
  iEvent.getByToken(run3ScoutingElectronToken_, run3ScoutingElectronHandle);

  if (!run3ScoutingElectronHandle.isValid()) {
    // Handle the absence as a warning
    edm::LogWarning("Run3ScoutingEGammaP4RegressVarProducer")
        << "No Run3ScoutingElectron collection found in the event!";
    return;
  }

  // const size_t num_electrons = run3ScoutingElectronHandle->size();
  // std::vector<int> besttrk_idx(num_electrons, -1);
  // std::vector<float> besttrk_d0s(num_electrons, std::numeric_limits<float>::max());
  // std::vector<float> besttrk_dzs(num_electrons, std::numeric_limits<float>::max());
  // std::vector<float> besttrk_pts(num_electrons, std::numeric_limits<float>::max());
  // std::vector<float> besttrk_etas(num_electrons, std::numeric_limits<float>::max());
  // std::vector<float> besttrk_phis(num_electrons, std::numeric_limits<float>::max());
  // std::vector<float> besttrk_pModes(num_electrons, std::numeric_limits<float>::max());
  // std::vector<float> besttrk_etaModes(num_electrons, std::numeric_limits<float>::max());
  // std::vector<float> besttrk_phiModes(num_electrons, std::numeric_limits<float>::max());
  // std::vector<float> besttrk_qoverpModeErrors(num_electrons, std::numeric_limits<float>::max());
  // std::vector<float> besttrk_chi2overndfs(num_electrons, std::numeric_limits<float>::max());
  // std::vector<int> besttrk_charges(num_electrons, std::numeric_limits<int>::max());

  // for (size_t iElectron = 0; iElectron < num_electrons; ++iElectron) {
  //   const Run3ScoutingElectron& electron = run3ScoutingElectronHandle->at(iElectron);
  //   const math::PtEtaPhiMLorentzVector cluster(electron.pt(), electron.eta(), electron.phi(), 0.0005);

  //   double besttrack_ediff = std::numeric_limits<double>::max();

  //   for (unsigned int i = 0; i < electron.trkpt().size(); ++i) {
  //     const unsigned int eta_idx = (std::abs(electron.trketa()[i]) < 1.479) ? 0 : 1;
  //     if (electron.trkpt()[i] < trackPtMin_[eta_idx])
  //       continue;
  //     if (electron.trkchi2overndf()[i] > trackChi2OverNdofMax_[eta_idx])
  //       continue;

  //     const math::PtEtaPhiMLorentzVector gsftrack(
  //         electron.trkpt()[i], electron.trketa()[i], electron.trkphi()[i], 0.0005);

  //     if (deltaPhi(cluster.phi(), gsftrack.phi()) > deltaPhiMax_[eta_idx])
  //       continue;

  //     const double track_ediff = std::abs((cluster.energy() - gsftrack.energy()) / cluster.energy());
  //     if (track_ediff > relativeEnergyDifferenceMax_[eta_idx])
  //       continue;

  //     if (track_ediff < besttrack_ediff) {
  //       besttrack_ediff = track_ediff;
  //       besttrk_idx[iElectron] = i;
  //     }
  //   }

  //   if (besttrk_idx[iElectron] >= 0) {
  //     besttrk_d0s[iElectron] = electron.trkd0()[besttrk_idx[iElectron]];
  //     besttrk_dzs[iElectron] = electron.trkdz()[besttrk_idx[iElectron]];
  //     besttrk_pts[iElectron] = electron.trkpt()[besttrk_idx[iElectron]];
  //     besttrk_etas[iElectron] = electron.trketa()[besttrk_idx[iElectron]];
  //     besttrk_phis[iElectron] = electron.trkphi()[besttrk_idx[iElectron]];
  //     if (!electron.trkpMode().empty()) {
  //       besttrk_pModes[iElectron] = electron.trkpMode()[besttrk_idx[iElectron]];
  //       besttrk_etaModes[iElectron] = electron.trketaMode()[besttrk_idx[iElectron]];
  //       besttrk_phiModes[iElectron] = electron.trkphiMode()[besttrk_idx[iElectron]];
  //       besttrk_qoverpModeErrors[iElectron] = electron.trkqoverpModeError()[besttrk_idx[iElectron]];
  //     }
  //     besttrk_chi2overndfs[iElectron] = electron.trkchi2overndf()[besttrk_idx[iElectron]];
  //     besttrk_charges[iElectron] = electron.trkcharge()[besttrk_idx[iElectron]];
  //   }
  // }

  // putValueMap<int>(iEvent, run3ScoutingElectronHandle, besttrk_idx, "Run3ScoutingElectronBestTrackIndex");
  // putValueMap<float>(iEvent, run3ScoutingElectronHandle, besttrk_d0s, "Run3ScoutingElectronTrackd0");
  // putValueMap<float>(iEvent, run3ScoutingElectronHandle, besttrk_dzs, "Run3ScoutingElectronTrackdz");
  // putValueMap<float>(iEvent, run3ScoutingElectronHandle, besttrk_pts, "Run3ScoutingElectronTrackpt");
  // putValueMap<float>(iEvent, run3ScoutingElectronHandle, besttrk_etas, "Run3ScoutingElectronTracketa");
  // putValueMap<float>(iEvent, run3ScoutingElectronHandle, besttrk_phis, "Run3ScoutingElectronTrackphi");
  // putValueMap<float>(iEvent, run3ScoutingElectronHandle, besttrk_pModes, "Run3ScoutingElectronTrackpMode");
  // putValueMap<float>(iEvent, run3ScoutingElectronHandle, besttrk_etaModes, "Run3ScoutingElectronTracketaMode");
  // putValueMap<float>(iEvent, run3ScoutingElectronHandle, besttrk_phiModes, "Run3ScoutingElectronTrackphiMode");
  // putValueMap<float>(
  //     iEvent, run3ScoutingElectronHandle, besttrk_qoverpModeErrors, "Run3ScoutingElectronTrackqoverpModeError");
  // putValueMap<float>(iEvent, run3ScoutingElectronHandle, besttrk_chi2overndfs, "Run3ScoutingElectronTrackchi2overndf");
  // putValueMap<int>(iEvent, run3ScoutingElectronHandle, besttrk_charges, "Run3ScoutingElectronTrackcharge");
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void Run3ScoutingEGammaP4RegressVarProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>(("Run3ScoutingElectron"), edm::InputTag("hltScoutingEgammaPacker"));
  desc.add<edm::InputTag>("run3ScoutingElectronBestTrk", edm::InputTag("hltScoutingEgammaPacker", "bestTrackIndex"));
  descriptions.add("Run3ScoutingEGammaP4RegressVarProducer", desc);
}

// ------------ method template for putting value maps into the event  ------------
template <typename T>
void Run3ScoutingEGammaP4RegressVarProducer::putValueMap(edm::Event& iEvent,
                                                        edm::Handle<Run3ScoutingElectronCollection>& handle,
                                                        const std::vector<T>& values,
                                                        const std::string& label) {
  std::unique_ptr<edm::ValueMap<T>> valuemap(new edm::ValueMap<T>());
  typename edm::ValueMap<T>::Filler filler(*valuemap);
  filler.insert(handle, values.begin(), values.end());
  filler.fill();
  iEvent.put(std::move(valuemap), label);
}

//define this as a plug-in
DEFINE_FWK_MODULE(Run3ScoutingEGammaP4RegressVarProducer);
