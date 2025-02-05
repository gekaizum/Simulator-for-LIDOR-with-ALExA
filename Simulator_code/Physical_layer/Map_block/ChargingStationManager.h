#ifndef CHARGINGSTATIONMANAGER_H_
#define CHARGINGSTATIONMANAGER_H_

#include <omnetpp.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "../Drone_block/ChargingStation.h"

using namespace omnetpp;

// Structure to represent a charging station

class ChargingStationManager {
private:
    int nextStationId;
protected:

public:
    //ChargingStationManager();
    //virtual ~ChargingStationManager();
    int addStation(int x, int y);
    bool moveStation(int id, int newX, int newY);
    bool removeStation(int id);
    bool getStationLocation(int id, int &x, int &y);
    void printAllStations();
};

#endif /* CHARGINGSTATIONMANAGER_H_ */

