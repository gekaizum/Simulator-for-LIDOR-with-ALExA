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
import inet.environment.common.PhysicalEnvironment;
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
        bool chargeStationPositionsCustomInput = default(false);
        bool droneStartPosCustomInput = default(false);
        bool droneStartPosDefault = default(true);
        bool droneStartPosOnChargeStation = default(false);
        @statistic[receptionPower](source="receptionMinSignalPower(radioMedium.signalArrivalStarted)"; record=last,vector);
        @display("bgb=7000,7000;bgi=photorealistic_heightmap,s;bgg=1000,2,grey95;bgu=m");
    submodules:
        //physicalEnvironment: PhysicalEnvironment {
         //   @display("i=;p=500,500");
        //}

        configurator: Ipv4NetworkConfigurator {
            parameters:
                //@display("p=670,50;is=s");
                @display("i=;p=9999,9999");
        }
        drones[numDrones]: Drone {
            parameters:
                Drone_ID = index + 1;
                useAODV = default(true);
                @display("p=2150,1990;i=powerOffDrone;b=1,1,rect,,black");
        }
        simControl: SimulationControl {
            parameters:
                blockSize = default(1000);
                numDrones = parent.numDrones;
                numOfSockets = parent.numDrones;
                numOfChargeStation = parent.numChargeStation;
                nextStationId = numOfChargeStation + numDrones;
                heightMapFile = default("heightmap.txt");
                userInterrupt = default(false);
                //@display("p=300,50");
                @display("i=;p=9999,9999");
        }
        radioMedium: Ieee80211RadioMedium {
            //@display("p=810,60");
            @display("i=;p=9999,9999");
        }
        ChargingStation[numChargeStation]: ChargingStation {
            parameters:
                ChargingStation_ID = parent.numDrones + index + 1;
                numOfSockets = parent.numDrones;
                @display("i=chargingStation;b=1,1,rect;p=5150,4800");
        }
        basicProtocol: BasicProtocol {
            numOfdronesSockets = parent.numDrones;
            numOfchargeStationSockets = parent.numChargeStation;
            //@display("p=390,50");
            @display("i=;p=9999,9999");
        }
        userLayer: UserLayer {
            //@display("p=210,50");
            @display("i=;p=9999,9999");
        }
    connections allowunconnected:
		//Connection between Simulation control and drones for cMessage passing
        for i=0..numDrones-1 {
            simControl.controlSocket[i] <--> { @display("ls=,0"); } <--> drones[i].socketControl;
        }
        //Connection between Basic protocol layer and Physical layer modules for cMessage passing
        for i=0..numDrones-1 {
            basicProtocol.dronesSocket[i] <--> { @display("ls=,0"); } <--> drones[i].basicProtocolSocket;
        }
        for i=0..numChargeStation-1 {
            basicProtocol.chargingStationSocket[i] <--> { @display("ls=,0"); } <--> ChargingStation[i].basicProtocolSocket;
        }
        basicProtocol.controlSocket <--> { @display("ls=,0"); } <--> simControl.basicProtocolSocket;

}

