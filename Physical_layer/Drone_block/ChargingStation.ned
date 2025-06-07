package Physical_layer.Drone_block;

package simulations;

simple ChargingStation
{
    parameters:
        int ChargingStation_ID;
        double frequencyHz @unit(Hz) = default(24000Hz);
        //string communicationType = default("WiFi");
        int numOfSockets; //number of created drones
    gates:
        input socketIn[numOfSockets]; // used by drones
        output socketOut[numOfSockets]; // used by drones
        inout socketControl; //used by simulation control
        inout basicProtocolSocket;
}