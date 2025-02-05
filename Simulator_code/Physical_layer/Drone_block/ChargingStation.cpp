/*
 * ChargingStation.cpp
 *
 *  Created on: Feb 5, 2025
 *      Author: gekaizum
 */
#include "ChargingStation.h"

void ChargingStation :: initialize(){ // Initializes the ChargingStation module
    //char communication_type;              // Communication type ('W' for WiFi, 'R' for Radio)
    frequencyHz = par("frequencyHz");                     // Frequency used by drone to communicate
}
void ChargingStation :: handleMessage(cMessage *msg){ // Handles incoming messages

}
void ChargingStation :: finish(){

}


