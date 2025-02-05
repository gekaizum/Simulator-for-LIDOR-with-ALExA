package Physical_layer.simulations;

//import Physical_layer.Map_block.HeightMapLoader;
import Physical_layer.Drone_block.ChargingStation;
import Physical_layer.Drone_block.Drone;

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

        drones[numDrones]: Drone {
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
    connections allowunconnected:
        for i=0..numDrones-2 {
            drones[i].socketOut --> {  delay = uniform(0.1ms, 0.5ms); } --> drones[i+1].socketIn;
        }
        drones[numDrones-1].socketOut --> {  delay = uniform(0.1ms, 0.5ms); } --> drones[0].socketIn;

        for i=0..numDrones-1 {
            simControl.controlSocket[i] <--> drones[i].socketControl;
        }
}

