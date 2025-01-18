#include <omnetpp.h>
#include <string>

using namespace omnetpp;

enum DroneState {
    POWER_ON,
    WAITING_FOR_TAKEOFF,
    DRONE_IN_AIR,
    WAITING_FOR_COMMANDS,
    RETURNING_TO_BASE,
    NON_OPERATIONAL
};

class Drone : public cSimpleModule {
  private:
    int drone_id;
    int battery_remain; // Current battery capacity in mAh
    DroneState state;

    void handlePowerOn();
    void handleWaitingForTakeoff(cMessage *msg);
    void handleDroneInAir();
    void handleWaitingForCommands();
    void handleReturningToBase();
    void handleNonOperational();
    void updateBattery(int newBatteryLevel);

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Drone);

void Drone::initialize() {
    state = POWER_ON;
    drone_id = par("drone_id").intValue();
    battery_remain = par("initialBatteryCapacity").intValue();
    EV << "Drone " << drone_id << " initialized with battery " << battery_remain << " mAh." << endl;
}

void Drone::handleMessage(cMessage *msg) {
    if (std::string(msg->getName()) == "BATTERY_UPDATE") {
        // Handle battery update from SimulationControl
        int newBatteryLevel = msg->par("battery_remain").longValue();
        updateBattery(newBatteryLevel);
    } else if (std::string(msg->getName()) == "REQUEST_BATTERY") {
        // Send current battery level to SimulationControl
        cMessage *batteryStatus = new cMessage("BATTERY_STATUS");
        batteryStatus->addPar("battery_remain") = battery_remain;
        send(batteryStatus, "out"); // Send to SimulationControl
    } else {
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
    }
    delete msg;
}

void Drone::handlePowerOn() {
    EV << "Drone is powering on." << endl;
    state = WAITING_FOR_TAKEOFF;
}

void Drone::handleWaitingForTakeoff(cMessage *msg) {
    EV << "Drone is waiting for takeoff signal." << endl;
    if (std::string(msg->getName()) == "TAKEOFF") {
        EV << "Takeoff signal received. Drone is airborne." << endl;
        state = DRONE_IN_AIR;
    }
}

void Drone::handleDroneInAir() {
    EV << "Drone is in the air." << endl;
    state = WAITING_FOR_COMMANDS;
}

void Drone::handleWaitingForCommands() {
    EV << "Drone is waiting for commands." << endl;
}

void Drone::handleReturningToBase() {
    EV << "Drone is returning to base." << endl;
    state = WAITING_FOR_TAKEOFF;
}

void Drone::handleNonOperational() {
    EV << "Drone is non-operational." << endl;
}

void Drone::updateBattery(int newBatteryLevel) {
    battery_remain = newBatteryLevel;
    EV << "Drone " << drone_id << " updated battery level: " << battery_remain << " mAh." << endl;
}
