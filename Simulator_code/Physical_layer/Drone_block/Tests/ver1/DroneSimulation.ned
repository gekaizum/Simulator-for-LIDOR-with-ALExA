network DroneSimulation
{
    submodules:
        simulationControl: SimulationControl {
            @display("p=100,51");
        }
        BatteryConsumption: BatteryConsumption {
            parameters:
                initialBatteryCapacity = 2000; // mAh
                weight = 1.5;
                gravity = 9.8;
                voltage = 11.1;
                efficiency = 0.85;
                acceleration = 1.2;
                velocity = 10.0;
                antennaPower = 2.5;
                sensorPower = 1.5;
                additionalPower = 0.5;
                timeStep = 1.0;
                @display("p=100,176");
        }
        drone1: Drone {
            parameters:
                drone_id = 1;
                initialBatteryCapacity = 2000; // mAh
                @display("p=476,60");
        }
    connections:
        simulationControl.batteryUsageOut --> BatteryConsumption.in;
        BatteryConsumption.out --> simulationControl.in;
        simulationControl.droneOut --> drone1.in;
}
