#include "ChargingStationManager.h"

ChargingStationManager::ChargingStationManager(cModule* parent, int numOfChargeStation, int nSid) {
    parentModule = parent;
    nextStationId = nSid;
    ChargingStation_data.clear();
    for (int i = 0; i < numOfChargeStation; i++) {
        cModule* ChargingStationModule = parentModule->getSubmodule("ChargingStation", i);
        if (ChargingStationModule) {
            ChargingStation* ChStation = check_and_cast<ChargingStation*>(ChargingStationModule);
            ChargingStation_data.push_back(ChStation);
        } else {
            EV << "Warning: ChargingStation submodule " << i << " not found!" << endl;
        }
    }
}

ChargingStationManager::~ChargingStationManager() {
    ChargingStation_data.clear();
}

void ChargingStationManager::addStation(int x, int y, int z) {
    int id = nextStationId++;
    ChargingStation* newStation = new ChargingStation();
    newStation->ChargingStation_ID=id;
    newStation->Current_Position[0] = x;
    newStation->Current_Position[0] = y;
    newStation->Current_Position[0] = z;

    // Create a new ChargingStation module
    cModuleType *ChargingStationType = cModuleType::get("Physical_layer.Drone_block.ChargingStation");
    if (!ChargingStationType) {
        EV << "Error: ChargingStation module type not found!" << endl;
        return;
    }

    // Generate a unique name for the drone
    std::string ChargingStationName = "Charging_Station_" + std::to_string(id);

    // Create the drone module
    cModule *ChargingStation = ChargingStationType->create(ChargingStationName.c_str(), parentModule);
    ChargingStation->finalizeParameters();  // Finalize before adding to simulation
    ChargingStation->buildInside();  // Necessary for compound modules
    ChargingStation->scheduleStart(simTime());  // Ensure the module starts running

    ChargingStation_data.push_back(newStation);
    EV << "Added charging station ID: " << id << " at (" << x << ", " << y << ", " << z << endl;
}

bool ChargingStationManager::removeStation(int corrected_id) { //argument will be ChargingStation_ID - numDrones
    if (corrected_id >= 0 && corrected_id < ChargingStation_data.size()) {
        delete ChargingStation_data[corrected_id];
        ChargingStation_data.erase(ChargingStation_data.begin() + corrected_id);
        EV << "Removed charging station ID: " << corrected_id << endl;
        return true;
    }
    EV << "Charging station ID: " << corrected_id << " not found" << endl;
    return false;
}

bool ChargingStationManager::moveStation(int id, int newX, int newY, int newZ) { //argument id will be ChargingStation_ID - numDrones
    if (id >= 0 && id < ChargingStation_data.size()) {
        ChargingStation_data[id]->Current_Position[0] = newX;
        ChargingStation_data[id]->Current_Position[1] = newY;
        ChargingStation_data[id]->Current_Position[2] = newZ;
        EV << "Moved charging station ID: " << id << " to (" << newX << ", " << newY << ", " << newZ << ")" << endl;
        return true;
    }
    EV << "Charging station ID: " << id << " not found" << endl;
    return false;
}

bool ChargingStationManager::getStationLocation(int id, int &x, int &y, int &z) { //argument id will be ChargingStation_ID - numDrones
    if (id >= 0 && id < ChargingStation_data.size()) {
        x = ChargingStation_data[id]->Current_Position[0];
        y = ChargingStation_data[id]->Current_Position[1];
        z = ChargingStation_data[id]->Current_Position[2];
        return true;
    }
    EV << "Charging station ID: " << id << " not found" << endl;
    return false;
}

void ChargingStationManager::printAllStations() {
    EV << "Charging Stations:" << endl;
    for (size_t i = 0; i < ChargingStation_data.size(); i++) {
        EV << "ID: " << i << " at (" << ChargingStation_data[i]->Current_Position[0] << ", " << ChargingStation_data[i]->Current_Position[1]
                                                << ", " << ChargingStation_data[i]->Current_Position[2] << ")" << endl;
    }
}
