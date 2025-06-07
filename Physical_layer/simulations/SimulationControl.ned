package Physical_layer.simulations;

simple SimulationControl
{
    parameters:
        int blockSize = default(100);//size of block (part of map) that will be uploaded into RAM
        int numDrones = default(3);//number of drones in simulation
        int numOfSockets; //numOfSockets = numDrones
        int numOfChargeStation = default(1);//number of charging/base stations in simulation
        int nextStationId; //value used for creation of new objects
        string heightMapFile = default("heightmap.txt"); //name of file which stores map metadata
        bool userInterrupt = default(true);
    gates:
        inout controlSocket[numOfSockets];
        inout basicProtocolSocket;
        input additInput@directIn;
}
