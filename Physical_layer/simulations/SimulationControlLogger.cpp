/*
 * SimulationControlLogger.cpp
 *
 *  Created on: Feb 1, 2025
 *      Author: gekaizum
 */
#include "SimulationControlLogger.h"
using namespace omnetpp;

SimulationControlLogger::SimulationControlLogger(){
    logFile.open("Sim_logs/SimControlLog.log", std::ios::out);  // Create/open log file
    if (!logFile.is_open()) {
        EV << "Error: Could not open log file!" << endl;
    } else {
        logFile << "Simulation started at: " << simTime() << "\n";
        logFile.flush();
    }
}

SimulationControlLogger::~SimulationControlLogger(){
    logFile.close();
}


