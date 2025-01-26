// Include OMNeT++ library for simulation-specific features
#include <omnetpp.h>
// Include INET's TCP application base for managing TCP communication
#include <string>
#include <array>
#include "Drone.h"

using namespace omnetpp;
//using namespace inet;

Define_Module(Drone);

void Drone::initialize() {
    //TcpAppBase::initialize(stage);
    //if (stage == INITSTAGE_LOCAL) {
        state = POWER_ON;                                   // Set initial state
        collision_detection_mode = true;                    // Enable collision detection
        Current_Position[0] = 0;                            // Set default position to (0, 0, 0)
        Current_Position[1] = 0;
        Current_Position[2] = 0;
        Drone_ID = par("Drone_ID");                         // Get the drone's ID from parameters
        EV << "Drone " << Drone_ID << " initialized in POWER_ON state with position (0, 0, 0)." << endl;
        hoveringCurrent = calculateHoveringCurrent(mass_of_drone+additional_mass, battery_voltage, battery_efficiency);
        if (Drone_ID==1){
            cMessage *myMsg = new cMessage("TAKEOFF");
            //cMessage *batteryCalcResponse = new cMessage("BatteryCalcResponse");
            //myMsg->addPar("TAKEOFF") = batteryCalcResponse->par("battery_remain");
            //myMsg->addPar("TAKEOFF")=TAKEOFF;
            send(myMsg, "socketOut");
            //delete myMsg;
            //myMsg=nullptr;
            // Clean up simulated response
            //delete batteryCalcResponse;
            //TAKEOFF = new cMessage("TAKEOFF");
            //scheduleAt(simTime() + 1.0, TAKEOFF);  // Schedule for 1s after start
        }
    //}
}

void Drone::handleMessage(cMessage *msg) {
    if (strcmp(msg->getName(), "TAKEOFF") == 0) {
        EV << "Drone " << getName() << " is taking off at t=" << simTime() << endl;
        // Safely delete the event
        delete msg;
        msg = nullptr;
    }
    cMessage *myMsg = new cMessage("TAKEOFF");
    switch (state) {
        case POWER_ON:
            handlePowerOn(myMsg);
            break;
        case WAITING_FOR_TAKEOFF:
            handleWaitingForTakeoff(myMsg);
            break;
        case DRONE_IN_AIR:
            handleDroneInAir();
            break;
        case WAITING_FOR_COMMANDS:
            handleWaitingForCommands();
            break;
        case RETURNING_TO_BASE:
            handleReturningToBase();
            break;
        case NON_OPERATIONAL:
            handleNonOperational();
            break;
    }
    delete msg; // Clean up the message after processing
}

void Drone::handleWaitingForTakeoff(cMessage *msg) {
    EV << "Drone is waiting for TAKEOFF sequence." << endl;

    if (std::string(msg->getName()) == "TAKEOFF") {         // Check for TAKEOFF signal
        EV << "TAKEOFF signal received. Drone is passing to DRONE_IN_AIR state." << endl;
        state = DRONE_IN_AIR;                                // Transition to DRONE_IN_AIR state
        //For debug use only
        send(msg, "socketOut");
        ///////////////////////////////
    } else {
        EV << "Waiting for TAKEOFF signal. Received: " << msg->getName() << endl;
    }
}

void Drone::handleDroneInAir() {
    EV << "Drone is in the air." << endl;
    state = WAITING_FOR_COMMANDS;                            // Transition to WAITING_FOR_COMMANDS state
}

void Drone::handlePowerOn(cMessage *msg) {
    EV << "Drone is powering on." << endl;
    state = WAITING_FOR_TAKEOFF;                             // Transition to WAITING_FOR_TAKEOFF state
    send(msg, "socketOut");
}

void Drone::handleWaitingForCommands() {
    EV << "Drone is waiting for operational commands." << endl;
    std::string command = "MOVE";                           // Placeholder for receiving commands

    if (command == "MOVE") {
        executeMoveCommand();                                // Execute move command and check for collisions
    } else if (command == "RETURN") {
        state = RETURNING_TO_BASE;                           // Transition to RETURNING_TO_BASE state
    } else {
        EV << "Unknown command received: " << command << endl;
    }
}

void Drone::handleReturningToBase() {
    EV << "Drone is returning to base station." << endl;
    state = WAITING_FOR_TAKEOFF;                             // Transition to WAITING_FOR_TAKEOFF state
}

void Drone::handleNonOperational() {
    EV << "Drone is non-operational due to collision." << endl;
}

bool Drone::checkCollision() {
    EV << "Checking for collisions..." << endl;
    // Placeholder collision detection logic
    return false; // Update with real collision detection logic
}

void Drone::executeMoveCommand() {
    EV << "Executing move command." << endl;

    if (checkCollision()) {
        EV << "Collision detected! Drone transitioning to NON_OPERATIONAL state." << endl;
        state = NON_OPERATIONAL;                             // Transition to NON_OPERATIONAL state
    } else {
        EV << "Move command executed successfully." << endl;
    }
}

void Drone::broadcast(const std::string& message) {
    EV << "Drone " << Drone_ID << " broadcasting message: " << message << endl;

    cMessage *msg = new cMessage(message.c_str());           // Create a new message for broadcasting
    send(msg, "socketOut");                                 // Send the message via the output socket
}

void Drone::sendTo(int target_id, const std::string& message) {
    EV << "Drone " << Drone_ID << " sending message to Drone " << target_id << ": " << message << endl;

    cMessage *msg = new cMessage(message.c_str());           // Create a new message for direct communication
    send(msg, "socketOut");                                 // Send the message via the output socket
}
