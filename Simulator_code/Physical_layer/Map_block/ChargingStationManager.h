#ifndef CHARGINGSTATIONMANAGER_H_
#define CHARGINGSTATIONMANAGER_H_

#include <omnetpp.h>
#include <vector>
#include <string>
#include <unordered_map>

using namespace omnetpp;

// Structure to represent a charging station
struct ChargingStation {
    int id;  // Unique identifier for the station
    int x;   // X-coordinate on the map
    int y;   // Y-coordinate on the map
};

class ChargingStationManager : public cSimpleModule {
private:
    std::unordered_map<int, ChargingStation> stations; // Map of stations by ID
    int nextStationId;

protected:
    virtual void initialize() override;
    //virtual void handleMessage(cMessage *msg) override;

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

