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
    SimControlLogger->logFile << simTime() << ": Simulation control online" << endl;
    //int res = system("python3 Physical_layer/Map_block/python_map_generator.py"); //python script to create a random map
    SimControlLogger->logFile << simTime() << ": New map created" << endl;
    //Map object creation///////////////////
    int blockSize = par("blockSize").intValue(); //size of block (part of map) that will be uploaded into RAM
    heightMapFile = par("heightMapFile").stdstringValue(); //name of file created by python script
    std::string heightMapPath = "Sim_logs/" + heightMapFile; // full path to map file
    Current_map = new HeightMapLoader(heightMapPath, blockSize);
    userInterrupt = par("userInterrupt");
    if (userInterrupt){
        interruptEvent = new cMessage("interruptEvent");
        interruptEvent->addPar("State") = "interruptEvent";
        interruptTime = 50;
        scheduleAt(simTime() + interruptTime, interruptEvent);
    }
    ////////////////////////////////////////
    nextStationId = par("nextStationId");
    numOfChargeStation = par("numOfChargeStation");
    ChargStationManager = new ChargingStationManager(getParentModule(),numOfChargeStation,nextStationId);
    //////////////////////////////////////
    //Self event to check if there any move to be done
    moveEventChecker = new cMessage("moveEventChecker");
    moveEventChecker->addPar("State") = "moveEventChecker";
    scheduleAt(simTime() + 1.0, moveEventChecker);
    /////////////////////////////////////
    //Creating list of drones
    int nD = par("numDrones").intValue();  // Get array size
    drone_data.clear();// Clear the vector to avoid duplication on re-initialization
    for (int i = 0; i < nD; i++) {// Extract drones from the NED submodules
        // Create a string for the module path
        std::string modulePath = "DroneNetwork.drones[" + std::to_string(i) + "]";
        // Get the module using the path
        cModule *host = getModuleByPath(modulePath.c_str());
        if (host) {
            // Get the "droneControl" submodule and cast it to DroneControl*
            DroneControl *drone = check_and_cast<DroneControl *>(host->getSubmodule("droneControl"));
            drone_data.push_back(drone);  // Store pointers to Drone instances
        } else {
            SimControlLogger->logFile << simTime() << ": Warning: Drone submodule " << i << " not found!" << endl;
        }
    }
    SimControlLogger->logFile << simTime() << ": Drone Objects (" << drone_data.size() << "):" << endl;
    for (const auto& drone : drone_data) {
        SimControlLogger->logFile << simTime() << ": Drone ID: " << drone->Drone_ID << endl;
    }
    getDisplayString().setTagArg("i", 0, "");
}

void SimulationControl::handleMessage(cMessage *msg){ // Handles incoming messages
    if (strcmp(msg->par("State").stringValue(), "moveEventChecker") == 0){
        current_drone_move(drone_data, 1, Current_map, SimControlLogger);
        scheduleAt(simTime() + 1.0, msg);
    }
    else if(strcmp(msg->par("State").stringValue(), "interruptEvent") == 0){
        // Get the parent module (e.g., the network or container module)
        cModule *parent = getParentModule();
        if (!parent)
            throw cRuntimeError("Cannot find module parent");
        // Get the module type from the NED file
        cModuleType *moduleType = cModuleType::get("Physical_layer.simulations.interruptModule");
        if (!moduleType)
            throw cRuntimeError("Cannot find module type 'interruptModule'");
        cModule *newModule = moduleType->create("interM", parent);
        // Build submodules and internal gates
        newModule->buildInside();
        // Schedule start if needed (for simple modules)
        newModule->scheduleStart(simTime());
        // Call initialize manually
        newModule->callInitialize();
        scheduleAt(simTime() + interruptTime, msg);
    }
    else if(strcmp(msg->par("State").stringValue(), "DELME") == 0){
        cModule *mod = getParentModule()->getSubmodule("interM");
        if (mod != nullptr) {
            mod->callFinish();
            mod->deleteModule();
        }
        delete msg;
    }

    else{
        SimControlLogger->logFile << simTime() << ": Unknown message: " << msg->getName() << endl;
    }
}
void SimulationControl::height_checker(double x_pos, double y_pos, double &z_val){
    z_val = Current_map->getHeightAt(x_pos, y_pos);
}
void SimulationControl::finish(){
    delete Current_map;
    delete ChargStationManager;
    if (moveEventChecker != nullptr) {
        if (moveEventChecker->isScheduled()) {
            cancelEvent(moveEventChecker);  // cancel first if still in event queue
        }
        delete moveEventChecker;
    }
    SimControlLogger->~SimulationControlLogger();
}



