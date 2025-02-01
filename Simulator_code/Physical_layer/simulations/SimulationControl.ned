package Physical_layer.simulations;

simple SimulationControl
{
    parameters:
        int blockSize = default(100);
        int numDrones = default(3);
        int numOfSockets;
        //std::vector<Drone>& drone_data = default(nullptr);
    gates:
        inout controlSocket[numOfSockets];  
}
