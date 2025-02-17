package Physical_layer.Drone_block;

package simulations;

import inet.node.inet.WirelessHost;
import Physical_layer.Drone_block.DroneControl;
//import inet.mobility.single.StationaryMobility;
module Drone extends WirelessHost
{
    parameters:
        int Drone_ID;
        mobility.typename = "StationaryMobility";  // Force stationary behavior
        mobility.initialX = default(100m);  // Read X from display
        mobility.initialY = default(200m + Drone_ID * 50m);               // Read Y from display
        mobility.initialZ = default(0m);
        mobility.initFromDisplayString = false;
        mobility.updateFromDisplayString = false;
        @display("i=device/satellite");
        
    gates:
        //input socketIn;
        //output socketOut;
        inout socketControl;
        inout basicProtocolSocket;
        //input in[];
        //output out[];
        input droneControlIn;
        output droneControlOut;
    submodules:
        droneControl: DroneControl {
            Drone_ID = parent.Drone_ID;
            //@display("p=920,670");
        }
    connections:
        droneControl.out --> droneControlOut;  // Connect submodule's output to Drone's output
        droneControlIn --> droneControl.in;    // Connect Drone's input to submodule's input
        socketControl <--> droneControl.socketControl;
}

