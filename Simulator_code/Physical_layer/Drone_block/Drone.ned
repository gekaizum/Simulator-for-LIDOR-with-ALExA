package Physical_layer.Drone_block;

package simulations;

import inet.node.inet.WirelessHost;
import Physical_layer.Drone_block.DroneControl;
import inet.routing.aodv.Aodv;
import inet.networklayer.ipv4.Ipv4RoutingTable;

module Drone extends WirelessHost
{
    parameters:
        int Drone_ID;
        mobility.typename = "StationaryMobility";  // Force stationary behavior
        mobility.initialX = default(200m + Drone_ID * 100m);  // Read X from display
        mobility.initialY = default(200m);               // Read Y from display
        mobility.initialZ = default(0m);
        mobility.initFromDisplayString = false;
        mobility.updateFromDisplayString = true;
        @display("i=device/satellite;bgb=2170,1130");

        bool useAODV = default(true);  // Determines if AODV should be created
        bool useAdhocHost = default(true);  // Determines if AODV should be created
        // Conditional parameter assignment
        wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";  // Higher overhead if using AODV
        wlan[*].agent.typename = ""; // Higher consumption for AODV
        forwarding = true;

        numApps = default(3);
        app[0].typename = "DroneTcpApp";
        app[1].typename = "DroneTcpClient";
        app[2].typename = "DroneUdpApp";

    gates:
        inout socketControl;
        inout basicProtocolSocket;
        input droneControlIn;
        output droneControlOut;
    submodules:
        droneControl: DroneControl {
            Drone_ID = parent.Drone_ID;
            @display("p=1090,60");
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
        if useAODV {
            aodv.socketOut --> at.in++;
            aodv.socketIn <-- at.out++;
        }
}

