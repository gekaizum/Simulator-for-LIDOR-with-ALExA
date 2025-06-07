//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package Physical_layer.Drone_block;
//import inet.applications.base.ApplicationBase;
import inet.applications.contract.IApp;

module DroneUdpApp like IApp
{
    parameters:
        string interfaceTableModule; // Relative module path of the interface table
        string localAddress = default("");
        int localPort = default(5001);
        //string destAddress;
        //int destPort;
        @display("i=block/app");
    gates:
        input udpAppIn@directIn;
        output trafficOut;
        input socketIn;
        output socketOut;
    submodules:
        udpSink: SmartPacketSink {
		    parameters:      
		}
		udpIo: DroneUdpIo {
		}
	connections allowunconnected:
	    udpAppIn --> udpIo.udpAppIn;
	    udpIo.trafficOut --> udpSink.in;
	    udpIo.socketOut --> socketOut;
	    socketIn --> udpIo.socketIn;
}
