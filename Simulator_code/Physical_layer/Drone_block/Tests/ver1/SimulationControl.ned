simple SimulationControl
{
    parameters:
        double initialBatteryCapacity; // Initial battery capacity in mAh
        @display("i=block/control");
    gates:
        output batteryUsageOut; // Outgoing connection to BatteryConsumption
        input in;               // Incoming response from BatteryConsumption
        output droneOut;        // Outgoing connection to Drone
}
