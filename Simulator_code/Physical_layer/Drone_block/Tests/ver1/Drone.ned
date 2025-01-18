simple Drone
{
    parameters:
        int drone_id;                 // Unique identifier for the drone
        int initialBatteryCapacity;   // Initial battery capacity in mAh
        @display("i=misc/drone;is=n");
    gates:
        input in;   // Input gate for receiving messages
}
