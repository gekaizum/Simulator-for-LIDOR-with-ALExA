/*
 * HybridFadingModel.h
 *
 *  Created on: May 31, 2025
 *      Author: shaked
 */

#ifndef PHYSICAL_LAYER_CALCULATION_BLOCK_HYBRIDFADINGMODEL_H_
#define PHYSICAL_LAYER_CALCULATION_BLOCK_HYBRIDFADINGMODEL_H_

#include "inet/environment/contract/IPhysicalEnvironment.h"
#include "inet/physicallayer/wireless/common/pathloss/FreeSpacePathLoss.h"
#include "inet/common/ModuleAccess.h"
//#include "inet/physicallayer/wireless/common/contract/packetlevel/ITransmission.h"
//#include "inet/physicallayer/wireless/common/contract/packetlevel/IArrival.h"


using namespace inet;
using namespace physicallayer;
using namespace std;

class HybridFadingModel : public FreeSpacePathLoss
{
  protected:
    double k;
    const physicalenvironment::IPhysicalEnvironment *physicalEnvironment = nullptr;

  protected:
    virtual void initialize(int stage) override;

  public:
    HybridFadingModel();
    virtual std::ostream& printToStream(std::ostream& stream, int level, int evFlags = 0) const override;
    //virtual double computePathLoss(mps propagationSpeed, Hz frequency, m distance, const ITransmission *transmission, const IArrival *arrival) const override;
    virtual double computePathLoss(mps propagationSpeed, Hz frequency, m distance) const override;
    //virtual double computePathLoss(const ITransmission *transmission, const IArrival *arrival) const override;
};



#endif /* PHYSICAL_LAYER_CALCULATION_BLOCK_HYBRIDFADINGMODEL_H_ */
