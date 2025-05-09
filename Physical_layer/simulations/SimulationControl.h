/*
 * SimulationControl.h
 *
 *  Created on: Jan 29, 2025
 *      Author: gekaizum
 */

#ifndef PHYSICAL_LAYER_SIMULATIONS_SIMULATIONCONTROL_H_
#define PHYSICAL_LAYER_SIMULATIONS_SIMULATIONCONTROL_H_

#include <omnetpp.h>
#include <vector>
#include "../Drone_block/drone_positions_c.h"
#include "../Drone_block/DroneControl.h"
#include "../Map_block/ChargingStationManager.h"

using namespace omnetpp;

class SimulationControl : public cSimpleModule {
  private:
    std::string heightMapFile; //Map will be stored here
    int blockSize;
    int numOfChargeStation;
    int nextStationId;
    cMessage *moveEventChecker;
    HeightMapLoader* Current_map = nullptr; // pointer to map object
    SimulationControlLogger* SimControlLogger = nullptr; // pointer to logger object
    ChargingStationManager* ChargStationManager = nullptr; // pointer to charging stations manager object
  protected:
    virtual void initialize(); // Initializes the drone module
    virtual void handleMessage(cMessage *msg); // Handles incoming messages
    virtual void finish();
  public:
    std::vector<DroneControl*> drone_data; //List of drones needed for "drone_positions_c"
    void height_checker(double x_pos, double y_pos, double &z_val); // functions checks terrain height at point and stores it in z_val
};

Define_Module(SimulationControl);

#endif /* PHYSICAL_LAYER_SIMULATIONS_SIMULATIONCONTROL_H_ */
