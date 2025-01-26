#include <omnetpp.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "ChargingStationManager.h"

using namespace omnetpp;

void ChargingStationManager::initialize() {
    nextStationId = 1; // Initialize the station ID counter
}

    // Add a new charging station at the specified coordinates
int ChargingStationManager::addStation(int x, int y) {
    int id = nextStationId++;
    stations[id] = {id, x, y};
    EV << "Added charging station ID: " << id << " at (" << x << ", " << y << ")\n";
    return id;
}

// Remove a charging station by its ID
bool ChargingStationManager::removeStation(int id) {
    if (stations.erase(id)) {
        EV << "Removed charging station ID: " << id << "\n";
        return true;
    }
    EV << "Charging station ID: " << id << " not found\n";
    return false;
}

// Move a charging station to new coordinates
bool ChargingStationManager::moveStation(int id, int newX, int newY) {
    auto it = stations.find(id);
    if (it != stations.end()) {
        it->second.x = newX;
        it->second.y = newY;
        EV << "Moved charging station ID: " << id << " to (" << newX << ", " << newY << ")\n";
        return true;
    }
    EV << "Charging station ID: " << id << " not found\n";
    return false;
}

// Get the coordinates of a charging station by its ID
bool ChargingStationManager::getStationLocation(int id, int &x, int &y) {
    auto it = stations.find(id);
    if (it != stations.end()) {
        x = it->second.x;
        y = it->second.y;
        return true;
    }
    EV << "Charging station ID: " << id << " not found\n";
    return false;
}

// Print all charging stations and their locations
void ChargingStationManager::printAllStations() {
    EV << "Charging Stations:\n";
    for (const auto &pair : stations) {
        const ChargingStation &station = pair.second;
        EV << "ID: " << station.id << " at (" << station.x << ", " << station.y << ")\n";
    }
}

Define_Module(ChargingStationManager);
