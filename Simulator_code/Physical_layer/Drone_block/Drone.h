#ifndef DRONE_H_
#define DRONE_H_

#include <omnetpp.h>
#include <string>
#include <array>
//#include "../Calculation_block/BatteryConsumption.cpp"
#include "../Calculation_block/BatteryConsumption.h"
//#include "../Calculation_block/SignalStrengthCalculation.cpp"
#include "../Calculation_block/SignalStrengthCalculation.h"
//#include "drone_positions_c.cpp"

using namespace omnetpp;
//using namespace inet;

enum DroneState {
    POWER_ON,               // Initial state when the drone is powered on
    WAITING_FOR_TAKEOFF,    // Drone is waiting for a TAKEOFF signal
    //DRONE_IN_AIR,           // Drone is airborne
    WAITING_FOR_COMMANDS,   // Drone is waiting for new commands
    RETURNING_TO_BASE,      // Drone is returning to its base station
    NON_OPERATIONAL,        // Drone is non-operational (e.g., due to collision)
    INITSTAGE_LOCAL,	    // Init stage before POWER_ON
    //TEMP_SEND_MSG         // Temporary for debug use only
};

//class Drone : public TcpAppBase {
class Drone : public cSimpleModule {
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
    double battery_remain;					// Remaining battery capacity in mAh
    double battery_voltage;
    //int network_parameters;				// Placeholder for network-specific parameters
    int drone_size;							// Size category of the drone - one number in meters
    double hoveringCurrent;					// Power needed for hovering

    DroneState state = INITSTAGE_LOCAL;						// Current state of the drone for FSM
	bool power_on = false;
	bool in_air = false;
    //bool collision_detection_mode;		// Whether collision detection is active

    // State handling functions
    void handlePowerOn(cMessage *msg);
    void handleWaitingForTakeoff(cMessage *msg);
    void handleDroneInAir(cMessage *msg);
    void handleWaitingForCommands(cMessage *msg);
    void handleReturningToBase(cMessage *msg);
    void handleNonOperational(cMessage *msg);

    // Helper functions
    //bool checkCollision();
    //void executeMoveCommand();
	void batteryCheckHelper(int time_step);
	//Periodical events for drone
	cMessage *batteryCheckEvent;
  protected:
    virtual void initialize(); // Initializes the drone module
    virtual void handleMessage(cMessage *msg); // Handles incoming messages
	virtual void finish();
  public:
	int Drone_ID;				// Unique identifier for the drone
	double acceleration;
    double x_velocity;
    double y_velocity;
    double z_velocity;
    
    bool Is_Moving = false;
    bool non_operational = false;
    uint8_t Idle_Steps[3];
    float Destination[3]; // Drone destination as [x,y,z]
    float Current_Position[3]; // Drone's position as [x,y,z] coordinates
    uint8_t Next_Move;
    void broadcast(const std::string& message);              // Broadcasts a message to all drones
    void sendTo(int target_id, const std::string& message);  // Sends a message to a specific drone
};
Define_Module(Drone);
#endif /* DRONE_H_ */

