#ifndef CHARGINGSTATIONMANAGER_H_
#define CHARGINGSTATIONMANAGER_H_

#include <omnetpp.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "../Drone_block/ChargingStation.h"

using namespace omnetpp;

class ChargingStationManager {
private:
    int nextStationId;
    cModule* parentModule;

public:
    std::vector<ChargingStation*> ChargingStation_data;
    ChargingStationManager(cModule* parent,int numOfChargeStation, int nSid);
    virtual ~ChargingStationManager();
    void addStation(int x, int y, int z);
    bool moveStation(int id, int newX, int newY, int newZ);
    bool removeStation(int id);
    bool getStationLocation(int id, int &x, int &y, int &z);
    void printAllStations();
};

#endif /* CHARGINGSTATIONMANAGER_H_ */
