simple BatteryConsumption
{
    parameters:
        double initialBatteryCapacity; // Initial battery capacity in mAh
        double weight;                // Weight of the drone in kg
        double gravity;               // Gravity constant in m/s^2
        double voltage;               // Battery voltage in V
        double efficiency;            // Efficiency (0-1)
        double acceleration;          // Acceleration in m/s^2
        double velocity;              // Velocity in m/s
        double antennaPower;          // Antenna power in W
        double sensorPower;           // Sensor power in W
        double additionalPower;       // Additional power consumption in W
        double timeStep;              // Time step in seconds
        @display("i=misc/highvoltage;is=n");
    gates:
        input in;   // Incoming connection from SimulationControl
        output out; // Outgoing connection to SimulationControl
}
