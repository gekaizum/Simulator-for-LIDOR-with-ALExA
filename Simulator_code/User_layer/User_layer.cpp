/*
 * User_layer.cpp
 *
 *  Created on: Feb 20, 2025
 *      Author: shaked
 */

#include "User_layer.h"

Define_Module(UserLayer);

void UserLayer::initialize() { // Initializes the drone module
//    UserLayer_Logger = new UserLayerLogger();
//    UserLayer_Logger->logFile.flush();
    //controlModule = check_and_cast<SimulationControl*>(getParentModule()->getSubmodule("simControl"));
    basicProtocolModule = check_and_cast<BasicProtocol*>(getParentModule()->getSubmodule("basicProtocol"));
//    UserLayer_Logger->logFile << "User layer online" << endl;
    testMsg = new cMessage("testMsg");
    testMsg->addPar("State") = "testMsg";
    scheduleAt(simTime()+2, testMsg);
}

void UserLayer::handleMessage(cMessage *msg){ // Handles incoming messages
        if(msg == testMsg){
            // Power on drones 1 and 2

            basicProtocolModule->height_checker(0, 0, z_val);
            basicProtocolModule->powerOn_drone(1, 0, 0, z_val);
            basicProtocolModule->powerOn_drone(2, 0, 0, z_val);
            basicProtocolModule->powerOn_drone(3, 0, 0, z_val);
            //basicProtocolModule->powerOn_drone(4, 0, 0, 0);

            // Take off Drone 1
            basicProtocolModule->set_velocity(1, 1,1, 0, 0, 10);
            basicProtocolModule->set_acceleration(1,1, 1, 5);
            basicProtocolModule->take_off(1, 1,1, 0, 0, z_val+50);      // Drone 1 takes off to 10m altitude
            //basicProtocolModule->take_off(1, 1, 150, 0, 10);     // Move Drone 1 forward until it reaches 150 meters
            cout << "Drone 1 took off and started moving...\n";
            Start_Flag = false;
            moveEV = new cMessage("moveTest");
            scheduleAt(simTime()+10, moveEV);
        }
        else if(msg == moveEV){
            basicProtocolModule->set_velocity(1, 1,1, 5, 5, 5);
            basicProtocolModule->set_acceleration(1,1, 1, 1);
            basicProtocolModule->move_to(1,1, 1,50, 50, z_val-100);
            testMsg2 = new cMessage("moveTest");
            scheduleAt(simTime()+10, testMsg2);
        }
        else{
            basicProtocolModule->set_velocity(1, 2,2, 5, 5, 5);
            basicProtocolModule->set_acceleration(1,2, 2, 1);
            basicProtocolModule->take_off(3, 1,2, 0, 0, z_val+50);      // Drone 1 takes off to 10m altitude

            cout << "Drone 1 reached 150 meters and stopped moving.\n";
        }
}

