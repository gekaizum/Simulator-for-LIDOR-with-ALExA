package Physical_layer.simulations;

//import Physical_layer.Map_block.HeightMapLoader;
import Physical_layer.Drone_block.ChargingStation;
import Physical_layer.Drone_block.Drone;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211RadioMedium;

//channel DataLinkChannel extends ned.DatarateChannel {
//    datarate = 54Mbps;  // Default to WiFi speed
//    delay = 2ms;
//}

network DroneNetwork
{
    parameters:
        int numChargeStation = default(2);
        int numDrones = default(3);
        @display("bgb=860,460");
        //string place="p=100,";
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
        radioMedium: Ieee80211RadioMedium {
            @display("p=50,50");
        }
        drones[numDrones]: Drone {
            parameters:
                Drone_ID = index + 1;
                //displayPositionY = 200 + Drone_ID * 50;
                //displayString = "a=0,p=200," + string(displayPositionY);
                //@display(displayString);
                
                //@display("i=device/satellite");
        }
        simControl: SimulationControl {
            parameters:
                blockSize = default(100);
                numDrones = parent.numDrones;
                numOfSockets = parent.numDrones;
                numOfChargeStation = parent.numChargeStation;
                nextStationId = numOfChargeStation + numDrones;
                //Drone drones_list = drones;
			    //string heightMapFile = "heightmap.txt"; //Map will be stored here
    			//vector<Drone>& drone_data = drones[numDrones]; //List of drones needed for "drone_positions_c"
                @display("p=250,280");
        }
    connections allowunconnected:
        //connection for cMessages between drone's submodules
        //for i=0..numDrones-2 {
        //    drones[i].socketOut --> {  delay = uniform(0.1ms, 0.5ms); } --> drones[i+1].socketIn;
        //}
        //drones[numDrones-1].socketOut --> {  delay = uniform(0.1ms, 0.5ms); } --> drones[0].socketIn;

        for i=0..numDrones-1 {
            simControl.controlSocket[i] <--> drones[i].socketControl;
        }
}

