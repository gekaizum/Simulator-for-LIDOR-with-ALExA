/*
 * User_layer.cpp
 *
 *  Created on: Feb 20, 2025
 *      Author: shaked
 */

#include "User_layer.h"

void UserLayer::initialize() { // Initializes the drone module
//    UserLayer_Logger = new UserLayerLogger();
//    UserLayer_Logger->logFile.flush();
    //controlModule = check_and_cast<SimulationControl*>(getParentModule()->getSubmodule("simControl"));
    basicProtocolModule = check_and_cast<BasicProtocol*>(getParentModule()->getSubmodule("basicProtocol"));
//    UserLayer_Logger->logFile << "User layer online" << endl;
    cMessage *testMsg = new cMessage("testMsg");
    testMsg->addPar("State") = "testMsg";
    scheduleAt(simTime()+2, testMsg);
}

void UserLayer::handleMessage(cMessage *msg){ // Handles incoming messages
        if(Start_Flag){
            // Power on drones 1 and 2
            basicProtocolModule->powerOn_drone(1, 0, 0, 0);
            basicProtocolModule->powerOn_drone(2, 0, 0, 0);
            basicProtocolModule->powerOn_drone(3, 0, 0, 0);
            //basicProtocolModule->powerOn_drone(4, 0, 0, 0);

            // Take off Drone 1
            basicProtocolModule->set_velocity(1, 1, 0, 0, 5);
            basicProtocolModule->set_acceleration(1, 1, 1);
            basicProtocolModule->take_off(1, 1, 0, 0, 10);      // Drone 1 takes off to 10m altitude
            //basicProtocolModule->take_off(1, 1, 150, 0, 10);     // Move Drone 1 forward until it reaches 150 meters
            cout << "Drone 1 took off and started moving...\n";
            Start_Flag = false;
            scheduleAt(simTime()+10, msg);
        }
        else{
            basicProtocolModule->sendMsgTCP(1, 2);
            /*while(drone1_x < 150){
                basicProtocolModule->getDrone_position(1, 1, pos_array);
                drone1_x = pos_array[2];
                basicProtocolModule->getDrone_position(1, 2, pos_array);
                drone3_x = pos_array[2];
                basicProtocolModule->getDrone_position(1, 3, pos_array);
                drone4_x = pos_array[2];
                basicProtocolModule->sendMsgTCP(0, 1);
                // Simulate sending a TCP packet every second
                cout << "TCP packet sent from Drone1 to Drone2\n";

                // Check if Drone 3 should take off as a relay
                if (drone1_x >= 40 && drone3_x == 0) {

                    //********need to update: drone1 sends tcp to drone3 to takeoff
                    basicProtocolModule->set_velocity(1, 3, 1, 0, 0);
                    basicProtocolModule->take_off(1, 3, 0, 0, 10);
                    basicProtocolModule->move_to(1, 3, 40, 0, 10);     // Move to (10, 0, 40) and stay

                    cout << "Drone 3 took off as a relay\n";
                }

                // Check if Drone 4 should take off as a relay
                if (drone1_x >= 80 && drone4_x == 0) {
                    //********need to update: drone1 sends tcp to drone4 to takeoff and to drone3 to move to (10, 0, 80).
                    basicProtocolModule->set_velocity(1, 4, 1, 0, 0);
                    basicProtocolModule->take_off(1, 4, 0, 0, 10);
                    basicProtocolModule->move_to(1, 4, 40, 0, 10);      //Drone4 move and replace Drone3 in (10, 0, 40) and stay
                    basicProtocolModule->move_to(1, 3, 80, 0, 10);     // Drone3 Move to (10, 0, 80) and stay
                    cout << "Drone 4 took off as a relay\n";
                }
            }*/
            // Stop Drone 1 when it reaches 150 meters
            basicProtocolModule->stop_drone(1, 1);
            cout << "Drone 1 reached 150 meters and stopped moving.\n";
        }
}

