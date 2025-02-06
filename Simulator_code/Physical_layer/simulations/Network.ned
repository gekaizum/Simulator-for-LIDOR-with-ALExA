package Physical_layer.simulations;

//import Physical_layer.Map_block.HeightMapLoader;
import Physical_layer.Drone_block.ChargingStation;
import Physical_layer.Drone_block.Drone;

//****new - For wireless Network****//
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.contract.INetworkNode;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;
//****new - For wireless Network****//

network DroneNetwork
{
    parameters:
        int numChargeStation = default(2);
        int numDrones = default(3);
        @display("bgb=860,460");
    submodules:
        //mapLoader: HeightMapLoader {
        //    @display("p=55,42;i=device/palm;is=s");
        //}
        ChargingStation[numChargeStation]: ChargingStation {
            parameters:
                ChargingStation_ID = parent.numDrones + index + 1;
                numOfSockets = parent.numDrones;
                @display("i=block/control");
        }

        drone[numDrones]: Drone {
            parameters:
                Drone_ID = index + 1;
                @display("i=device/satellite");
        }
        simControl: SimulationControl {
            parameters:
                blockSize = default(100);
                numDrones = default(3);
                numOfSockets = default(3);
                //Drone drones_list = drones;
			    //string heightMapFile = "heightmap.txt"; //Map will be stored here
    			//vector<Drone>& drone_data = drones[numDrones]; //List of drones needed for "drone_positions_c"
                @display("p=340,220");
        }
        
//****new - For wireless Network****//      
        visualizer: <default(firstAvailableOrEmpty("IntegratedOsgVisualizer"))> like IIntegratedVisualizer if typename != "" {
            parameters:
                @display("p=100,300;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=100,100;is=s");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            parameters:
                @display("p=100,200;is=s");
        }
                                     
//****new - For wireless Network****//        
        
     
    connections allowunconnected:
        for i=0..numDrones-2 {
            drone[i].socketOut --> {  delay = uniform(0.1ms, 0.5ms); } --> drone[i+1].socketIn;
        }
        drone[numDrones-1].socketOut --> {  delay = uniform(0.1ms, 0.5ms); } --> drone[0].socketIn;

        for i=0..numDrones-1 {
            simControl.controlSocket[i] <--> drone[i].socketControl;
        }
}

