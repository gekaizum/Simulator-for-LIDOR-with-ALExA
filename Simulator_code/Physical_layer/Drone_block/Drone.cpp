// Include OMNeT++ library for simulation-specific features
//#include <omnetpp.h>
// Include INET's TCP application base for managing TCP communication
//#include <string>
//#include <array>
#include "Drone.h"

//using namespace omnetpp;
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

//Define_Module(Drone);
////////////////////FSM functions///////////////////////////////////////////////////////////////////
void Drone::initialize() {
	state = INITSTAGE_LOCAL;                            // Set initial state
	//collision_detection_mode = true;                  // Enable collision detection
	Current_Position[0] = 0;                            // Set default position to (0, 0, 0)
	Current_Position[1] = 0;
	Current_Position[2] = 0;
	Destination[0] = 0;
	Destination[1] = 0;
	Destination[2] = 0;
	Idle_Steps[0] = 1;
	Idle_Steps[1] = 1;
	Idle_Steps[2] = 1;
	Drone_ID = par("Drone_ID");                         // Get the drone's ID from parameters
    number_of_rotors = par("number_of_rotors");
    motor_efficiency = par("motor_efficiency");
    mass_of_drone = par("mass_of_drone");
    additional_mass = par("additional_mass");
    frequencyHz = par("frequencyHz");
    antena_power = par("antena_power");
    sensor_power = par("sensor_power");
    additional_power = par("additional_power");
    battery_capacity = par("battery_capacity");
    battery_remain = par("battery_remain");
    battery_voltage = par("battery_voltage");
    drone_size = par("drone_size");
    acceleration = par("acceleration");
    x_velocity = par("x_velocity");
    y_velocity = par("y_velocity");
    z_velocity = par("z_velocity");
    fileName = "Drone_" + std::to_string(Drone_ID) + "_LogFile.log";
    droneLogFile.open(fileName, std::ios::out);  // Create/open log file
    if (!droneLogFile.is_open()) {
        EV << "Error: Could not open log file!" << endl;
    } else {
        droneLogFile << "Simulation started at: " << simTime() << "\n";
        droneLogFile.flush();
    }
    droneLogFile << "Drone " << Drone_ID << " initialized in INITSTAGE_LOCAL state with position (0, 0, 0)." << endl;
	this->hoveringCurrent = calculateHoveringPower(mass_of_drone+additional_mass, motor_efficiency);
	droneLogFile << "Drone " << Drone_ID << " starting with: "<< mass_of_drone+additional_mass << " - total mass, " << motor_efficiency << " motor efficiency" << endl;
	droneLogFile << "Drone " << Drone_ID << " power consumption for hovering: "<< hoveringCurrent << endl;
}

void Drone::handleMessage(cMessage *msg) {
	if (power_on==true && in_air==true) {
		switch (state) {
			case WAITING_FOR_COMMANDS:
				handleWaitingForCommands(msg);
				break;
			case RETURNING_TO_BASE:
				handleReturningToBase(msg);
				break;
			case NON_OPERATIONAL:
				handleNonOperational(msg);
				break;
		}
	}
	else if (power_on==true){//here will be set of states where drone operates from ground as access point
		switch (state) {
			case WAITING_FOR_TAKEOFF:
				handleWaitingForTakeoff(msg);
				break;
		}
	}
	else if (strcmp(msg->par("State").stringValue(), "POWER_ON") == 0) {
	    droneLogFile << "Drone " << getName() << " received POWER_ON command at t=" << simTime() << endl;
        Current_Position[0] = msg->par("x").doubleValue();
        Current_Position[1] = msg->par("y").doubleValue();
        Current_Position[2] = msg->par("z").doubleValue();
        Destination[0] = Current_Position[0];
        Destination[1] = Current_Position[1];
        Destination[2] = Current_Position[2];
        droneLogFile << "Drone " << Drone_ID << " initial coordinates updated: x=" << Current_Position[0] <<
        ", y=" << Current_Position[1] << ", z=" << Current_Position[2] << endl;
        state = POWER_ON;
        power_on=true;
        handlePowerOn();
    }
    //delete msg; // Clean up the message after processing
}

void Drone::handlePowerOn() {
    droneLogFile << "Drone is powering on and waiting for TAKEOFF sequence." << endl;
    state = WAITING_FOR_TAKEOFF; // Transition to WAITING_FOR_TAKEOFF state
	batteryCheckEvent = new cMessage("batteryCheckEvent");
	batteryCheckEvent->addPar("State") = "batteryCheckEvent";
    scheduleAt(simTime() + 10.0, batteryCheckEvent);
    //delete msg;
}

void Drone::handleWaitingForTakeoff(cMessage *msg) {
    if (strcmp(msg->par("State").stringValue(), "batteryCheckEvent") == 0) {
        int time_step = 10;
        batteryCheckHelper(time_step);
        scheduleAt(simTime() + 10.0, msg); // Repeat
    }
    else if (strcmp(msg->par("State").stringValue(), "TAKEOFF") == 0) {// Check for TAKEOFF signal
        droneLogFile << "TAKEOFF signal received. Drone is passing to DRONE_IN_AIR state." << endl;
		in_air=true;
		Is_Moving = true;
		Next_Move = 3;//Take off will always start from moving on z axis
        state = WAITING_FOR_COMMANDS; // Transition to DRONE_IN_AIR state
        Destination[0] = msg->par("x").doubleValue();
        Destination[1] = msg->par("y").doubleValue();
        Destination[2] = msg->par("z").doubleValue();
        droneLogFile << "Drone " << Drone_ID << " destination coordinates updated: x=" << Destination[0] <<
		", y=" << Destination[1] << ", z=" << Destination[2] << endl;
		acceleration = msg->par("acceleration").doubleValue();
        x_velocity = msg->par("x_velocity").doubleValue();
        y_velocity = msg->par("y_velocity").doubleValue();
        z_velocity = msg->par("z_velocity").doubleValue();
        batteryCheckHelper_forMove();
        
    }
	else {
	    droneLogFile << "Wrong command. Waiting for TAKEOFF signal. Received: " << msg->getName() << endl;
    }
}

void Drone::handleWaitingForCommands(cMessage *msg) {
	if (strcmp(msg->par("State").stringValue(), "batteryCheckEvent") == 0) {
	    int time_step = 10;
		batteryCheckHelper(time_step);
		scheduleAt(simTime() + 10.0, msg); // Repeat
	}
    else if (strcmp(msg->par("State").stringValue(), "MOVE") == 0) {
		acceleration = msg->par("acceleration").doubleValue();
        x_velocity = msg->par("x_velocity").doubleValue();
		y_velocity = msg->par("y_velocity").doubleValue();
		z_velocity = msg->par("z_velocity").doubleValue();
		Destination[0] = msg->par("x").doubleValue();
		Destination[1] = msg->par("y").doubleValue();
		Destination[2] = msg->par("z").doubleValue();
		Is_Moving = true;
		Next_Move = 1;
		batteryCheckHelper_forMove();
    } 
	else if (strcmp(msg->par("State").stringValue(), "RETURN") == 0) {
        state = RETURNING_TO_BASE;
    }
	else if (strcmp(msg->par("State").stringValue(), "NON_OPERATIONAL") == 0){
		state = NON_OPERATIONAL;
	}
	else {
	    droneLogFile << "Unknown command received: " << msg << endl;
    }
}

void Drone::handleReturningToBase(cMessage *msg) {
    droneLogFile << "Drone is returning to base station." << endl;
    state = WAITING_FOR_TAKEOFF;                             // Transition to WAITING_FOR_TAKEOFF state
}

void Drone::handleNonOperational(cMessage *msg) {
    droneLogFile << "Drone is non-operational due to collision." << endl;
    droneLogFile << "Drone " << Drone_ID << " is non-operational due to collision. New message received and ignored." << endl;
    droneLogFile << "Message: " << msg->getName() << endl;
}

void Drone::finish() {
	//cancelAndDelete(batteryCheckEvent);
    droneLogFile.close();
}

/////////////additional functions///////////////////////////////////////////////////////////////////
void Drone::batteryCheckHelper(int time_step){
    droneLogFile << "Battery check event at " << simTime() << " sec." << endl;
	double battery_remain_joules = battery_remain * battery_voltage * 3600 / 1000;
	double remainingCapacity = updateBatteryCapacity(battery_remain_joules, 0, true, sensor_power,
								additional_power, hoveringCurrent, time_step); // in Joules
	battery_remain = (remainingCapacity*1000)/(battery_voltage*3600); // converting Joules to mAh
	droneLogFile << "Drone " << Drone_ID << " remaining power: " << battery_remain << " mAh, "
	<< (battery_remain/battery_capacity)*100 << "%" <<endl;
}

void Drone::batteryCheckHelper_forMove(){
        double distance = sqrt(std::pow(Destination[0]-Current_Position[0], 2)+std::pow(Destination[1]-Current_Position[1], 2)+std::pow(Destination[2]-Current_Position[2], 2));
        double battery_remain_joules = battery_remain * battery_voltage * 3600 / 1000; //battery remain in Joules
        double total_velocity = std::sqrt(x_velocity*x_velocity+y_velocity*y_velocity+z_velocity*z_velocity); // Total velocity
        double energyConsumption = calculateTotalCurrent(mass_of_drone+additional_mass, acceleration, total_velocity, distance); // Energy needs for move
        double remainingCapacity = updateBatteryCapacity(battery_remain_joules, energyConsumption, false, 0, 0, 0, total_velocity/acceleration); // in Joules
        battery_remain = (remainingCapacity*1000)/(battery_voltage*3600); // converting Joules to mAh
        droneLogFile << "Drone " << Drone_ID << " remaining power after move: " << battery_remain << " mAh, "
            << (battery_remain/battery_capacity)*100 << "%" <<endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void Drone::broadcast(const std::string& message) {
    droneLogFile << "Drone " << Drone_ID << " broadcasting message: " << message << endl;

    cMessage *msg = new cMessage(message.c_str());           // Create a new message for broadcasting
    send(msg, "socketOut");                                 // Send the message via the output socket
}

void Drone::sendTo(int target_id, const std::string& message) {
    droneLogFile << "Drone " << Drone_ID << " sending message to Drone " << target_id << ": " << message << endl;

    cMessage *msg = new cMessage(message.c_str());           // Create a new message for direct communication
    send(msg, "socketOut");                                 // Send the message via the output socket
}
