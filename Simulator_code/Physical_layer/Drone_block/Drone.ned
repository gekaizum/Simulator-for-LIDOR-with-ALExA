package Physical_layer.Drone_block;

package simulations;

import inet.node.inet.WirelessHost;
import Physical_layer.Drone_block.DroneControl;
import inet.routing.aodv.Aodv;
//import inet.applications.tcpapp.*;

module Drone extends WirelessHost
{
    parameters:
        int Drone_ID;
        mobility.typename = "StationaryMobility";  // Force stationary behavior
        mobility.initialX = default(200m + Drone_ID * 100m);  // Read X from display
        mobility.initialY = default(200m);               // Read Y from display
        mobility.initialZ = default(0m);
        mobility.initFromDisplayString = false;
        mobility.updateFromDisplayString = false;
        @display("i=device/satellite");

        bool useAODV = default(true);  // Determines if AODV should be created
        bool useAdhocHost = default(true);  // Determines if AODV should be created
        // Conditional parameter assignment
        wlan[*].mgmt.typename = default(useAODV ? "Ieee80211MgmtAdhoc" : "Ieee80211RadioMedium");  // Higher overhead if using AODV
        wlan[*].agent.typename = default(useAdhocHost ? "" : ""); // Higher consumption for AODV
        forwarding = default(useAdhocHost ? true : true);

    gates:
        input socketIn;
        output socketOut;
        inout socketControl;
        inout basicProtocolSocket;
        //output tcpAppIn;  // NEW: Gate to receive connection requests
        //input in[];
        //output out[];
        input droneControlIn;
        output droneControlOut;
    submodules:
        droneControl: DroneControl {
            Drone_ID = parent.Drone_ID;
            //@display("p=920,670");
            @display("p=1080,80");
        }
        aodv: Aodv if useAODV {
            parameters:
                @display("p=890,220");
        }
    connections allowunconnected:
        droneControl.out --> droneControlOut;  // Connect submodule's output to Drone's output
        droneControlIn --> droneControl.in;    // Connect Drone's input to submodule's input
        socketControl <--> droneControl.socketControl;
        droneControl.socketOut --> at.in++;
        droneControl.socketIn <-- at.out++;
        basicProtocolSocket <--> droneControl.basicProtocolSocket;
        //tcpAppIn --> this.app[0].controlIn;
        //droneControl.tcpApp --> app[0].tcpAppIn;
        if useAODV {
            aodv.socketOut --> at.in++;
            aodv.socketIn <-- at.out++;
        }
        //DroneTcpApp.tcpAppIn <-- at.out++;
        //app[0].tcpAppIn <-- at.out++;
}

