/*
 * BasicProtocolLogger.cpp
 *
 *  Created on: Feb 17, 2025
 *      Author: gekaizum
 */
#include "BasicProtocolLogger.h"
using namespace omnetpp;

BasicProtocolLogger::BasicProtocolLogger(){
    logFile.open("Sim_logs/BasicProtocolLog.log", std::ios::out);  // Create/open log file
    if (!logFile.is_open()) {
        EV << "Error: Could not open log file!" << endl;
    } else {
        logFile << "Simulation started at: " << simTime() << "\n";
        logFile.flush();
    }
}

BasicProtocolLogger::~BasicProtocolLogger(){
    logFile.close();
}



