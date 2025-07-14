// -*- C++ -*-
//
// Package:    PhysicsTools/Scouting
// Class:      Run3ScoutingEGammaP4RegressTrainNtupliser
//
/**
 Description: Creates Ntuples for Energy Momentum Regression for Electrons and Photons in Run3Scouting
 Implementation:
     Relies on shower shape variable and DetID classes
*/
//
// Original Author:  Abanti Ranadhir Sahasransu
//         Created:  Fri, 19 Jun 2025 11:08:20 GMT
//
//

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Scouting/interface/Run3ScoutingElectron.h"

#include "PhysicsTools/Scouting/interface/Run3ScoutingEGammaMakeShowerStruct.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

#include <array>

class Run3ScoutingEGammaP4RegressTrainNtupliser : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit Run3ScoutingEGammaP4RegressTrainNtupliser(const edm::ParameterSet&);
  ~Run3ScoutingEGammaP4RegressTrainNtupliser() override;

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;

  edm::EDGetTokenT<std::vector<Run3ScoutingElectron>> electronToken_;
  edm::EDGetTokenT<edm::ValueMap<int>> run3SctEle_bestTrkToken_;
  edm::EDGetTokenT<std::vector<reco::GenParticle>> genParticleToken_;

  TFile* outputFile_;
  TTree* tree_;

  const bool isMC_;
  float gen_pt_, gen_eta_, gen_phi_;

  // Variables for branches
  float pt_, eta_, phi_, rawe_, pse_, ecorrerror_, sieie_, hoe_, eiso_, hiso_, r9_, smin_, smaj_;
  float detain_, dphiin_, ooemoop_, tiso_, fbrem_;
  int ieta_, iphi_, iseb_;

  float eMax_, e2nd_, eL_, eR_, eT_, eB_;
  float e1x5_, e5x5_, e2x5M_, e2x5L_, e2x5R_, e2x5T_, e2x5B_;

  static constexpr unsigned int ecal2dwindow_ = Run3ScoutingEGammaMakeShowerStruct::ecal2dwindow;
  std::array<float, ecal2dwindow_> c_edep_;

  int foundGoodTrack_, trkq_;
  float trkpt_, trketa_, trkphi_, trkd0_, trkdz_, trkpMode_, trketaMode_, trkphiMode_, trkqoverpModeError_,
      trkchi2overndf_;
  int trkvecn_;
  std::vector<float> trkvecpt_, trkveceta_, trkvecphi_, trkvecpmode_, trkvecetamode_, trkvecphimode_, trkvecd0_,
      trkvecdz_, trkvecqoverpmodeerror_, trkvecchi2overndf_;
};

Run3ScoutingEGammaP4RegressTrainNtupliser::Run3ScoutingEGammaP4RegressTrainNtupliser(const edm::ParameterSet& iConfig)
    : electronToken_(consumes<std::vector<Run3ScoutingElectron>>(iConfig.getParameter<edm::InputTag>("electrons"))),
      run3SctEle_bestTrkToken_(
          consumes<edm::ValueMap<int>>(iConfig.getParameter<edm::InputTag>("run3ScoutingElectronBestTrk"))),
      isMC_(iConfig.getParameter<bool>("isMC")) {
  usesResource("TFileService");

  if (isMC_) {
    genParticleToken_ = consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genParticles"));
  }
}

Run3ScoutingEGammaP4RegressTrainNtupliser::~Run3ScoutingEGammaP4RegressTrainNtupliser() {}

void Run3ScoutingEGammaP4RegressTrainNtupliser::beginJob() {
  edm::Service<TFileService> fs;
  tree_ = fs->make<TTree>("Events", "Run3 Scouting Electron Regression Training Ntuple");

  // Branch to store genparticle information for MC only
  if (isMC_) {
    tree_->Branch("gen_pt", &gen_pt_, "gen_pt/F");
    tree_->Branch("gen_eta", &gen_eta_, "gen_eta/F");
    tree_->Branch("gen_phi", &gen_phi_, "gen_phi/F");
  }

  tree_->Branch("pt", &pt_, "pt/F");
  tree_->Branch("eta", &eta_, "eta/F");
  tree_->Branch("phi", &phi_, "phi/F");
  tree_->Branch("rawe", &rawe_, "rawe/F");
  tree_->Branch("pse", &pse_, "pse/F");
  tree_->Branch("ecorrerror", &ecorrerror_, "ecorrerror/F");
  tree_->Branch("sieie", &sieie_, "sieie/F");
  tree_->Branch("hoe", &hoe_, "hoe/F");
  tree_->Branch("eiso", &eiso_, "eiso/F");
  tree_->Branch("hiso", &hiso_, "hiso/F");
  tree_->Branch("r9", &r9_, "r9/F");
  tree_->Branch("smin", &smin_, "smin_/F");
  tree_->Branch("smaj", &smaj_, "smaj/F");

  tree_->Branch("detain", &detain_, "detain/F");
  tree_->Branch("dphiin", &dphiin_, "dphiin/F");
  tree_->Branch("ooemoop", &ooemoop_, "ooemoop/F");
  tree_->Branch("tiso", &tiso_, "tiso/F");
  tree_->Branch("fbrem", &fbrem_, "fbrem/F");

  tree_->Branch("ieta", &ieta_, "ieta/I");
  tree_->Branch("iphi", &iphi_, "iphi/I");
  tree_->Branch("iseb", &iseb_, "iseb/I");

  tree_->Branch("eMax", &eMax_, "eMax/F");
  tree_->Branch("e2nd", &e2nd_, "e2nd/F");
  tree_->Branch("eL", &eL_, "eL/F");
  tree_->Branch("eR", &eR_, "eR/F");
  tree_->Branch("eT", &eT_, "eT/F");
  tree_->Branch("eB", &eB_, "eB/F");
  tree_->Branch("e1x5", &e1x5_, "e1x5/F");
  tree_->Branch("e5x5", &e5x5_, "e5x5/F");
  tree_->Branch("e2x5M", &e2x5M_, "e2x5M/F");
  tree_->Branch("e2x5L", &e2x5L_, "e2x5L/F");
  tree_->Branch("e2x5R", &e2x5R_, "e2x5R/F");
  tree_->Branch("e2x5T", &e2x5T_, "e2x5T/F");
  tree_->Branch("e2x5B", &e2x5B_, "e2x5B/F");

  tree_->Branch("c_edep", c_edep_.data(), "c_edep[25]/F");

  tree_->Branch("foundGoodTrack", &foundGoodTrack_, "foundGoodTrack/I");
  tree_->Branch("trkpt", &trkpt_, "trkpt/F");
  tree_->Branch("trketa", &trketa_, "trketa/F");
  tree_->Branch("trkphi", &trkphi_, "trkphi/F");
  tree_->Branch("trkd0", &trkd0_, "trkd0/F");
  tree_->Branch("trkdz", &trkdz_, "trkdz/F");
  tree_->Branch("trkq", &trkq_, "trkq/I");
  tree_->Branch("trkpMode", &trkpMode_, "trkpMode/F");
  tree_->Branch("trketaMode", &trketaMode_, "trketaMode/F");
  tree_->Branch("trkphiMode", &trkphiMode_, "trkphiMode/F");
  tree_->Branch("trkqoverpModeError", &trkqoverpModeError_, "trkqoverpModeError/F");
  tree_->Branch("trkchi2overndf", &trkchi2overndf_, "trkchi2overndf/F");

  tree_->Branch("trkvecn", &trkvecn_, "trkvec/I");
  tree_->Branch("trkvecpt", &trkvecpt_);
  tree_->Branch("trkveceta", &trkveceta_);
  tree_->Branch("trkvecphi", &trkvecphi_);
  tree_->Branch("trkvecpmode", &trkvecpmode_);
  tree_->Branch("trkvecetamode", &trkvecetamode_);
  tree_->Branch("trkvecphimode", &trkvecphimode_);
  tree_->Branch("trkvecd0", &trkvecd0_);
  tree_->Branch("trkvecdz", &trkvecdz_);
  tree_->Branch("trkvecqoverpmodeerror", &trkvecqoverpmodeerror_);
  tree_->Branch("trkvecchi2overndf", &trkvecchi2overndf_);
}

void Run3ScoutingEGammaP4RegressTrainNtupliser::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<std::vector<Run3ScoutingElectron>> electrons;
  iEvent.getByToken(electronToken_, electrons);

  edm::Handle<edm::ValueMap<int>> run3SctEle_bestTrks;
  iEvent.getByToken(run3SctEle_bestTrkToken_, run3SctEle_bestTrks);

  if (!electrons.isValid())
    throw cms::Exception("Invalid Product for Run3 Scouting Electrons");

  if (!run3SctEle_bestTrks.isValid())
    throw cms::Exception("Found Invalid Value Map for Run3 Scouting Electron Best Track Indices");

  for (size_t i = 0; i < electrons->size(); ++i) {
    const auto& ele = (*electrons)[i];

    // Genmatching for MC
    if (isMC_) {
      gen_pt_ = -50.0f;
      gen_eta_ = -5.0f;
      gen_phi_ = -5.0f;
      edm::Handle<std::vector<reco::GenParticle>> genParticles;
      iEvent.getByToken(genParticleToken_, genParticles);
      if (genParticles.isValid()) {
        for (const auto& gen : *genParticles) {
          if ((abs(gen.pdgId()) == 11) && (gen.fromHardProcessFinalState() || gen.isPromptFinalState())) {
            math::PtEtaPhiMLorentzVector sctele(ele.pt(), ele.eta(), ele.phi(), ele.m());
            // Checked for double electron gun the difference between matched gen e's is quite small
            // In 1797 reco'd scouting electrons in 1000 events, where match not found for 332 for dR < 0.1 and 328 for dR < 0.2
            if (reco::deltaR(sctele, gen) < 0.1) {
              gen_pt_ = gen.pt();
              gen_eta_ = gen.eta();
              gen_phi_ = gen.phi();
              break;  // Found a matching gen electron, no need to continue
            }
          }
        }
      }
    }

    pt_ = ele.pt();
    eta_ = ele.eta();
    phi_ = ele.phi();
    rawe_ = ele.rawEnergy();
    pse_ = ele.preshowerEnergy();
    ecorrerror_ = ele.corrEcalEnergyError();
    sieie_ = ele.sigmaIetaIeta();
    hoe_ = ele.hOverE();
    eiso_ = ele.ecalIso();
    hiso_ = ele.hcalIso();
    r9_ = ele.r9();
    smin_ = ele.sMin();
    smaj_ = ele.sMaj();

    detain_ = ele.dEtaIn();
    dphiin_ = ele.dPhiIn();
    ooemoop_ = ele.ooEMOop();
    tiso_ = ele.trackIso();
    fbrem_ = ele.trackfbrem();

    int iseb = 0;
    std::tuple<int, int> detangs = Run3ScoutingEGammaMakeShowerStruct::getiEtaiPhiFromSeedId(ele.seedId(), iseb);
    ieta_ = std::get<0>(detangs);
    iphi_ = std::get<1>(detangs);
    iseb_ = iseb;

    c_edep_.fill(0.f);
    Run3ScoutingEGammaMakeShowerStruct::ShowerStruct ss =
        Run3ScoutingEGammaMakeShowerStruct::makeShowerStruct(ele.seedId(), ele.detIds(), ele.energyMatrix(), c_edep_);
    eMax_ = ss.eMax;
    e2nd_ = ss.e2nd;
    eL_ = ss.eL;
    eR_ = ss.eR;
    eT_ = ss.eT;
    eB_ = ss.eB;
    e1x5_ = ss.e1x5;
    e5x5_ = ss.e5x5;
    e2x5M_ = ss.e2x5M;
    e2x5L_ = ss.e2x5L;
    e2x5R_ = ss.e2x5R;
    e2x5T_ = ss.e2x5T;
    e2x5B_ = ss.e2x5B;

    edm::Ref<std::vector<Run3ScoutingElectron>> eleRef(electrons, i);
    int bestTrkIdx = (*run3SctEle_bestTrks)[eleRef];
    if (bestTrkIdx != -1) {
      foundGoodTrack_ = 1;
      trkpt_ = ele.trkpt()[bestTrkIdx];
      trketa_ = ele.trketa()[bestTrkIdx];
      trkphi_ = ele.trkphi()[bestTrkIdx];
      trkd0_ = ele.trkd0()[bestTrkIdx];
      trkdz_ = ele.trkdz()[bestTrkIdx];
      trkq_ = ele.trkcharge()[bestTrkIdx];
      trkpMode_ = ele.trkpMode()[bestTrkIdx];
      trketaMode_ = ele.trketaMode()[bestTrkIdx];
      trkphiMode_ = ele.trkphiMode()[bestTrkIdx];
      trkqoverpModeError_ = ele.trkqoverpModeError()[bestTrkIdx];
      trkchi2overndf_ = ele.trkchi2overndf()[bestTrkIdx];
    } else {
      foundGoodTrack_ = 0;
      trkpt_ = -50.0f;
      trketa_ = -5.0f;
      trkphi_ = -5.0f;
      trkd0_ = 0.0f;
      trkdz_ = -50.0f;
      trkq_ = -3.0f;
      trkpMode_ = -500.0f;
      trketaMode_ = -5.0f;
      trkphiMode_ = -5.0f;
      trkqoverpModeError_ = -1.0f;
      trkchi2overndf_ = -5.0f;
    }

    trkvecpt_ = ele.trkpt();
    trkveceta_ = ele.trketa();
    trkvecphi_ = ele.trkphi();
    trkvecpmode_ = ele.trkpMode();
    trkvecetamode_ = ele.trketaMode();
    trkvecphimode_ = ele.trkphiMode();
    trkvecd0_ = ele.trkd0();
    trkvecdz_ = ele.trkdz();
    trkvecqoverpmodeerror_ = ele.trkqoverpModeError();
    trkvecchi2overndf_ = ele.trkchi2overndf();
    trkvecn_ = trkvecpt_.size();

    tree_->Fill();
    trkvecpt_.clear();
    trkveceta_.clear();
    trkvecphi_.clear();
    trkvecpmode_.clear();
    trkvecetamode_.clear();
    trkvecphimode_.clear();
    trkvecd0_.clear();
    trkvecdz_.clear();
    trkvecqoverpmodeerror_.clear();
    trkvecchi2overndf_.clear();
  }
}

// Define this as a plugin
DEFINE_FWK_MODULE(Run3ScoutingEGammaP4RegressTrainNtupliser);
