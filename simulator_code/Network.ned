//package Physical_layer.simulations;

//import Physical_layer.Map_block.HeightMapLoader;
import Physical_layer.Drone_block.ChargingStation;
import Physical_layer.Drone_block.Drone;
import Physical_layer.simulations.SimulationControl;
import BasicProtocol_layer.BasicProtocol;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211RadioMedium;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;

//channel DataLinkChannel extends ned.DatarateChannel {
//    datarate = 54Mbps;  // Default to WiFi speed
//    delay = 2ms;
//}

network DroneNetwork
{
    parameters:
        int numChargeStation = default(2);
        int numDrones = default(4);
        @display("bgb=860,460");
    submodules:
        ChargingStation[numChargeStation]: ChargingStation {
            parameters:
                ChargingStation_ID = parent.numDrones + index + 1;
                numOfSockets = parent.numDrones;
                @display("i=block/control");
        }
        radioMedium: Ieee80211RadioMedium {
            @display("p=50,50");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=100,100;is=s");
        }        
        drones[numDrones]: Drone {
            parameters:
                Drone_ID = index + 1;
        }
        simControl: SimulationControl {
            parameters:
                blockSize = default(100);
                numDrones = parent.numDrones;
                numOfSockets = parent.numDrones;
                numOfChargeStation = parent.numChargeStation;
                nextStationId = numOfChargeStation + numDrones;
                @display("p=260,50");
        }
        basicProtocol: BasicProtocol {
            numOfdronesSockets = parent.numDrones;
            numOfchargeStationSockets = parent.numChargeStation;
            @display("p=130,50");
        }
    connections allowunconnected:
        //connection for cMessages between drone's submodules
        //for i=0..numDrones-2 {
        //    drones[i].socketOut --> {  delay = uniform(0.1ms, 0.5ms); } --> drones[i+1].socketIn;
        //}
        //drones[numDrones-1].socketOut --> {  delay = uniform(0.1ms, 0.5ms); } --> drones[0].socketIn;

		//Connection between Simulation control and drones for cMessage passing
        for i=0..numDrones-1 {
            simControl.controlSocket[i] <--> drones[i].socketControl;
        }
        //Connection between Basic protocol layer and Physical layer modules for cMessage passing
        for i=0..numDrones-1 {
            basicProtocol.dronesSocket[i] <--> drones[i].basicProtocolSocket;
        }
        for i=0..numChargeStation-1 {
            basicProtocol.chargingStationSocket[i] <--> ChargingStation[i].basicProtocolSocket;
        }
        basicProtocol.controlSocket <--> simControl.basicProtocolSocket;
}
