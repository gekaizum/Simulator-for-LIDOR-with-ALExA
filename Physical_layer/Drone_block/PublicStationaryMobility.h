#ifndef PUBLICSTATIONARYMOBILITY_H_
#define PUBLICSTATIONARYMOBILITY_H_

#include "inet/mobility/base/StationaryMobilityBase.h"
#include "inet/common/geometry/common/Coord.h"

using namespace inet;

class PublicStationaryMobility : public StationaryMobilityBase {
  public:
    void setPositionPublic(const inet::Coord& pos);
    virtual void refreshDisplay() const override;
    virtual void updateMobilityStateFromDisplayString();
    bool updateFromDisplayString;

  protected:
    virtual void initialize(int stage) override;
};

#endif
