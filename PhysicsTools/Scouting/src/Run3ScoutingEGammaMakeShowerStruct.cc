// -*- C++ -*-
//
// Package:    PhysicsTools/Scouting
// Class:      Run3ScoutingEGammaMakeShowerStruct
//
/**
 Description: Create shower shape variables for Run3 scouting EG regression
*/
//
// Original Author:  Abanti Ranadhir Sahasransu
//         Created:  Fri, 07 July 2025 07:07:25 GMT
//
//

#include "PhysicsTools/Scouting/interface/Run3ScoutingEGammaMakeShowerStruct.h"

std::tuple<int, int> Run3ScoutingEGammaMakeShowerStruct::getiEtaiPhiFromSeedId(uint32_t seedid, int& isEB) {
  int iEtaOrIX = -1, iPhiOrIY = -1;
  DetId eleSeedId(seedid);
  if (eleSeedId.det() == DetId::Ecal) {
    if (eleSeedId.subdetId() == EcalBarrel) {
      EBDetId ebId(eleSeedId);
      iEtaOrIX = ebId.ieta();
      iPhiOrIY = ebId.iphi();
      isEB = 1;
    } else if (eleSeedId.subdetId() == EcalEndcap) {
      EEDetId eeId(eleSeedId);
      iEtaOrIX = eeId.ix();
      iPhiOrIY = eeId.iy();
      isEB = 0;
    }
  }

  return std::make_tuple(iEtaOrIX, iPhiOrIY);
}

DetId Run3ScoutingEGammaMakeShowerStruct::getOffsetId(const DetId& seedId, int iEtaOrIX, int iPhiOrIY) {
  if (seedId.det() == DetId::Ecal && seedId.subdetId() == EcalBarrel) {
    EBDetId ebId(seedId);
    return ebId.offsetBy(iEtaOrIX, iPhiOrIY);
  } else if (seedId.det() == DetId::Ecal && seedId.subdetId() == EcalEndcap) {
    EEDetId eeId(seedId);
    return eeId.offsetBy(iEtaOrIX, iPhiOrIY);
  } else {
    return DetId(0);
  }
}

float Run3ScoutingEGammaMakeShowerStruct::getHitEnergy(const DetId& id,
                                                       const std::unordered_map<int, float>& detIdToEnergy) {
  auto entry = detIdToEnergy.find(id.rawId());
  if (entry != detIdToEnergy.end()) {
    return entry->second;
  } else {
    return 0.f;
  }
}

Run3ScoutingEGammaMakeShowerStruct::ShowerStruct Run3ScoutingEGammaMakeShowerStruct::makeShowerStruct(
    const uint32_t seedId,
    const std::vector<uint32_t>& detIds,
    const std::vector<float>& eneM,
    std::array<float, ecal2dwindow>& c_edep) {
  float eMax = 0.0f, e2nd = 0.0, eL = 0.0f, eR = 0.0f, eT = 0.0f, eB = 0.0f, e1x5 = 0.0f, e5x5 = 0.0f, e2x5M = 0.0f,
        e2x5L = 0.0f, e2x5R = 0.0f, e2x5T = 0.0f, e2x5B = 0.0f;
  std::unordered_map<int, float> detIdToEnergy;
  for (size_t index = 0; index < detIds.size(); index++) {
    detIdToEnergy[detIds[index]] = eneM[index];
  }
  constexpr int arrayOffset = ecal2dwindow_idxoffset;
  std::array<float, ecal2dwindow> iEtaOrIXStrips;
  std::array<float, ecal2dwindow> iPhiOrIYStrips;
  iEtaOrIXStrips.fill(0.f);
  iPhiOrIYStrips.fill(0.f);
  for (int iEtaOrIXNr = -arrayOffset; iEtaOrIXNr <= arrayOffset; iEtaOrIXNr++) {
    for (int iPhiOrIYNr = -arrayOffset; iPhiOrIYNr <= arrayOffset; iPhiOrIYNr++) {
      const DetId& id = getOffsetId(seedId, iEtaOrIXNr, iPhiOrIYNr);
      float energy = getHitEnergy(id, detIdToEnergy);
      if (energy > eMax) {
        e2nd = eMax;
        eMax = energy;
      } else if (energy > e2nd) {
        e2nd = energy;
      }
      e5x5 += energy;
      iEtaOrIXStrips[iEtaOrIXNr + arrayOffset] += energy;
      iPhiOrIYStrips[iPhiOrIYNr + arrayOffset] += energy;
      unsigned int edep_pos = ((iEtaOrIXNr + arrayOffset) * ((2 * arrayOffset) + 1)) + (iPhiOrIYNr + arrayOffset);
      c_edep[edep_pos] = energy;
    }
  }
  eL = getHitEnergy(getOffsetId(seedId, -1, 0), detIdToEnergy);
  eR = getHitEnergy(getOffsetId(seedId, 1, 0), detIdToEnergy);
  eT = getHitEnergy(getOffsetId(seedId, 0, 1), detIdToEnergy);
  eB = getHitEnergy(getOffsetId(seedId, 0, -1), detIdToEnergy);

  e2x5L = iEtaOrIXStrips[-2 + arrayOffset] + iEtaOrIXStrips[-1 + arrayOffset];
  e2x5R = iEtaOrIXStrips[1 + arrayOffset] + iEtaOrIXStrips[2 + arrayOffset];
  e1x5 = iEtaOrIXStrips[0 + arrayOffset];
  e2x5T = iPhiOrIYStrips[1 + arrayOffset] + iPhiOrIYStrips[2 + arrayOffset];
  e2x5B = iPhiOrIYStrips[-2 + arrayOffset] + iPhiOrIYStrips[-1 + arrayOffset];
  e2x5M = std::max(iEtaOrIXStrips[-1 + arrayOffset], iEtaOrIXStrips[1 + arrayOffset]) + iEtaOrIXStrips[0 + arrayOffset];

  return Run3ScoutingEGammaMakeShowerStruct::ShowerStruct(
      eMax, e2nd, eL, eR, eT, eB, e1x5, e5x5, e2x5M, e2x5L, e2x5R, e2x5T, e2x5B);
}
