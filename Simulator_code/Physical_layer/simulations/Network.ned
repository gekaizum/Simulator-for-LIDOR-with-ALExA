package Physical_layer.simulations;

import Physical_layer.Map_block.HeightMapLoader;
//import Physical_layer.Calculation_block.SignalStrengthCalculation;
//import Physical_layer.Calculation_block.BatteryConsumption;
import Physical_layer.Map_block.ChargingStationManager;
import Physical_layer.Drone_block.Drone;

network DroneNetwork
{
    parameters:
        int numDrones = default(3);
		//Drone dronesList[numDrones];
    submodules:
        mapLoader: HeightMapLoader {
            @display("p=228,314;i=device/palm;is=s");
        }
        chargingStationManager: ChargingStationManager {
            @display("p=582,266;i=block/control");
        }
        //signalCalculator: SignalStrengthCalculation {
        //    @display("p=629,100;i=block/wrx");
        //}
        //batteryManager: BatteryConsumption {
        //    @display("p=382,314;i=block/activeq");
        //}

        drones[numDrones]: Drone {
            parameters:
                Drone_ID = index + 1;
                @display("i=device/satellite");
        }
        //drones1: Drone {
        //    parameters:
        //        Drone_ID = 1;
        //        @display("p=228,100;i=device/satellite");  // Set fixed position (or update in .ini)
        //}
        //drones2: Drone {
        //    parameters:
        //        Drone_ID = 2;
        //        @display("p=360,100;i=device/satellite");  // Set fixed position (or update in .ini)
        //}

    //connections allowunconnected:  // Corrected by replacing ';' with ':'
        //drones1.socketOut --> {  delay = uniform(0.1ms, 0.5ms); } --> drones2.socketIn;
        //drones2.socketOut --> {  delay = uniform(0.1ms, 0.5ms); } --> drones1.socketIn;
    //connections allowunconnected:
    //    for i = 0..int(ceil(numDrones/2))-1, for j = int(ceil(numDrones/2))..numDrones-1 {  // Ensure each pair is connected only once
    //            drones[i].socketOut --> { delay = uniform(0.1ms, 0.5ms); } --> drones[j].socketIn;
    //            drones[j].socketOut --> { delay = uniform(0.1ms, 0.5ms); } --> drones[i].socketIn;
    //    }
    connections allowunconnected:
        for i=0..numDrones-2 {
            drones[i].socketOut --> {  delay = uniform(0.1ms, 0.5ms); } --> drones[i+1].socketIn;
        }
        drones[numDrones-1].socketOut --> {  delay = uniform(0.1ms, 0.5ms); } --> drones[0].socketIn;
}

