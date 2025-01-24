#include <omnetpp.h>

using namespace omnetpp;

class BatteryConsumption : public cSimpleModule {
  private:
    double remainingCapacity; // [mAh]

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    // Function to calculate hovering current
    double calculateHoveringCurrent(double weight, double gravity, double voltage, double efficiency);

    // Function to calculate total current consumption
    double calculateTotalCurrent(double hoveringCurrent, double acceleration, double velocity, double antennaPower, double sensorPower, double additionalPower);

    // Function to update remaining battery capacity
    void updateBatteryCapacity(double &remainingCapacity, double currentConsumption, double timeStep);
};

Define_Module(BatteryConsumption);

// Initialize module parameters
void BatteryConsumption::initialize() {
    // Get initial battery capacity from NED parameters
    remainingCapacity = par("initialBatteryCapacity"); // [mAh]
}

// Handle incoming messages (e.g., simulation steps)
void BatteryConsumption::handleMessage(cMessage *msg) {
    // Get parameters from message or module parameters
    double weight = par("weight");                  // [kg]
    double gravity = par("gravity");                // [m/s^2]
    double voltage = par("voltage");                // [V]
    double efficiency = par("efficiency");          // [0-1]
    double acceleration = par("acceleration");      // [m/s^2]
    double velocity = par("velocity");              // [m/s]
    double antennaPower = par("antennaPower");      // [W]
    double sensorPower = par("sensorPower");        // [W]
    double additionalPower = par("additionalPower");// [W]
    double timeStep = par("timeStep");              // [s]

    // Step 1: Calculate hovering current (computed once if static)
    double hoveringCurrent = calculateHoveringCurrent(weight, gravity, voltage, efficiency);

    // Step 2: Calculate total current consumption
    double currentConsumption = calculateTotalCurrent(hoveringCurrent, acceleration, velocity, antennaPower, sensorPower, additionalPower);

    // Step 3: Update remaining battery capacity
    updateBatteryCapacity(remainingCapacity, currentConsumption, timeStep);

    // Log remaining capacity
    EV << "Remaining battery capacity: " << remainingCapacity << " mAh" << endl;

    // Clean up message
    delete msg;
}

// Function to calculate hovering current
double BatteryConsumption::calculateHoveringCurrent(double weight, double gravity, double voltage, double efficiency) {
    return (weight * gravity) / (voltage * efficiency);
}

// Function to calculate total current consumption
double BatteryConsumption::calculateTotalCurrent(double hoveringCurrent, double acceleration, double velocity, double antennaPower, double sensorPower, double additionalPower) {
    double accelerationCurrent = acceleration / 10.0; // Example scale factor
    double antennaCurrent = antennaPower / 11.1; // Based on battery voltage [V]
    double sensorCurrent = sensorPower / 11.1;  // Based on battery voltage [V]
    return hoveringCurrent + accelerationCurrent + antennaCurrent + sensorCurrent + additionalPower;
}

// Function to update remaining battery capacity
void BatteryConsumption::updateBatteryCapacity(double &remainingCapacity, double currentConsumption, double timeStep) {
    remainingCapacity -= (currentConsumption * timeStep * 1000) / 3600.0;
}
