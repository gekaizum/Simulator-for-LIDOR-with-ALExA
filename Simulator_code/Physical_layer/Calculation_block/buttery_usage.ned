simple BatteryConsumption {
    parameters:
        double initialBatteryCapacity; // [mAh]
        double weight;                // [kg]
        double gravity = 9.81;        // [m/s^2]
        double voltage = 11.1;        // [V]
        double efficiency = 0.8;     // [0-1]
        double acceleration;          // [m/s^2]
        double velocity;              // [m/s]
        double antennaPower;          // [W]
        double sensorPower;           // [W]
        double additionalPower = 0.0; // [W]
        double timeStep = 1.0;        // [s]
}
