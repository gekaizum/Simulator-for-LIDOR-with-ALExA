package Physical_layer.simulations;

simple SimulationControl
{
    parameters:
        int blockSize = default(100);
        int numDrones = default(3);
        int numOfSockets;
        int numOfChargeStation;
        int nextStationId;
    gates:
        inout controlSocket[numOfSockets];  
}
