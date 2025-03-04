/*
 * User_layer.cpp
 *
 *  Created on: Feb 20, 2025
 *      Author: shaked
 */

#include "User_layer.h"

void UserLayer::initialize() { // Initializes the simulation and get access to basic protocol module
    basicProtocolModule = check_and_cast<BasicProtocol*>(getParentModule()->getSubmodule("basicProtocol"));
    // create and send Start self cMessage to start the simulation
    cMessage *startMsg = new cMessage("startMsg");
    startMsg->addPar("startMsg") = "startMsg";
    scheduleAt(simTime()+1, startMsg);
}

void UserLayer::handleMessage(cMessage *msg){ // Handles incoming messages
        if(strcmp(msg->par("startMsg").stringValue(), "startMsg") == 0){
            // Power on the drones
            basicProtocolModule->powerOn_drone(1, 0, 0, 0);
            basicProtocolModule->powerOn_drone(2, 0, 0, 0);
            basicProtocolModule->powerOn_drone(3, 0, 0, 0);
            basicProtocolModule->set_velocity(1, 1, 0, 0, 10); // set Drone1 velocity
            basicProtocolModule->take_off(1, 1, 0, 0, 300); // Drone1 takes off to 300m altitude
            cout << "Drone 1 took off and started moving...\n";

            simMsg->addPar("simMsg") = "simMsg";// create and send Simulation self cMessage
            scheduleAt(simTime()+10, simMsg);
        }
        if(strcmp(msg->par("simMsg").stringValue(), "simMsg") == 0){ //handles simulation cMessages
            basicProtocolModule->sendMsgTCP(2, 1); // TCP general data message
            basicProtocolModule->getDrone_position(1, 1, pos_array_1); //update the position array of Drone1
            basicProtocolModule->getDrone_position(1, 1, pos_array_3); //update the position array of Drone3 (the relay drone)

            if(pos_array_1[0] == 0 && pos_array_1[1] == 0 && pos_array_1[2] == 300){ //check if Drone1 is at z = 300m to start moving forward
                basicProtocolModule->set_velocity(1, 1, 5, 0, 0);
                basicProtocolModule->move_to(1, 1, 1000, 0, 300);   // Drone 1 start moving to the destination
            }
            if (pos_array_1[0] >= 250 && pos_array_3[0] == 0) { // Check if Drone 3 should take off as a relay
                basicProtocolModule->sendMsgTCP(1, 3); // TCP command message to drone 3 to start moving to {500,0,300}

                //simulates sending a cMessage from Drone3 to User_layer
                cMessage *Drone3Msg = new cMessage("Drone3Msg");
                Drone3Msg->addPar("Drone3Msg") = "Drone3Msg";
                scheduleAt(simTime()+1, Drone3Msg);

            }
            if(pos_array_3[2] == 300 && relayFlag == true){
                basicProtocolModule->set_velocity(1, 3, 10, 0, 0);
                basicProtocolModule->move_to(1, 3, 500, 0, 300);
                relayFlag = false;
                scheduleAt(simTime()+1, simMsg);
            }
            if(pos_array_1[0] == 1000){
                // Stop Drone1 when it reaches 1000 meters
                basicProtocolModule->stop_drone(1, 1);
                cout << "Drone 1 reached 1000 meters and stopped moving.\n";
                cMessage *simEndMsg = new cMessage("simEndMsg");
                simEndMsg->addPar("simEndMsg") = "simEndMsg";
                scheduleAt(simTime()+1, simEndMsg);
            }
        }
        if(strcmp(msg->par("Drone3Msg").stringValue(), "Drone3Msg") == 0){ //handles cMessages form drone3
            basicProtocolModule->set_velocity(1, 3, 0, 0, 10); // set Drone3 velocity
            basicProtocolModule->take_off(1, 3, 0, 0, 300); // Drone3 takes off to 300m altitude
            cout << "Drone3 took off as a relay\n";
        }
        if(strcmp(msg->par("simEndMsg").stringValue(), "simEndMsg") == 0){ //handles End of simulation cMessage
            basicProtocolModule->sendMsgTCP(1, 2); // Drone1 informs Drone2 via a TCP message that it has reached its destination
            /*
             * clean memory and close files here if needed
             */
            cout << "End of Simulation\n";
        }
}
