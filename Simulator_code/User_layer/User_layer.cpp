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
    getDisplayString().setTagArg("i", 0, "");
    landingEV = new cMessage("landingEV");
    landingEV->addPar("State") = "landingEV";
    scheduleAt(simTime()+452, landingEV);
}

void UserLayer::handleMessage(cMessage *msg){ // Handles incoming messages
        if(msg == testMsg){
            // Power on drones 1 and 2
            basicProtocolModule->height_checker(1500, 1250, z_val);
            basicProtocolModule->moveStation(1, 1500, 1250, z_val);
            basicProtocolModule->height_checker(250, 250, z_val);
            basicProtocolModule->powerOn_drone(1, 250, 250, z_val);
            basicProtocolModule->height_checker(450, 250, z_val);
            basicProtocolModule->powerOn_drone(2, 450, 250, z_val);
            basicProtocolModule->height_checker(650, 250, z_val);
            basicProtocolModule->powerOn_drone(3, 650, 250, z_val);

            // Take off Drone 1
            basicProtocolModule->set_velocity(1, 1,1, 0, 0, 10);
            basicProtocolModule->set_acceleration(1,1, 1, 5);
            basicProtocolModule->height_checker(250, 250, z_val);
            basicProtocolModule->take_off(1, 1,1, 250, 250, z_val+200);      // Drone 1 takes off to 10m altitude
            //basicProtocolModule->take_off(1, 1, 150, 0, 10);     // Move Drone 1 forward until it reaches 150 meters
            cout << "Drone took off and started moving...\n";
            //Start_Flag = false;
            //delete(testMsg);
            moveEV = new cMessage("moveTest");
            scheduleAt(simTime()+20, moveEV);
        }
        else if(msg == moveEV){
            cout << "MoveEV for drone 1.\n";
            basicProtocolModule->set_velocity(1, 1,1, 5, 5, 5);
            basicProtocolModule->set_acceleration(1,1, 1, 1);
            basicProtocolModule->height_checker(2500, 2500, z_val);
            basicProtocolModule->move_to(1,1, 1,2500, 2500, z_val+200);
            testMsg2 = new cMessage("moveTest2");
            scheduleAt(simTime()+10, testMsg2);
            //delete(moveEV);
        }
        else if(msg == moveEV2){
           cout << "MoveEV for drone 2.\n";
           basicProtocolModule->set_velocity(1, 2,2, 5, 5, 5);
           basicProtocolModule->set_acceleration(1,2, 2, 1);
           basicProtocolModule->height_checker(3000, 1500, z_val);
           basicProtocolModule->move_to(3,1, 2,3000, 1500, z_val+200);
           basicProtocolModule->set_velocity(1, 3,3, 1, 1, 1);
           basicProtocolModule->set_acceleration(1,3, 3, 1);
           basicProtocolModule->height_checker(650, 250, z_val);
           basicProtocolModule->take_off(2, 1,3, 650, 250, z_val+50);      // Drone 1 takes off to 10m altitude
           basicProtocolModule->set_base(2,2, 3,23, 23, 0);
           //delete(moveEV);
       }
        else if(msg == landingEV){
            basicProtocolModule->stop_drone(1,1, 1);
            landingEV2 = new cMessage("landingEV2");
            landingEV2->addPar("State") = "landingEV2";
            scheduleAt(simTime()+5, landingEV2);
        }
        else if(msg == landingEV2){
            basicProtocolModule->set_velocity(1, 1,1, 0, 0, -1);
            basicProtocolModule->set_acceleration(1,1, 1, 1);
            basicProtocolModule->land_drone(1, 1,1);
            landingEV3 = new cMessage("landingEV3");
            landingEV3->addPar("State") = "landingEV3";
            scheduleAt(simTime()+210, landingEV3);
        }
        else if(msg == landingEV3){
                basicProtocolModule->powerOff_drone(1, 1,1);
                basicProtocolModule->stop_drone(1,2, 2);
                basicProtocolModule->set_velocity(1, 2,2, -5, -5, -5);
                basicProtocolModule->set_acceleration(1,2, 2, 5);
                basicProtocolModule->return_to_base(1,2,2);
            }
        else{
            basicProtocolModule->set_velocity(1, 2,2, 5, 5, 5);
            basicProtocolModule->set_acceleration(1,2, 2, 1);
            basicProtocolModule->height_checker(450, 250, z_val);
            basicProtocolModule->take_off(3, 1,2, 450, 250, z_val+200);      // Drone 1 takes off to 10m altitude
            moveEV2 = new cMessage("moveTest3");
            scheduleAt(simTime()+10, moveEV2);
            cout << "TakeOff message via TCP was created.\n";
        }
}

void UserLayer::finish(){
    delete(testMsg);
    delete(moveEV);
    delete(moveEV2);
    delete(testMsg2);
}
