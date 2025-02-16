/*
 * This file defines module charging station, it can be used as host based on ground or only as base station for drone landing and charging.
 * Any drone can be send to corresponding charging station.
 * Charging station can be moved manually, by user
 */

#include "ChargingStation.h"

void ChargingStation :: initialize(){                 // Initializes the ChargingStation module
    //char communication_type;                        // Communication type ('W' for WiFi, 'R' for Radio)
    frequencyHz = par("frequencyHz");                 // Frequency used by ChargingStation to communicate
}
void ChargingStation :: handleMessage(cMessage *msg){ // Handles incoming messages

}
void ChargingStation :: finish(){

}


