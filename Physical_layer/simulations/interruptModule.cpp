/*
 * interruptModule.cpp
 *
 *  Created on: May 14, 2025
 *      Author: gekaizum
 */
#include "interruptModule.h"


void interruptModule::initialize(){
    chooseOpt = par("chooseOpt");
    interruptSc = new cMessage("userInterrupt");
    interruptSc->addPar("chooseOpt") = chooseOpt;
    scheduleAt(simTime(), interruptSc);
}

void interruptModule::handleMessage(cMessage *msg){
    if( msg->par("chooseOpt").boolValue() ){
        cModule* appModule = getParentModule()->getSubmodule("drones", 0)->getSubmodule("droneControl");
        interruptSc2 = new cMessage("interRev");
        interruptSc2->addPar("State") = "INTEREV";
        sendDirect(interruptSc2, appModule,"tcpAppIn");
    }
    deleteMe = new cMessage("deleteMe");
    deleteMe->addPar("State") = "DELME";
    cModule* appModule2 = getParentModule()->getSubmodule("simControl");
    sendDirect(deleteMe, appModule2,"additInput");
}
void interruptModule::finish(){
    delete(interruptSc);
}
