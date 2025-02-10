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
    int res = system("python3 ../Map_block/python_map_generator.py");
    SimControlLogger->logFile << "New map created" << endl;
    //Map object creation///////////////////
    int blockSize = par("blockSize").intValue();
    Current_map = new HeightMapLoader("heightmap.txt", blockSize);
    ////////////////////////////////////////
    nextStationId = par("nextStationId");
    numOfChargeStation = par("numOfChargeStation");
    ChargStationManager = new ChargingStationManager(getParentModule(),numOfChargeStation,nextStationId);
    //Test message to power on all drones
    testInit = new cMessage("testInit");
    testInit->addPar("State") = "POWER_ON";
    testInit->addPar("x") = 1;
    testInit->addPar("y") = 1;
    testInit->addPar("z") = Current_map->getHeightAt(1, 1);
    SimControlLogger->logFile << "Drone 1: start at height: " << Current_map->getHeightAt(1, 1) << endl;
    scheduleAt(simTime() + 2.0, testInit);
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
        cModule* droneModule = getParentModule()->getSubmodule("drones", i);
        if (droneModule) {
            Drone* drone = check_and_cast<Drone*>(droneModule);
            drone_data.push_back(drone);  // Store pointers to Drone instances
        } else {
            SimControlLogger->logFile << "Warning: Drone submodule " << i << " not found!" << endl;
        }
    }
    /////////////////////////////////////
}
void SimulationControl::handleMessage(cMessage *msg){ // Handles incoming messages
    if (strcmp(msg->par("State").stringValue(), "POWER_ON") == 0){
        send(msg, "controlSocket$o",0);
        SimControlLogger->logFile << "POWER_ON signal sent to drone 1" << endl;
        cMessage *msg2 = msg->dup();
        msg2->par("x") = 10;
        msg2->par("y") = 10;
        msg2->par("z") = Current_map->getHeightAt(10, 10);
        send(msg2, "controlSocket$o",1);
        SimControlLogger->logFile << "Drone 2: start at height: " << Current_map->getHeightAt(10, 10) << endl;
        SimControlLogger->logFile << "POWER_ON signal sent to drone 2" << endl;
        cMessage *msg3 = msg->dup();
        msg3->par("x") = 20;
        msg3->par("y") = 20;
        msg3->par("z") = Current_map->getHeightAt(20, 20);
        SimControlLogger->logFile << "Drone 3: start at height: " << Current_map->getHeightAt(20, 20) << endl;
        send(msg3, "controlSocket$o",2);
        SimControlLogger->logFile << "POWER_ON signal sent to drone 3" << endl;

        testInit2 = new cMessage("TAKEOFF_test");
        testInit2->addPar("State") = "TAKEOFF";
        testInit2->addPar("x") = 1;
        testInit2->addPar("y") = 1;
        testInit2->addPar("z") = Current_map->getHeightAt(1, 1)+100;
        testInit2->addPar("acceleration") = 1;
        testInit2->addPar("x_velocity") = 5;
        testInit2->addPar("y_velocity") = 5;
        testInit2->addPar("z_velocity") = 5;
        scheduleAt(simTime() + 10.0, testInit2);
    }
    else if (strcmp(msg->par("State").stringValue(), "TAKEOFF") == 0){
        send(msg, "controlSocket$o",0);
        SimControlLogger->logFile << "TAKEOFF signal sent to drone 1" << endl;
        cMessage *msg2 = msg->dup();
        msg2->par("x") = 10;
        msg2->par("y") = 10;
        msg2->par("z") = Current_map->getHeightAt(10, 10)+150;
        send(msg2, "controlSocket$o",1);
        SimControlLogger->logFile << "TAKEOFF signal sent to drone 2" << endl;
        cMessage *msg3 = msg->dup();
        msg3->par("x") = 20;
        msg3->par("y") = 20;
        msg3->par("z") = Current_map->getHeightAt(20, 20)+100;
        send(msg3, "controlSocket$o",2);
        SimControlLogger->logFile << "TAKEOFF signal sent to drone 3" << endl;
    }
    else if (strcmp(msg->par("State").stringValue(), "moveEventChecker") == 0){
        current_drone_move(drone_data, 2, Current_map, SimControlLogger);
        scheduleAt(simTime() + 2.0, msg);
    }
    else{
        SimControlLogger->logFile << "Unknown message: " << msg->getName() << endl;
    }
    //delete msg;
}
void SimulationControl::simControl_mainFunc(){}
void SimulationControl::finish(){
    delete Current_map;
    delete ChargStationManager;
    //delete testInit;
    SimControlLogger->~SimulationControlLogger();
}



