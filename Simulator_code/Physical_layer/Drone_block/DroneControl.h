/*
 * This file defines module drone, it is general wireless host that can be configured for different protocols.
 * Each drone have its base station.
 */

#ifndef DRONE_H_
#define DRONE_H_

#include <omnetpp.h>
#include <string>
#include <array>
#include <cmath>
#include <fstream>
#include "inet/common/INETDefs.h"

#include "inet/applications/base/ApplicationBase.h"
#include "../Calculation_block/BatteryConsumption.h"
#include "../Calculation_block/SignalStrengthCalculation.h"

#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"

#include "inet/networklayer/common/InterfaceTable.h"
//#include "inet/networklayer/common/NetworkInterface.h"
#include "inet/common/ModuleAccess.h"


using namespace omnetpp;
using namespace inet;

// States for drone FSM
enum DroneState {
    POWER_ON,               // Initial state when the drone is powered on
    WAITING_FOR_TAKEOFF,    // Drone is waiting for a TAKEOFF signal
    WAITING_FOR_COMMANDS,   // Drone is waiting for new commands
    RETURNING_TO_BASE,      // Drone is returning to its base station
    NON_OPERATIONAL,        // Drone is non-operational (e.g., due to collision)
    INITSTAGE_LOCAL_DRONE,	    // Init stage before POWER_ON
};

class DroneControl : public ApplicationBase {
  private:
    int number_of_rotors;					// Number of rotors on the drone
    double motor_efficiency;					// efficiency of motors [W/N]
	
    double mass_of_drone;					// Base mass of the drone [kg]
    double additional_mass;                  // Additional payload mass [kg]
	
    //char communication_type;              // Communication type ('W' for WiFi, 'R' for Radio)
	double frequencyHz;						// Frequency used by drone to communicate
    //float antenna_efficiency;             // Efficiency of the antenna
    double antena_power;					// Power used by antena to send messages
    double sensor_power;					// Power needed gor sensor operating
    double additional_power;				// Power required for additional equipment
    double battery_capacity;				// Total battery capacity in mAh
    double battery_voltage;
    //int network_parameters;				// Placeholder for network-specific parameters
    int drone_size;							// Size category of the drone - one number in meters
    double hoveringCurrent;					// Power needed for hovering

    DroneState state;						// Current state of the drone for FSM

	bool in_air = false;

	double time_step; //frequency of battery check event
    // State handling functions
    void handlePowerOn();
    void handleWaitingForTakeoff(cMessage *msg);
    void handleDroneInAir(cMessage *msg);
    void handleWaitingForCommands(cMessage *msg);
    void handleReturningToBase(cMessage *msg);
    void handleNonOperational(cMessage *msg);

    void handleSetAcceleration(cMessage *msg);
    void handleSetVelocity(cMessage *msg);
    void handleMove(cMessage *msg);
    void handleSetBase(cMessage *msg);

    // Helper functions
	void batteryCheckHelper(int time_step);
	void batteryCheckHelper_forMove();

	//Periodical events for drone
	cMessage *batteryCheckEvent;
	cMessage *nonOperationalEvent;
  protected:
    virtual void initialize(int stage) override; // Initializes the drone module
    virtual void handleMessageWhenUp(cMessage *msg) override; // Handles incoming messages
    virtual void finish();
    // Required lifecycle methods - not in use, but need to be implemented
    virtual void handleStartOperation(inet::LifecycleOperation *operation) override {droneLogFile << "Drone: Start operation" << endl;}
    virtual void handleStopOperation(inet::LifecycleOperation *operation) override {droneLogFile << "Drone: Stop operation" << endl;}
    virtual void handleCrashOperation(inet::LifecycleOperation *operation) override {droneLogFile << "Drone: Crash operation" << endl;}


  public:
	int Drone_ID;				// Unique identifier for the drone
	double battery_remain;      // Remaining battery capacity in mAh
	bool power_on = false;
	double acceleration;
    double x_velocity;
    double y_velocity;
    double z_velocity;
    
    bool Is_Moving = false;
    bool non_operational = false;
    uint8_t Idle_Steps[3];
    double Destination[3]; // Drone destination as [x,y,z]
    double Current_Position[3]; // Drone's position as [x,y,z] coordinates
    uint8_t Next_Move;
    double ChargeStationCoord[3];

    std::ofstream droneLogFile; // logger for drone
    std::string fileName;
};
Define_Module(DroneControl);
#endif /* DRONE_H_ */

