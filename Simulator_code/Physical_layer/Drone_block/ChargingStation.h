/*
 * ChargingStation.h
 *
 *  Created on: Feb 5, 2025
 *      Author: gekaizum
 */

#ifndef PHYSICAL_LAYER_DRONE_BLOCK_CHARGINGSTATION_H_
#define PHYSICAL_LAYER_DRONE_BLOCK_CHARGINGSTATION_H_

#include <omnetpp.h>
#include "../Calculation_block/SignalStrengthCalculation.h"

using namespace omnetpp;

class ChargingStation : public cSimpleModule {
  private:
    //char communication_type;              // Communication type ('W' for WiFi, 'R' for Radio)
    double frequencyHz;                     // Frequency used by drone to communicate

  protected:
    virtual void initialize(); // Initializes the drone module
    virtual void handleMessage(cMessage *msg); // Handles incoming messages
    virtual void finish();
  public:
    int ChargingStation_ID;               // Unique identifier for the ChargingStation
    float Current_Position[3];            // ChargingStation's position as [x,y,z] coordinates

    void broadcast(const std::string& message);              // Broadcasts a message to all drones
    void sendTo(int target_id, const std::string& message);  // Sends a message to a specific drone

    //std::ofstream droneLogFile; // logger for drone
    //std::string fileName;
};
Define_Module(ChargingStation);



#endif /* PHYSICAL_LAYER_DRONE_BLOCK_CHARGINGSTATION_H_ */
