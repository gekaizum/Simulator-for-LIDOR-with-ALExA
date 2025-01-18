#include <omnetpp.h>

using namespace omnetpp;

class BatteryConsumption : public cSimpleModule {
  private:
    double remainingCapacity;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    double calculateHoveringCurrent(double weight, double gravity, double voltage, double efficiency);
    double calculateTotalCurrent(double hoveringCurrent, double acceleration, double velocity, double antennaPower, double sensorPower, double additionalPower);
    void updateBatteryCapacity(double &remainingCapacity, double currentConsumption, double timeStep);
};

Define_Module(BatteryConsumption);

void BatteryConsumption::initialize() {
    remainingCapacity = par("initialBatteryCapacity");
}

void BatteryConsumption::handleMessage(cMessage *msg) {
    if (std::string(msg->getName()) == "BatteryCalcRequest") {
        double weight = par("weight");
        double gravity = par("gravity");
        double voltage = par("voltage");
        double efficiency = par("efficiency");
        double acceleration = par("acceleration");
        double velocity = par("velocity");
        double antennaPower = par("antennaPower");
        double sensorPower = par("sensorPower");
        double additionalPower = par("additionalPower");
        double timeStep = par("timeStep");

        double hoveringCurrent = calculateHoveringCurrent(weight, gravity, voltage, efficiency);
        double currentConsumption = calculateTotalCurrent(hoveringCurrent, acceleration, velocity, antennaPower, sensorPower, additionalPower);
        updateBatteryCapacity(remainingCapacity, currentConsumption, timeStep);

        cMessage *response = new cMessage("BatteryCalcResponse");
        response->addPar("battery_remain") = remainingCapacity;
        send(response, "out");
    }
    delete msg;
}

double BatteryConsumption::calculateHoveringCurrent(double weight, double gravity, double voltage, double efficiency) {
    return (weight * gravity) / (voltage * efficiency);
}

double BatteryConsumption::calculateTotalCurrent(double hoveringCurrent, double acceleration, double velocity, double antennaPower, double sensorPower, double additionalPower) {
    return hoveringCurrent + acceleration / 10.0 + antennaPower / 11.1 + sensorPower / 11.1 + additionalPower;
}

void BatteryConsumption::updateBatteryCapacity(double &remainingCapacity, double currentConsumption, double timeStep) {
    remainingCapacity -= (currentConsumption * timeStep * 1000) / 3600.0;
}
