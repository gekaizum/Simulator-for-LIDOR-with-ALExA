/*
 * This file defines module drone, it is general wireless host that can be configured for different protocols.
 * Each drone have its base station.
 */

#include "DroneControl.h"

/*  Example of CMessage
	It is a constant structure
cMessage *msg = new cMessage("DroneData");
msg->addPar("state") = "NAVIGATION";
msg->addPar("x") = 12.5f;
msg->addPar("y") = 8.3f;
msg->addPar("z") = 0;
send(msg, "out");
*/

////////////////////FSM functions///////////////////////////////////////////////////////////////////
void DroneControl::initialize(int stage) {
    ApplicationBase::initialize(stage);  // Call base class initialization
    if (stage == inet::INITSTAGE_LOCAL) {
        Drone_ID = par("Drone_ID");                         // Get the drone's ID from parameters
        fileName = "Drone_logs/Drone_" + std::to_string(Drone_ID) + "_LogFile.log";
        droneLogFile.open(fileName, std::ios::out);  // Create/open log file
        if (!droneLogFile.is_open()) {
            EV << "Error: Could not open log file!" << endl;
        } else {
            droneLogFile << simTime() << ": Simulation start" << "\n";
            droneLogFile.flush();
        }
        droneLogFile << simTime() << ": Drone: Local initialization" << endl;
        state = INITSTAGE_LOCAL_DRONE;                            // Set initial state
        Current_Position[0] = 0;                            // Set default position to (0, 0, 0)
        Current_Position[1] = 0;
        Current_Position[2] = 0;
        Destination[0] = 0;
        Destination[1] = 0;
        Destination[2] = 0;
        Idle_Steps[0] = 1;
        Idle_Steps[1] = 1;
        Idle_Steps[2] = 1;
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
        time_step = par("time_step");

        droneLogFile << simTime() << ": Drone " << Drone_ID << " initialized in INITSTAGE_LOCAL state with position (0, 0, 0)." << endl;
        this->hoveringCurrent = calculateHoveringPower(mass_of_drone+additional_mass, motor_efficiency);
        droneLogFile << simTime() << ": Drone " << Drone_ID << " starting with: "<< mass_of_drone+additional_mass << " - total mass, " << motor_efficiency << " motor efficiency" << endl;
        droneLogFile << simTime() << ": Drone " << Drone_ID << " power consumption for hovering: "<< hoveringCurrent << endl;
    }
    else if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
        droneLogFile << simTime() << ": Drone: Application initialization" << endl;
    }
}

void DroneControl::handleMessageWhenUp(cMessage *msg) {
    if(state == NON_OPERATIONAL || non_operational == true){
        handleNonOperational(msg);
    }
    else if (power_on==true && in_air==true) { // set of states when drone is in air
		switch (state) {
			case WAITING_FOR_COMMANDS:
				handleWaitingForCommands(msg);
				break;
			case RETURNING_TO_BASE:
				handleReturningToBase(msg);
				break;
		}
	}
	else if (power_on==true){//set of states where drone operates from ground as access point
		switch (state) {
			case WAITING_FOR_TAKEOFF:
				handleWaitingForTakeoff(msg);
				break;
		}
	}
	else if (strcmp(msg->par("State").stringValue(), "POWER_ON") == 0) {
	    droneLogFile << simTime() << ": Drone " << getName() << " received POWER_ON command" << endl;
        Current_Position[0] = msg->par("x").doubleValue();
        Current_Position[1] = msg->par("y").doubleValue();
        Current_Position[2] = msg->par("z").doubleValue();
        Destination[0] = Current_Position[0];
        Destination[1] = Current_Position[1];
        Destination[2] = Current_Position[2];
        droneLogFile << simTime() << ": Drone " << Drone_ID << " initial coordinates updated: x=" << Current_Position[0] <<
        ", y=" << Current_Position[1] << ", z=" << Current_Position[2] << endl;
        state = POWER_ON;
        power_on=true;
        handlePowerOn();
        delete(msg);
    }
}

void DroneControl::handlePowerOn() {
    droneLogFile << simTime() << ": Drone is powering on and waiting for TAKEOFF sequence." << endl;
    state = WAITING_FOR_TAKEOFF; // Transition to WAITING_FOR_TAKEOFF state
	batteryCheckEvent = new cMessage("batteryCheckEvent");
	batteryCheckEvent->addPar("State") = "batteryCheckEvent";
    scheduleAt(simTime() + 10.0, batteryCheckEvent);
    /*cModule* mobilityMod = getParentModule()->getSubmodule("mobility");
    PublicStationaryMobility* mobility = check_and_cast<PublicStationaryMobility*>(mobilityMod);
    Coord newPos(Current_Position[0], Current_Position[1], Current_Position[2]);
    mobility->setPositionPublic(newPos);*/
    //nonOperationalEvent = new cMessage("nonOperationalEvent");
    //nonOperationalEvent->addPar("State") = "NON_OPERATIONAL";
    //scheduleAt(simTime() + 1.0, nonOperationalEvent);
}

void DroneControl::handleWaitingForTakeoff(cMessage *msg) {
    if (strcmp(msg->par("State").stringValue(), "batteryCheckEvent") == 0) {
        int time_step = 10;
        batteryCheckHelper(time_step);
        scheduleAt(simTime() + 10.0, msg); // Repeat
    }
    else if(msg->getArrivalGate() == gate("tcpRequestIn")){
        cModule *appModule = getParentModule()->getSubmodule("app", 1);
        sendDirect(msg, appModule,"tcpAppIn");                                 // Send the message via the output socket
        droneLogFile << simTime() << ": Drone " << Drone_ID << " send message via DroneTcpApp: " << msg << endl;
    }
    else if(msg->getArrivalGate() == gate("udpRequestIn")){
        cModule *appModule = getParentModule()->getSubmodule("app", 2);
        sendDirect(msg, appModule,"udpAppIn");                                 // Send the message via the output socket
        droneLogFile << simTime() << ": Drone " << Drone_ID << " send message via DroneUdpApp: " << msg << endl;
    }
    else if (strcmp(msg->par("State").stringValue(), "TAKEOFF") == 0) {// Check for TAKEOFF signal
        droneLogFile << simTime() << ": TAKEOFF signal received. Drone is passing to DRONE_IN_AIR state." << endl;
        msg->par("x") = Current_Position[0];
        msg->par("y") = Current_Position[1];
        msg->par("z") = Current_Position[2];
        handleMove(msg);
        batteryCheckHelper_forMove();
        in_air=true;
        Is_Moving = true;
        Next_Move = 3;//Take off will always start from moving on z axis
        state = WAITING_FOR_COMMANDS; // Transition to DRONE_IN_AIR state
        delete(msg);
    }
    else if (strcmp(msg->par("State").stringValue(), "SETACCEL") == 0){
        droneLogFile << simTime() << ": SETACCEL signal received. Acceleration parameters changed." << endl;
        handleSetAcceleration(msg);
        delete(msg);
    }
    else if (strcmp(msg->par("State").stringValue(), "SETVEL") == 0){
        droneLogFile << simTime() << ": SETVEL signal received. Velocity parameters changed." << endl;
        handleSetVelocity(msg);
        delete(msg);
    }
	else {
	    droneLogFile << simTime() << ": Wrong command. Waiting for TAKEOFF signal. Received: " << msg->getName() << endl;
	    delete(msg);
    }
}

void DroneControl::handleWaitingForCommands(cMessage *msg) {
	if (strcmp(msg->par("State").stringValue(), "batteryCheckEvent") == 0) {
	    //time_step = 10;
		batteryCheckHelper(time_step);
		scheduleAt(simTime() + 10.0, msg); // Repeat
	}
	else if(msg->getArrivalGate() == gate("tcpRequestIn")){
        cModule *appModule = getParentModule()->getSubmodule("app", 1);
        sendDirect(msg, appModule,"tcpAppIn");                                 // Send the message via the output socket
        droneLogFile << simTime() << ": Drone " << Drone_ID << " send message via DroneTcpApp: " << msg << endl;
    }
	else if(msg->getArrivalGate() == gate("udpRequestIn")){
        cModule *appModule = getParentModule()->getSubmodule("app", 2);
        sendDirect(msg, appModule,"udpAppIn");                                 // Send the message via the output socket
        droneLogFile << simTime() << ": Drone " << Drone_ID << " send message via DroneUdpApp: " << msg << endl;
    }
	else if (strcmp(msg->par("State").stringValue(), "SETACCEL") == 0){
        droneLogFile << simTime() << ": SETACCEL signal received. Acceleration parameters changed." << endl;
        handleSetAcceleration(msg);
        delete(msg);
    }
    else if (strcmp(msg->par("State").stringValue(), "SETVEL") == 0){
        droneLogFile << simTime() << ": SETVEL signal received. Velocity parameters changed." << endl;
        handleSetVelocity(msg);
        delete(msg);
    }
    else if (strcmp(msg->par("State").stringValue(), "MOVE") == 0) {
        Is_Moving = false;
        handleMove(msg);
		Is_Moving = true;
		Next_Move = 1;
		batteryCheckHelper_forMove();
		delete(msg);
    } 
	else if (strcmp(msg->par("State").stringValue(), "RETURN") == 0) {
        state = RETURNING_TO_BASE;
    }
	else if (strcmp(msg->par("State").stringValue(), "STOP") == 0) {
        Is_Moving = false;
        Next_Move = 1;
        Destination[0] = Current_Position[0];
        Destination[1] = Current_Position[1];
        Destination[2] = Current_Position[2];
        droneLogFile << simTime() << ": Drone " << Drone_ID << " was stoped at: x=" << Current_Position[0] <<
                ", y=" << Current_Position[1] << ", z=" << Current_Position[2] << endl;
        delete(msg);
    }
	else if (strcmp(msg->par("State").stringValue(), "SETBASE") == 0){
	    handleSetBase(msg);
	    delete(msg);
    }
	else {
	    droneLogFile << simTime() << ": Unknown command received: " << msg << endl;
	    delete(msg);
    }
}

void DroneControl::handleReturningToBase(cMessage *msg) {
    droneLogFile << simTime() << ": Drone is returning to base station." << endl;
    state = WAITING_FOR_TAKEOFF;                             // Transition to WAITING_FOR_TAKEOFF state
}

void DroneControl::handleNonOperational(cMessage *msg) {
    if (non_operational == true && state != NON_OPERATIONAL) {
        state = NON_OPERATIONAL;
        droneLogFile << simTime() << ": Drone" << Drone_ID << " is non-operational due to collision. State was changed to: NON_OPERATIONAL" << endl;
        droneLogFile << simTime() << getParentModule()->getSubmodule("wlan",0)->getSubmodule("radio") << endl;
        for (int i = 0; i < getParentModule()->getSubmodule("wlan",0)->getNumParams(); ++i) {
                cPar& par = getParentModule()->getSubmodule("wlan",0)->par(i);
            droneLogFile << "Parameter: " << par.getFullName() << " = " << par.str() << endl;
        }
        //getParentModule()->getSubmodule("wlan",0)->



    }
    else if (non_operational == true && state == NON_OPERATIONAL) {
        droneLogFile << simTime() << ": Drone " << Drone_ID << " is non-operational due to collision. New message received and ignored: " << msg->getName() << endl;
    }
}

void DroneControl::finish() {
    droneLogFile << simTime() << ": Finish function was called." << endl;
    droneLogFile << simTime() << ": Drone " << Drone_ID << " current position: x=" << Current_Position[0] <<
                    ", y=" << Current_Position[1] << ", z=" << Current_Position[2] << endl;
    if (batteryCheckEvent != nullptr) {
        if (batteryCheckEvent->isScheduled()) {
            cancelEvent(batteryCheckEvent);  // cancel first if still in event queue
        }
        delete batteryCheckEvent;
    }
    droneLogFile.close();
}

/////////////additional functions///////////////////////////////////////////////////////////////////
void DroneControl::batteryCheckHelper(int time_step){
    droneLogFile << simTime() << ": Battery check event" << endl;
	double battery_remain_joules = battery_remain * battery_voltage * 3600 / 1000;
	double remainingCapacity = updateBatteryCapacity(battery_remain_joules, 0, true, sensor_power,
								additional_power, hoveringCurrent, time_step); // in Joules
	battery_remain = (remainingCapacity*1000)/(battery_voltage*3600); // converting Joules to mAh
	droneLogFile << simTime() << ": Drone " << Drone_ID << " remaining power: " << battery_remain << " mAh, "
	<< (battery_remain/battery_capacity)*100 << "%" <<endl;
}

void DroneControl::batteryCheckHelper_forMove(){
        double distance = sqrt(std::pow(Destination[0]-Current_Position[0], 2)+std::pow(Destination[1]-Current_Position[1], 2)+std::pow(Destination[2]-Current_Position[2], 2));
        double battery_remain_joules = battery_remain * battery_voltage * 3600 / 1000; //battery remain in Joules
        double total_velocity = std::sqrt(x_velocity*x_velocity+y_velocity*y_velocity+z_velocity*z_velocity); // Total velocity
        double energyConsumption = calculateTotalCurrent(mass_of_drone+additional_mass, acceleration, total_velocity, distance); // Energy needs for move
        double remainingCapacity = updateBatteryCapacity(battery_remain_joules, energyConsumption, false, 0, 0, 0, total_velocity/acceleration); // in Joules
        battery_remain = (remainingCapacity*1000)/(battery_voltage*3600); // converting Joules to mAh
        droneLogFile << simTime() << ": Drone " << Drone_ID << " remaining power after move: " << battery_remain << " mAh, "
            << (battery_remain/battery_capacity)*100 << "%" <<endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void DroneControl::handleSetAcceleration(cMessage *msg){
    acceleration = msg->par("acceleration").doubleValue();
    droneLogFile << simTime() << ": Drone " << Drone_ID << " acceleration set to: " << acceleration << " m/sec" << endl;
}
void DroneControl::handleSetVelocity(cMessage *msg){
    x_velocity = msg->par("x_vel").doubleValue();
    y_velocity = msg->par("y_vel").doubleValue();
    z_velocity = msg->par("z_vel").doubleValue();
    droneLogFile << simTime() << ": Drone " << Drone_ID << " velocity values updated: x_velocity=" << x_velocity <<
        ", y_velocity=" << y_velocity << ", z_velocity=" << z_velocity << endl;
}
void DroneControl::handleMove(cMessage *msg){
    Destination[0] = msg->par("x").doubleValue();
    Destination[1] = msg->par("y").doubleValue();
    Destination[2] = msg->par("z").doubleValue();
    droneLogFile << simTime() << ": Drone " << Drone_ID << " destination coordinates updated: x=" << Destination[0] <<
    ", y=" << Destination[1] << ", z=" << Destination[2] << endl;
}
void DroneControl::handleSetBase(cMessage *msg){
    ChargeStationCoord[0] = msg->par("x").doubleValue();
    ChargeStationCoord[1] = msg->par("y").doubleValue();
    ChargeStationCoord[2] = msg->par("z").doubleValue();
    droneLogFile << simTime() << ": Drone " << Drone_ID << " base station coordinates updated: x=" << ChargeStationCoord[0] <<
    ", y=" << ChargeStationCoord[1] << ", z=" << ChargeStationCoord[2] << endl;
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
