// Include OMNeT++ library for simulation-specific features
#include <omnetpp.h>
// Include INET's TCP application base for managing TCP communication
#include <string>
#include <array>
#include "Drone.h"

using namespace omnetpp;
//using namespace inet;

/*  Example of message
	It is a constant structure
cMessage *msg = new cMessage("DroneData");
msg->addPar("state") = "NAVIGATION";
msg->addPar("x") = 12.5f;
msg->addPar("y") = 8.3f;
msg->addPar("z") = 0;
send(msg, "out");
*/

Define_Module(Drone);
////////////////////FSM functions///////////////////////////////////////////////////////////////////
void Drone::initialize() {
	state = INITSTAGE_LOCAL;                            // Set initial state
	//collision_detection_mode = true;                    // Enable collision detection
	Current_Position[0] = 0;                            // Set default position to (0, 0, 0)
	Current_Position[1] = 0;
	Current_Position[2] = 0;
	Destination[0] = 0;
	Destination[1] = 0;
	Destination[2] = 0;
	Drone_ID = par("Drone_ID");                         // Get the drone's ID from parameters
	EV << "Drone " << Drone_ID << " initialized in INITSTAGE_LOCAL state with position (0, 0, 0)." << endl;
	hoveringCurrent = calculateHoveringCurrent(mass_of_drone+additional_mass, battery_voltage, battery_efficiency);
	EV << "Drone " << Drone_ID << "Power consumption for hovering: "<< hoveringCurrent << endl;
	////////////////This part for debug use only and will be deleted///////////////////
	if (Drone_ID==1){
		cMessage *myMsg = new cMessage("TAKEOFF");
		send(myMsg, "socketOut");
		//scheduleAt(simTime() + 1.0, TAKEOFF);  // Schedule for 1s after start
	}
	///////////////////////////////////////////////////////////////////////////////////
}

void Drone::handleMessage(cMessage *msg) {
    if (strcmp(msg->par("State").stringValue(), "POWER_ON") == 0) {
        EV << "Drone " << getName() << " received POWER_ON command at t=" << simTime() << endl;
		Current_Position[0] = msg->par("x").floatValue();
		Current_Position[1] = msg->par("y").floatValue();
		Destination[0] = Current_Position[0];
		Destination[1] = Current_Position[0];
		EV << "Drone " << getName() << " initial coordinates updated: x=" << Current_Position[0] <<
		", y=" << Current_Position[1] << ", z=0" << endl;
        delete msg;
        msg = nullptr;
		state = POWER_ON;
		power_on=true;
    }
	if (power_on==true && in_air==true) {
		switch (state) {
			case WAITING_FOR_COMMANDS:
				handleWaitingForCommands(msg);
				break;
			case RETURNING_TO_BASE:
				handleReturningToBase(msg);
				break;
			case NON_OPERATIONAL:
				handleNonOperational();
				break;
		}
	}
	else if (power_on==true){//here will be set of states where drone operates from ground as access point
		switch (state) {
			case POWER_ON:
				handlePowerOn(msg);
				break;
			case WAITING_FOR_TAKEOFF:
				handleWaitingForTakeoff(msg);
				break;
		}
	}
    delete msg; // Clean up the message after processing
}

void Drone::handlePowerOn(cMessage *msg) {
    EV << "Drone is powering on and waiting for TAKEOFF sequence." << endl;
    state = WAITING_FOR_TAKEOFF; // Transition to WAITING_FOR_TAKEOFF state
	batteryCheckEvent = new cMessage("batteryCheckEvent");
    scheduleAt(simTime() + 10.0, batteryCheckEvent);
}

void Drone::handleWaitingForTakeoff(cMessage *msg) {
    if (strcmp(msg->par("State").stringValue(), "TAKEOFF") == 0) {// Check for TAKEOFF signal
        EV << "TAKEOFF signal received. Drone is passing to DRONE_IN_AIR state." << endl;
		in_air==true;
        state = WAITING_FOR_COMMANDS; // Transition to DRONE_IN_AIR state
		Current_Position[2] = msg->par("z").floatValue();
		Destination[2] = Current_Position[2];
		EV << "Drone " << getName() << " initial coordinates updated: x=" << Current_Position[0] <<
		", y=" << Current_Position[1] << ", z=" << Current_Position[2] << endl;
        
    }
	else if (msg == batteryCheckEvent) {
		batteryCheckHelper();
		scheduleAt(simTime() + 10.0, batteryCheckEvent); // Repeat
	}
	else {
        EV << "Wrong command. Waiting for TAKEOFF signal. Received: " << msg->getName() << endl;
    }
}

void Drone::handleWaitingForCommands(cMessage *msg) {
	if (msg == batteryCheckEvent) {
		batteryCheckHelper();
		scheduleAt(simTime() + 10.0, batteryCheckEvent); // Repeat
	}
    else if (strcmp(msg->par("State").stringValue(), "MOVE") == 0) {
		acceleration = msg->par("acceleration").doubleValue();
        x_velocity = msg->par("x_velocity").doubleValue();
		y_velocity = msg->par("y_velocity").doubleValue();
		z_velocity = msg->par("z_velocity").doubleValue();
		Destination[0] = msg->par("x").floatValue();
		Destination[1] = msg->par("y").floatValue();
		Destination[2] = msg->par("z").floatValue();
		Is_Moving = true;
		Next_Move = 1;
    } 
	else if (strcmp(msg->par("State").stringValue(), "RETURN") == 0) {
        state = RETURNING_TO_BASE;
    }
	else if (strcmp(msg->par("State").stringValue(), "NON_OPERATIONAL") == 0){
		state = NON_OPERATIONAL;
	}
	else {
        EV << "Unknown command received: " << command << endl;
    }
}

void Drone::handleReturningToBase() {
    EV << "Drone is returning to base station." << endl;
    state = WAITING_FOR_TAKEOFF;                             // Transition to WAITING_FOR_TAKEOFF state
}

void Drone::handleNonOperational() {
    EV << "Drone is non-operational due to collision." << endl;
	EV << "Drone " << getName() << " is non-operational due to collision. New message received and ignored." << endl;
	EV << "Message: " << msg->getName() << endl;
}

virtual void finish() override {
	cancelAndDelete(batteryCheckEvent);
}

/////////////additional functions///////////////////////////////////////////////////////////////////
void batteryCheckHelper(){
	EV << "Battery check event at " << simTime() << " sec." << endl;
	double battery_remain_joules = battery_remain * battery_voltage * 3600 / 1000;
	double remainingCapacity = updateBatteryCapacity(battery_remain_joules, 0, true, sensor_power,
								additional_power, hoveringCurrent); // in Joules
	battery_remain = (remainingCapacity*1000)/(battery_voltage*3600); // converting Joules to mAh
	EV << "Drone " << getName() << " remaining power: " << battery_remain << " mAh, " 
	<< (battery_remain/battery_capacity)*100 << "%" <<endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
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
