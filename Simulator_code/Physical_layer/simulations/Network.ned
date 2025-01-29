package Physical_layer.simulations;

import Physical_layer.Map_block.HeightMapLoader;
import Physical_layer.Map_block.ChargingStationManager;
import Physical_layer.Drone_block.Drone;

network DroneNetwork
{
    parameters:
        int numDrones = default(3);
    submodules:
        mapLoader: HeightMapLoader {
            @display("p=228,314;i=device/palm;is=s");
        }
        chargingStationManager: ChargingStationManager {
            @display("p=582,266;i=block/control");
        }

        drones[numDrones]: Drone {
            parameters:
                Drone_ID = index + 1;
                @display("i=device/satellite");
        }
		simControl: SimulationControl {
			parameters:
			    //string heightMapFile = "heightmap.txt"; //Map will be stored here
    			//vector<Drone>& drone_data = drones[numDrones]; //List of drones needed for "drone_positions_c"
		}
    connections allowunconnected:
        for i=0..numDrones-2 {
            drones[i].socketOut --> {  delay = uniform(0.1ms, 0.5ms); } --> drones[i+1].socketIn;
        }
        drones[numDrones-1].socketOut --> {  delay = uniform(0.1ms, 0.5ms); } --> drones[0].socketIn;
        
        simControl.socketOut --> drones[0].socketControl;
}

