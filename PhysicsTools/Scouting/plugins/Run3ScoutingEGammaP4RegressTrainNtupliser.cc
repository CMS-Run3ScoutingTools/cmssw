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
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/Scouting/interface/Run3ScoutingElectron.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

#include <tuple>


class Run3ScoutingEGammaP4RegressTrainNtupliser : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
    explicit Run3ScoutingEGammaP4RegressTrainNtupliser(const edm::ParameterSet&);
    ~Run3ScoutingEGammaP4RegressTrainNtupliser() override;

    std::tuple<int, int> getiEtaiPhiFromSeedId(uint32_t, int&);


private:
    void beginJob() override;
    void analyze(const edm::Event&, const edm::EventSetup&) override;
    DetId getOffsetId(const DetId&, int, int);
    float getHitEnergy(const DetId&, const std::unordered_map<int,float>&);

    edm::EDGetTokenT<std::vector<Run3ScoutingElectron>> electronToken_;

    TFile* outputFile_;
    TTree* tree_;

    // Variables for branches
    float pt_, eta_, phi_, rawe_, pse_, ecorrerror_, sieie_, hoe_, eiso_, hiso_, r9_, smin_, smaj_; 
    float detain_, dphiin_, ooemoop_, tiso_, fbrem_;
    int ieta_, iphi_, iseb_;

    float eMax_, e2nd_, eL_, eR_, eT_, eB_;
    float e1x5_, e5x5_, e2x5M_, e2x5L_, e2x5R_, e2x5T_, e2x5B_;

    static constexpr unsigned int ecal2dwindow_idxoffset = 2;
    static constexpr unsigned int ecal2dwindow = (2*(ecal2dwindow_idxoffset)+1) * (2*(ecal2dwindow_idxoffset)+1);
    std::array<float, ecal2dwindow> c_edep_;

public:
    struct ShowerStruct {
        // M - Max, L - Left, R - Right, T - Top, B - Bottom
        float eMax;
        float e2nd;
        float eL;
        float eR;
        float eT;
        float eB;
        float e1x5;
        float e5x5;
        float e2x5M;
        float e2x5L;
        float e2x5R;
        float e2x5T;
        float e2x5B;

        ShowerStruct(float eMax_v, float e2nd_v, float eL_v, float eR_v, float eT_v, float eB_v,
            float e1x5_v, float e5x5_v, float e2x5M_v, float e2x5L_v, float e2x5R_v, float e2x5T_v, float e2x5B_v)
        : eMax(eMax_v), e2nd(e2nd_v), eL(eL_v), eR(eR_v), eT(eT_v), eB(eB_v), 
        e1x5(e1x5_v), e5x5(e5x5_v), e2x5M(e2x5M_v), e2x5L(e2x5L_v), e2x5R(e2x5R_v), e2x5T(e2x5T_v), e2x5B(e2x5B_v){}
    };

    ShowerStruct makeShowerStruct(const uint32_t, const std::vector<unsigned uint32_t>&, const std::vector<float>&,
        std::array<float, ecal2dwindow>&);

};


Run3ScoutingEGammaP4RegressTrainNtupliser::Run3ScoutingEGammaP4RegressTrainNtupliser(const edm::ParameterSet& iConfig)
    : electronToken_(consumes<std::vector<Run3ScoutingElectron>>(iConfig.getParameter<edm::InputTag>("electrons"))) {
    usesResource("TFileService");
}


Run3ScoutingEGammaP4RegressTrainNtupliser::~Run3ScoutingEGammaP4RegressTrainNtupliser() {}


void Run3ScoutingEGammaP4RegressTrainNtupliser::beginJob() {
    edm::Service<TFileService> fs;
    tree_ = fs->make<TTree>("Events", "Run3 Scouting Electron Regression Training Ntuple");

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
}


void Run3ScoutingEGammaP4RegressTrainNtupliser::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    edm::Handle<std::vector<Run3ScoutingElectron>> electrons;
    iEvent.getByToken(electronToken_, electrons);

    if (!electrons.isValid()) return;

    for (const auto& ele : *electrons) {
        
        // TODO Add a condition that does exclusive gen matching

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
        std::tuple<int, int> detangs = getiEtaiPhiFromSeedId(ele.seedId(), iseb);
        ieta_ = std::get<0>(detangs);
        iphi_ = std::get<1>(detangs);
        iseb_ = iseb;

        c_edep_.fill(0.f);
        ShowerStruct ss = makeShowerStruct(ele.seedId(), ele.detIds(), ele.energyMatrix(), c_edep_);
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

        // std::vector<float> trkd0,
        // std::vector<float> trkdz,
        // std::vector<float> trkpt,
        // std::vector<float> trketa,
        // std::vector<float> trkphi,
        // std::vector<float> trkpMode,
        // std::vector<float> trketaMode,
        // std::vector<float> trkphiMode,
        // std::vector<float> trkqoverpModeError,
        // std::vector<float> trkchi2overndf,
        // std::vector<int> trkcharge,
        // uint32_t seedId,
        // uint32_t nClusters,
        // uint32_t nCrystals,
        // std::vector<float> energyMatrix,
        // std::vector<uint32_t> detIds,
        // std::vector<float> timingMatrix,

        tree_->Fill();
    }
}


std::tuple<int, int> Run3ScoutingEGammaP4RegressTrainNtupliser::getiEtaiPhiFromSeedId(uint32_t seedid, int& isEB) {
    
    int iEtaOrIX=-1, iPhiOrIY=-1;
    DetId eleSeedId(seedid);
    if(eleSeedId.det() == DetId::Ecal) {
        if(eleSeedId.subdetId() == EcalBarrel) {
            EBDetId ebId(eleSeedId);
            iEtaOrIX = ebId.ieta();
            iPhiOrIY = ebId.iphi();
            isEB = 1;
        }
        else if(eleSeedId.subdetId() == EcalEndcap) {
            EEDetId eeId(eleSeedId);
            iEtaOrIX = eeId.ix();
            iPhiOrIY = eeId.iy();
            isEB = 0;
        }
    }

    return std::make_tuple(iEtaOrIX, iPhiOrIY);
}


DetId Run3ScoutingEGammaP4RegressTrainNtupliser::getOffsetId(const DetId& seedId,int iEtaOrIX,int iPhiOrIY) {
    if(seedId.det()==DetId::Ecal && seedId.subdetId()==EcalBarrel){
        EBDetId ebId(seedId);
        return ebId.offsetBy(iEtaOrIX,iPhiOrIY);   
    }else if (seedId.det()==DetId::Ecal && seedId.subdetId()==EcalEndcap){
        EEDetId eeId(seedId);
        return eeId.offsetBy(iEtaOrIX,iPhiOrIY);
    }else{
        return DetId(0);
    }
}


float Run3ScoutingEGammaP4RegressTrainNtupliser::getHitEnergy(const DetId& id,const std::unordered_map<int,float>& detIdToEnergy) {
    auto entry = detIdToEnergy.find(id.rawId());
    if(entry!=detIdToEnergy.end()){
        return entry->second;
    }else{
        return 0.f;
    }
}


Run3ScoutingEGammaP4RegressTrainNtupliser::ShowerStruct Run3ScoutingEGammaP4RegressTrainNtupliser::makeShowerStruct(const uint32_t seedId,
const std::vector<uint32_t>& detIds, const std::vector<float>& eneM, std::array<float, ecal2dwindow>& c_edep) {
    float eMax=0.0f, e2nd=0.0, eL=0.0f, eR=0.0f, eT=0.0f, eB=0.0f,
        e1x5=0.0f, e5x5=0.0f, e2x5M=0.0f, e2x5L=0.0f, e2x5R=0.0f, e2x5T=0.0f, e2x5B=0.0f;
    std::unordered_map<int, float> detIdToEnergy;
    for(size_t index=0;index<detIds.size();index++) {
        detIdToEnergy[detIds[index]] = eneM[index];
    }
    constexpr int arrayOffset = ecal2dwindow_idxoffset;
    std::array<float,5> iEtaOrIXStrips;
    std::array<float,5> iPhiOrIYStrips;
    iEtaOrIXStrips.fill(0.f);
    iPhiOrIYStrips.fill(0.f);
    for(int iEtaOrIXNr=-arrayOffset;iEtaOrIXNr<=arrayOffset;iEtaOrIXNr++) {
        for(int iPhiOrIYNr=-arrayOffset;iPhiOrIYNr<=arrayOffset;iPhiOrIYNr++) {
            const DetId& id = getOffsetId(seedId,iEtaOrIXNr,iPhiOrIYNr);
            float energy = getHitEnergy(id,detIdToEnergy);
            if(energy>eMax) {
                e2nd = eMax;
                eMax = energy;
            }
            else if(energy>e2nd) {
                e2nd = energy;
            }
            e5x5+=energy;
            iEtaOrIXStrips[iEtaOrIXNr+arrayOffset]+=energy;
            iPhiOrIYStrips[iPhiOrIYNr+arrayOffset]+=energy;
            unsigned int edep_pos = ( (iEtaOrIXNr+arrayOffset) * ((2*arrayOffset)+1) ) + (iPhiOrIYNr+arrayOffset);
            c_edep[edep_pos] = energy;
        }
    }
    eL = getHitEnergy(getOffsetId(seedId,-1,0),detIdToEnergy);
    eR = getHitEnergy(getOffsetId(seedId,1,0),detIdToEnergy);
    eT = getHitEnergy(getOffsetId(seedId,0,1),detIdToEnergy);
    eB = getHitEnergy(getOffsetId(seedId,0,-1),detIdToEnergy);

    e2x5L = iEtaOrIXStrips[-2+arrayOffset]+iEtaOrIXStrips[-1+arrayOffset];
    e2x5R = iEtaOrIXStrips[1+arrayOffset]+iEtaOrIXStrips[2+arrayOffset];
    e1x5 = iEtaOrIXStrips[0+arrayOffset];
    e2x5T = iPhiOrIYStrips[1+arrayOffset] + iPhiOrIYStrips[2+arrayOffset];
    e2x5B = iPhiOrIYStrips[-2+arrayOffset] + iPhiOrIYStrips[-1+arrayOffset];
    e2x5M = std::max(iEtaOrIXStrips[-1+arrayOffset],iEtaOrIXStrips[1+arrayOffset])+ iEtaOrIXStrips[0+arrayOffset];

    return Run3ScoutingEGammaP4RegressTrainNtupliser::ShowerStruct(eMax, e2nd, eL, eR, eT, eB,
        e1x5, e5x5, e2x5M, e2x5L, e2x5R, e2x5T, e2x5B);

}

// Define this as a plugin
DEFINE_FWK_MODULE(Run3ScoutingEGammaP4RegressTrainNtupliser);
