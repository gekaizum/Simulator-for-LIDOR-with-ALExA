#ifndef DRONE_H_
#define DRONE_H_

#include <omnetpp.h>
//#include "../../../inet-master/src/inet/applications/tcpapp/TcpAppBase.h"
//#include "../../../inet-master/src/inet/common/INETDefs.h"
#include <string>
#include <array>
#include "../Calculation_block/BatteryConsumption.cpp"
#include "../Calculation_block/SignalStrengthCalculation.cpp"

using namespace omnetpp;
//using namespace inet;

enum DroneState {
    POWER_ON,               // Initial state when the drone is powered on
    WAITING_FOR_TAKEOFF,    // Drone is waiting for a TAKEOFF signal
    DRONE_IN_AIR,           // Drone is airborne
    WAITING_FOR_COMMANDS,   // Drone is waiting for new commands
    RETURNING_TO_BASE,      // Drone is returning to its base station
    NON_OPERATIONAL,        // Drone is non-operational (e.g., due to collision)
    //INITSTAGE_LOCAL,	    // Init stage before POWER_ON
    //TEMP_SEND_MSG           // Temporary for debug use only
};

//class Drone : public TcpAppBase {
class Drone : public cSimpleModule {
  private:
    int number_of_rotors;                      // Number of rotors on the drone
    int mass_of_drone;                         // Base mass of the drone
    int additional_mass;                       // Additional payload mass
    char communication_type;                   // Communication type ('W' for WiFi, 'R' for Radio)
    float antenna_efficiency;                  // Efficiency of the antenna
    int antena_power;
    int sensor_power;
    int additional_power;                      // Power required for additional equipment
    int battery_capacity;                      // Total battery capacity in mAh
    int battery_remain;                        // Remaining battery capacity in mAh
    int battery_voltage;
    int battery_efficiency;
    int network_parameters;                    // Placeholder for network-specific parameters
    int drone_size;                            // Size category of the drone
    //int drone_velocity_parameters;             // Velocity-related parameters
    double hoveringCurrent;                    // Power needed for hovering

    DroneState state;                          // Current state of the drone
    bool collision_detection_mode;             // Whether collision detection is active

    // State handling functions
    void handlePowerOn(cMessage *msg);
    void handleWaitingForTakeoff(cMessage *msg);
    void handleDroneInAir();
    void handleWaitingForCommands();
    void handleReturningToBase();
    void handleNonOperational();

    // Helper functions
    bool checkCollision();
    void executeMoveCommand();

  protected:
    //virtual void initialize(int stage) override; // Initializes the drone module
    //virtual void handleMessageWhenUp(cMessage *msg) override; // Handles incoming messages
    virtual void initialize(); // Initializes the drone module
    virtual void handleMessage(cMessage *msg); // Handles incoming messages
  public:
    int x_velocity;
    int y_velocity;
    int z_velocity;
    int Drone_ID;                              // Unique identifier for the drone
    bool Is_Moving = false;
    bool non_operational = false;
    uint8_t Idle_Steps[3];
    float Destination[3]; // Drone destination as [x,y,z]
    float Current_Position[3]; // Drone's position as [x,y,z] coordinates
    uint8_t Next_Move;
    void broadcast(const std::string& message);              // Broadcasts a message to all drones
    void sendTo(int target_id, const std::string& message);  // Sends a message to a specific drone
};

#endif /* DRONE_H_ */

