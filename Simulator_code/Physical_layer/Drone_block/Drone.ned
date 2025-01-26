package Physical_layer.Drone_block;

package simulations;

simple Drone
{
    parameters:
        int Drone_ID;
        double batteryCapacity @unit(mAh) = default(5000mAh);
        double weight @unit(kg) = default(1.5kg);
        string communicationType = default("WiFi");
        //@display("i=misc/drone;is=n");
    gates:
        input socketIn;
        output socketOut;
}

