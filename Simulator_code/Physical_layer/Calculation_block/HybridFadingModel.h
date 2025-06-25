/*
 * HybridFadingModel.h
 *
 *  Created on: May 31, 2025
 *      Author: shaked
 */

#ifndef PHYSICAL_LAYER_CALCULATION_BLOCK_HYBRIDFADINGMODEL_H_
#define PHYSICAL_LAYER_CALCULATION_BLOCK_HYBRIDFADINGMODEL_H_

#include <omnetpp.h>
#include "inet/environment/contract/IPhysicalEnvironment.h"
#include "inet/physicallayer/wireless/common/pathloss/FreeSpacePathLoss.h"
#include "inet/common/ModuleAccess.h"
#include "../Map_block/HeightMapLoader.h"
#include "../Drone_block/DroneControl.h"
#include "../simulations/SimulationControl.h"
//#include "inet/physicallayer/contract/packetlevel/IRadio.h"
#include "inet/physicallayer/wireless/common/radio/packetlevel/Radio.h"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <array>
#include <cmath>

using namespace inet;
using namespace physicallayer;
using namespace std;

class HybridFadingModel : public FreeSpacePathLoss
{
  protected:
    double k;
    const physicalenvironment::IPhysicalEnvironment *physicalEnvironment = nullptr;
    HeightMapLoader* Current_map = nullptr; // pointer to map object
    mps propagationSpeed = static_cast<mps>(299792458); //propagationSpeed in meter per second
    Hz frequency = static_cast<Hz>(2400000000); //2.4GHz wifi frequency

  protected:
    virtual void initialize(int stage) override;
    void loggerFunc(const std::string word) const;
  public:
    HybridFadingModel();
    virtual std::ostream& printToStream(std::ostream& stream, int level, int evFlags = 0) const override;
    virtual double computePathLoss(const ITransmission *transmission, const IArrival *arrival) const override;
    mutable std::ofstream fadingLogFile; // logger for drone
    std::string fileName;
};



#endif /* PHYSICAL_LAYER_CALCULATION_BLOCK_HYBRIDFADINGMODEL_H_ */
