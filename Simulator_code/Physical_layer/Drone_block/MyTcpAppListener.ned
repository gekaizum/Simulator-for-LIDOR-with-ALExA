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
import inet.applications.contract.IApp;
//import inet.applications.tcpapp.TcpServerListener;
//import inet.transportlayer.contract.ITcp;
//import Physical_layer.Drone_block.MyTcpAppListener;

simple MyTcpAppListener like IApp
{
    parameters:
        string localAddress = default(""); // May be left empty ("")
        int localPort = default(1234); // Port number to listen on
        string serverConnectionModuleType = default("Physical_layer.Drone_block.CustomTcpServerConnection"); // Module type of "connection" module to launch on incoming connections
        //string serverConnectionModuleType = default("inet.applications.tcpapp.TcpRequestResponseApp");
        //string serverConnectionModuleType = default("inet.applications.tcpapp.TcpGenericServerApp");
        //bool autoRead = default(true); // Defines the autoRead setting for connection handling modules
        @display("i=block/server");
    gates:
        input socketIn;
        output socketOut;
        input tcpAppIn@directIn;
    //connections allowunconnected:
}