//package Physical_layer.simulations;

//import Physical_layer.Map_block.HeightMapLoader;
import Physical_layer.Drone_block.ChargingStation;
import Physical_layer.Drone_block.Drone;
import Physical_layer.simulations.SimulationControl;
import BasicProtocol_layer.BasicProtocol;
import User_layer.UserLayer;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211RadioMedium;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;

import inet.visualizer.canvas.integrated.IntegratedCanvasVisualizer;
//channel DataLinkChannel extends ned.DatarateChannel {
//    datarate = 54Mbps;  // Default to WiFi speed
//    delay = 2ms;
//}

network DroneNetwork
{
    parameters:
        int numChargeStation = default(2);
        int numDrones = default(3);
        //@display("bgb=1090,540;bgi=samples/Simulator_code_mark_4/photorealistic_heightmap.png,s;bgg=1000,2,grey95;bgu=m");
        @display("bgb=1090,540;bgi=photorealistic_heightmap,s;bgg=1000,2,grey95;bgu=m");
    submodules:
        radioMedium: Ieee80211RadioMedium {
            @display("p=810,20");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=720,20;is=s");
        }
        ChargingStation[numChargeStation]: ChargingStation {
            parameters:
                ChargingStation_ID = parent.numDrones + index + 1;
                numOfSockets = parent.numDrones;
                @display("i=block/control");
        }
        drones[numDrones]: Drone {
            parameters:
                Drone_ID = index + 1;
                useAODV = default(true);
                @display("p=110,250;i=contrastDrone;b=1,1,rect");
        }
        simControl: SimulationControl {
            parameters:
                blockSize = default(100);
                numDrones = parent.numDrones;
                numOfSockets = parent.numDrones;
                numOfChargeStation = parent.numChargeStation;
                nextStationId = numOfChargeStation + numDrones;
                heightMapFile = default("heightmap.txt");
                @display("p=810,110");
        }
        basicProtocol: BasicProtocol {
            numOfdronesSockets = parent.numDrones;
            numOfchargeStationSockets = parent.numChargeStation;
            @display("p=810,310");
        }
        userLayer: UserLayer {
            @display("p=210,50");
        }
    connections allowunconnected:
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

