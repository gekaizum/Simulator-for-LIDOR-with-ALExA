#include <omnetpp.h>

using namespace omnetpp;

class SimulationControl : public cSimpleModule {
  private:
    cMessage *batteryUpdateTimer; // Timer for sending battery updates
    double remainingBattery;      // Tracks the remaining battery capacity in mAh

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override; // Clean up before module destruction
    void sendBatteryUpdate();
};

Define_Module(SimulationControl);

void SimulationControl::initialize() {
    batteryUpdateTimer = new cMessage("BatteryUpdateTimer");
    remainingBattery = par("initialBatteryCapacity"); // Get initial battery capacity
    scheduleAt(simTime() + 1.0, batteryUpdateTimer);  // Schedule first battery update
}

void SimulationControl::handleMessage(cMessage *msg) {
    if (msg == batteryUpdateTimer) {
        sendBatteryUpdate();
        scheduleAt(simTime() + 1.0, batteryUpdateTimer); // Reschedule for the next update
    } else {
        EV << "Unknown message received in SimulationControl: " << msg->getName() << endl;
    }
}

void SimulationControl::sendBatteryUpdate() {
    // Send a battery calculation request
    cMessage *batteryCalcRequest = new cMessage("BatteryCalcRequest");
    send(batteryCalcRequest, "batteryUsageOut");

    // Simulate receiving a response
    cMessage *batteryCalcResponse = new cMessage("BatteryCalcResponse");
    double usage = uniform(10, 50); // Simulate battery usage in mAh
    remainingBattery -= usage;     // Decrease the battery based on usage

    if (remainingBattery < 0) {
        remainingBattery = 0; // Prevent negative battery level
    }

    batteryCalcResponse->addPar("battery_remain") = remainingBattery;

    // Forward the battery update to the drone
    cMessage *batteryUpdate = new cMessage("BATTERY_UPDATE");
    batteryUpdate->addPar("battery_remain") = batteryCalcResponse->par("battery_remain");
    send(batteryUpdate, "droneOut");

    // Clean up simulated response
    delete batteryCalcResponse;

    EV << "Battery updated. Remaining capacity: " << remainingBattery << " mAh." << endl;
}

void SimulationControl::finish() {
    if (batteryUpdateTimer != nullptr) {
        cancelAndDelete(batteryUpdateTimer);
        batteryUpdateTimer = nullptr;
    }
}
