// Include OMNeT++ library for simulation-specific features
#include <omnetpp.h>
// Include INET's TCP application base for managing TCP communication
#include <inet/applications/tcpapp/TcpAppBase.h>
#include <inet/common/INETDefs.h>
#include <string>
#include <array>

using namespace omnetpp;
using namespace inet;

enum DroneState {
    POWER_ON,               // Initial state when the drone is powered on
    WAITING_FOR_TAKEOFF,    // Drone is waiting for a TAKEOFF signal
    DRONE_IN_AIR,           // Drone is airborne
    WAITING_FOR_COMMANDS,   // Drone is waiting for new commands
    RETURNING_TO_BASE,      // Drone is returning to its base station
    NON_OPERATIONAL         // Drone is non-operational (e.g., due to collision)
};

class Drone : public TcpAppBase {
  private:
    int drone_id;                              // Unique identifier for the drone
    int number_of_rotors;                      // Number of rotors on the drone
    int mass_of_drone;                         // Base mass of the drone
    int additional_mass;                       // Additional payload mass
    char communication_type;                   // Communication type ('W' for WiFi, 'R' for Radio)
    float antenna_efficiency;                  // Efficiency of the antenna
    int additional_power;                      // Power required for additional equipment
    int battery_capacity;                      // Total battery capacity in mAh
    int battery_remain;                        // Remaining battery capacity in mAh
    std::array<float, 3> drone_Position;       // Drone's position as x, y, z coordinates
    int network_parameters;                    // Placeholder for network-specific parameters
    int drone_size;                            // Size category of the drone
    int drone_velocity_parameters;             // Velocity-related parameters

    DroneState state;                          // Current state of the drone
    bool collision_detection_mode;             // Whether collision detection is active

    // State handling functions
    void handlePowerOn();
    void handleWaitingForTakeoff(cMessage *msg);
    void handleDroneInAir();
    void handleWaitingForCommands();
    void handleReturningToBase();
    void handleNonOperational();

    // Helper functions
    bool checkCollision();
    void executeMoveCommand();

  protected:
    virtual void initialize(int stage) override; // Initializes the drone module
    virtual void handleMessageWhenUp(cMessage *msg) override; // Handles incoming messages

  public:
    void broadcast(const std::string& message);              // Broadcasts a message to all drones
    void sendTo(int target_id, const std::string& message);  // Sends a message to a specific drone
};

Define_Module(Drone);

void Drone::initialize(int stage) {
    TcpAppBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        state = POWER_ON;                                    // Set initial state
        collision_detection_mode = true;                     // Enable collision detection
        drone_Position = {0.0, 0.0, 0.0};                    // Set default position to (0, 0, 0)
        drone_id = par("drone_id");                         // Get the drone's ID from parameters
        EV << "Drone " << drone_id << " initialized in POWER_ON state with position (0, 0, 0)." << endl;
    }
}

void Drone::handleMessageWhenUp(cMessage *msg) {
    switch (state) {
        case POWER_ON:
            handlePowerOn();
            break;
        case WAITING_FOR_TAKEOFF:
            handleWaitingForTakeoff(msg);
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
    EV << "Drone is waiting for takeoff sequence." << endl;

    if (std::string(msg->getName()) == "TAKEOFF") {         // Check for TAKEOFF signal
        EV << "Takeoff signal received. Drone is transitioning to DRONE_IN_AIR state." << endl;
        state = DRONE_IN_AIR;                                // Transition to DRONE_IN_AIR state
    } else {
        EV << "Waiting for TAKEOFF signal. Received: " << msg->getName() << endl;
    }
}

void Drone::handleDroneInAir() {
    EV << "Drone is in the air." << endl;
    state = WAITING_FOR_COMMANDS;                            // Transition to WAITING_FOR_COMMANDS state
}

void Drone::handlePowerOn() {
    EV << "Drone is powering on." << endl;
    state = WAITING_FOR_TAKEOFF;                             // Transition to WAITING_FOR_TAKEOFF state
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
    EV << "Drone " << drone_id << " broadcasting message: " << message << endl;

    cMessage *msg = new cMessage(message.c_str());           // Create a new message for broadcasting
    send(msg, "socketOut");                                 // Send the message via the output socket
}

void Drone::sendTo(int target_id, const std::string& message) {
    EV << "Drone " << drone_id << " sending message to Drone " << target_id << ": " << message << endl;

    cMessage *msg = new cMessage(message.c_str());           // Create a new message for direct communication
    send(msg, "socketOut");                                 // Send the message via the output socket
}
