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
    EV << "Simulation control online" << endl;
    int res = system("python3 ../Map_block/python_map_generator.py");
    EV << "New map created" << endl;
    testInit = new cMessage("testInit");
    testInit->addPar("State") = "POWER_ON";
    testInit->addPar("x") = 1;
    testInit->addPar("y") = 1;
    testInit->addPar("z") = 0;
    scheduleAt(simTime() + 2.0, testInit);
}
void SimulationControl::handleMessage(cMessage *msg){ // Handles incoming messages
    if (strcmp(msg->par("State").stringValue(), "POWER_ON") == 0){
        send(msg, "socketOut");
        EV << "POWER_ON signal sent to drone 1" << endl;
        testInit2 = new cMessage("testInit2");
        testInit2->addPar("State") = "POWER_ON";
        testInit2->addPar("x") = 1;
        testInit2->addPar("y") = 1;
        testInit2->addPar("z") = 0;
        scheduleAt(simTime() + 40.0, testInit2);
    }
    else{
        EV << "Unknown message: " << msg->getName() << endl;
    }
    //delete msg;
}
void SimulationControl::simControl_mainFunc(){}
void SimulationControl::finish(){}



