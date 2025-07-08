#ifndef RUN3SCOUTINGEGAMMAMAKESHOWERSTRUCT_H
#define RUN3SCOUTINGEGAMMAMAKESHOWERSTRUCT_H

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include <array>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <tuple>


class Run3ScoutingEGammaMakeShowerStruct {
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

    ShowerStruct(float eMax_v,
                 float e2nd_v,
                 float eL_v,
                 float eR_v,
                 float eT_v,
                 float eB_v,
                 float e1x5_v,
                 float e5x5_v,
                 float e2x5M_v,
                 float e2x5L_v,
                 float e2x5R_v,
                 float e2x5T_v,
                 float e2x5B_v)
        : eMax(eMax_v),
          e2nd(e2nd_v),
          eL(eL_v),
          eR(eR_v),
          eT(eT_v),
          eB(eB_v),
          e1x5(e1x5_v),
          e5x5(e5x5_v),
          e2x5M(e2x5M_v),
          e2x5L(e2x5L_v),
          e2x5R(e2x5R_v),
          e2x5T(e2x5T_v),
          e2x5B(e2x5B_v) {}
  };

  static constexpr unsigned int ecal2dwindow_idxoffset = 2;
  static constexpr unsigned int ecal2dwindow = (2 * (ecal2dwindow_idxoffset) + 1) * (2 * (ecal2dwindow_idxoffset) + 1);

  ShowerStruct makeShowerStruct(const uint32_t,
                                const std::vector<uint32_t>&,
                                const std::vector<float>&,
                                std::array<float, ecal2dwindow>&);

  std::tuple<int, int> getiEtaiPhiFromSeedId(uint32_t, int&);
  DetId getOffsetId(const DetId&, int, int);
  float getHitEnergy(const DetId&, const std::unordered_map<int, float>&);

};

#endif