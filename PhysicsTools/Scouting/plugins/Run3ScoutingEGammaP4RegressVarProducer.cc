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
#include <sstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/ValueMap.h"
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

  static constexpr unsigned int ecal2dwindow_ = Run3ScoutingEGammaMakeShowerStruct::ecal2dwindow;
};

//
// constructors and destructor
//
Run3ScoutingEGammaP4RegressVarProducer::Run3ScoutingEGammaP4RegressVarProducer(const edm::ParameterSet& iConfig)
    : run3ScoutingElectronToken_(
          consumes<std::vector<Run3ScoutingElectron>>(iConfig.getParameter<edm::InputTag>("Run3ScoutingElectron"))) {
  produces<edm::ValueMap<int>>("iEtaOriX");
  produces<edm::ValueMap<int>>("iPhiOriY");
  produces<edm::ValueMap<int>>("isEB");
  produces<edm::ValueMap<float>>("showerEMax");
  produces<edm::ValueMap<float>>("showerE2ndMax");
  produces<edm::ValueMap<float>>("showerELeft");
  produces<edm::ValueMap<float>>("showerERight");
  produces<edm::ValueMap<float>>("showerETop");
  produces<edm::ValueMap<float>>("showerEBottom");
  produces<edm::ValueMap<float>>("showerE1x5");
  produces<edm::ValueMap<float>>("showerE5x5");
  produces<edm::ValueMap<float>>("showerE2x5Max");
  produces<edm::ValueMap<float>>("showerE2x5Left");
  produces<edm::ValueMap<float>>("showerE2x5Right");
  produces<edm::ValueMap<float>>("showerE2x5Top");
  produces<edm::ValueMap<float>>("showerE2x5Bottom");
  for(unsigned int iCrystal=0; iCrystal<ecal2dwindow_; iCrystal++) {
    std::stringstream bname;
    bname<<"showerEdep"<<iCrystal;
    produces<edm::ValueMap<float>>(bname.str());
  }
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

  const size_t num_electrons = run3ScoutingElectronHandle->size();
  std::vector<int> ieta(num_electrons, -1);
  std::vector<int> iphi(num_electrons, -1);
  std::vector<int> iseb(num_electrons, -1);
  std::vector<float> emax(num_electrons, -1.0f);
  std::vector<float> e2nd(num_electrons, -1.0f);
  std::vector<float> el(num_electrons, -1.0f);
  std::vector<float> er(num_electrons, -1.0f);
  std::vector<float> et(num_electrons, -1.0f);
  std::vector<float> eb(num_electrons, -1.0f);
  std::vector<float> e1x5(num_electrons, -1.0f);
  std::vector<float> e5x5(num_electrons, -1.0f);
  std::vector<float> e2x5m(num_electrons, -1.0f);
  std::vector<float> e2x5l(num_electrons, -1.0f);
  std::vector<float> e2x5r(num_electrons, -1.0f);
  std::vector<float> e2x5t(num_electrons, -1.0f);
  std::vector<float> e2x5b(num_electrons, -1.0f);
  std::vector<std::array<float, ecal2dwindow_>> c_edep_(num_electrons, std::array<float, ecal2dwindow_>{});

  for (size_t iElectron = 0; iElectron < num_electrons; ++iElectron) {
    const Run3ScoutingElectron& electron = run3ScoutingElectronHandle->at(iElectron);

    int iseb_ = 0;
    std::tuple<int, int> detangs = Run3ScoutingEGammaMakeShowerStruct::getiEtaiPhiFromSeedId(electron.seedId(), iseb_);
    ieta[iElectron] = std::get<0>(detangs);
    iphi[iElectron] = std::get<1>(detangs);
    iseb[iElectron] = iseb_;

    c_edep_[iElectron].fill(0.f);
    Run3ScoutingEGammaMakeShowerStruct::ShowerStruct ss =
        Run3ScoutingEGammaMakeShowerStruct::makeShowerStruct(electron.seedId(), electron.detIds(), electron.energyMatrix(), c_edep_[iElectron]);
    emax[iElectron] = ss.eMax;
    e2nd[iElectron] = ss.e2nd;
    el[iElectron] = ss.eL;
    er[iElectron] = ss.eR;
    et[iElectron] = ss.eT;
    eb[iElectron] = ss.eB;
    e1x5[iElectron] = ss.e1x5;
    e5x5[iElectron] = ss.e5x5;
    e2x5m[iElectron] = ss.e2x5M;
    e2x5l[iElectron] = ss.e2x5L;
    e2x5r[iElectron] = ss.e2x5R;
    e2x5t[iElectron] = ss.e2x5T;
    e2x5b[iElectron] = ss.e2x5B;
  }

  putValueMap<int>(iEvent, run3ScoutingElectronHandle, ieta, "iEtaOriX");
  putValueMap<int>(iEvent, run3ScoutingElectronHandle, iphi, "iPhiOriY");
  putValueMap<int>(iEvent, run3ScoutingElectronHandle, iseb, "isEB");
  putValueMap<float>(iEvent, run3ScoutingElectronHandle, emax, "showerEMax");
  putValueMap<float>(iEvent, run3ScoutingElectronHandle, e2nd, "showerE2ndMax");
  putValueMap<float>(iEvent, run3ScoutingElectronHandle, el, "showerELeft");
  putValueMap<float>(iEvent, run3ScoutingElectronHandle, er, "showerERight");
  putValueMap<float>(iEvent, run3ScoutingElectronHandle, et, "showerETop");
  putValueMap<float>(iEvent, run3ScoutingElectronHandle, eb, "showerEBottom");
  putValueMap<float>(iEvent, run3ScoutingElectronHandle, e1x5, "showerE1x5");
  putValueMap<float>(iEvent, run3ScoutingElectronHandle, e5x5, "showerE5x5");
  putValueMap<float>(iEvent, run3ScoutingElectronHandle, e2x5m, "showerE2x5Max");
  putValueMap<float>(iEvent, run3ScoutingElectronHandle, e2x5l, "showerE2x5Left");
  putValueMap<float>(iEvent, run3ScoutingElectronHandle, e2x5r, "showerE2x5Right");
  putValueMap<float>(iEvent, run3ScoutingElectronHandle, e2x5t, "showerE2x5Top");
  putValueMap<float>(iEvent, run3ScoutingElectronHandle, e2x5b, "showerE2x5Bottom");
  for(unsigned int iCrystal=0; iCrystal<ecal2dwindow_; iCrystal++) {
    std::stringstream bname;
    bname<<"showerEdep"<<iCrystal;

    std::vector<float> c_edep_crystal(num_electrons, 0.0f);
    for (size_t iElectron = 0; iElectron < num_electrons; ++iElectron) {
      c_edep_crystal[iElectron] = c_edep_[iElectron][iCrystal];
    }

    putValueMap<float>(iEvent, run3ScoutingElectronHandle, c_edep_crystal, bname.str());
  }
  
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void Run3ScoutingEGammaP4RegressVarProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>(("Run3ScoutingElectron"), edm::InputTag("hltScoutingEgammaPacker"));
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
