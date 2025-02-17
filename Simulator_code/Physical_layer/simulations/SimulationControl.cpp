/*
 * SimulationControl.cpp
 *
 *  Created on: Jan 29, 2025
 *      Author: gekaizum
 */
/*  Example of message
    It is a constant structure
cMessage *msg = new cMessage("DroneData");
msg->addPar("state") = "NAVIGATION";
msg->addPar("x") = 12.5f;
msg->addPar("y") = 8.3f;
msg->addPar("z") = 0;
send(msg, "out");
*/
#include "SimulationControl.h"

void SimulationControl::initialize() {
    SimControlLogger = new SimulationControlLogger();
    SimControlLogger->logFile.flush();
    SimControlLogger->logFile << "Simulation control online" << endl;
    int res = system("python3 Physical_layer/Map_block/python_map_generator.py");
    SimControlLogger->logFile << "New map created" << endl;
    //Map object creation///////////////////
    int blockSize = par("blockSize").intValue();
    Current_map = new HeightMapLoader("Sim_logs/heightmap.txt", blockSize);
    ////////////////////////////////////////
    nextStationId = par("nextStationId");
    numOfChargeStation = par("numOfChargeStation");
    ChargStationManager = new ChargingStationManager(getParentModule(),numOfChargeStation,nextStationId);
    //////////////////////////////////////
    //Self event to check if there any move to be done
    moveEventChecker = new cMessage("moveEventChecker");
    moveEventChecker->addPar("State") = "moveEventChecker";
    scheduleAt(simTime() + 2.0, moveEventChecker);
    /////////////////////////////////////
    //Creating list of drones
    int nD = par("numDrones").intValue();  // Get array size

    // Clear the vector to avoid duplication on re-initialization
    drone_data.clear();

    // Extract drones from the NED submodules
    for (int i = 0; i < nD; i++) {
        // Create a string for the module path
        std::string modulePath = "DroneNetwork.drones[" + std::to_string(i) + "]";

        // Get the module using the path
        cModule *host = getModuleByPath(modulePath.c_str());

        if (host) {
            // Get the "droneControl" submodule and cast it to DroneControl*
            DroneControl *drone = check_and_cast<DroneControl *>(host->getSubmodule("droneControl"));
            drone_data.push_back(drone);  // Store pointers to Drone instances
        } else {
            SimControlLogger->logFile << "Warning: Drone submodule " << i << " not found!" << endl;
        }
    }
    //std::string getInfo() const { return "Drone ID: " + std::to_string(id);
    SimControlLogger->logFile << "Drone Objects (" << drone_data.size() << "):" << endl;
    for (const auto& drone : drone_data) {
        SimControlLogger->logFile << "Drone ID: " << drone->Drone_ID << endl;
    }
    /////////////////////////////////////
}
void SimulationControl::handleMessage(cMessage *msg){ // Handles incoming messages
    if (strcmp(msg->par("State").stringValue(), "POWER_ON") == 0){
    }
    else if (strcmp(msg->par("State").stringValue(), "TAKEOFF") == 0){
    }
    else if (strcmp(msg->par("State").stringValue(), "moveEventChecker") == 0){
        current_drone_move(drone_data, 2, Current_map, SimControlLogger);
        scheduleAt(simTime() + 2.0, msg);
    }
    else{
        SimControlLogger->logFile << "Unknown message: " << msg->getName() << endl;
    }
}
void SimulationControl::simControl_mainFunc(){}
double SimulationControl::height_checker(double x_pos, double y_pos){
    return Current_map->getHeightAt(x_pos, y_pos);
}
void SimulationControl::finish(){
    delete Current_map;
    delete ChargStationManager;
    SimControlLogger->~SimulationControlLogger();
}



