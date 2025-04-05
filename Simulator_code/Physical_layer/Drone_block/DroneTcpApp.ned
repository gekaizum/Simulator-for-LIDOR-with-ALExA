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
//package inet.applications.tcpapp;

import inet.applications.contract.IApp;
import inet.common.MessageDispatcher;
//import inet.applications.base.ApplicationBase;
//import inet.transportlayer.contract.tcp.TcpSocket;
//import inet.applications.tcpapp.TcpServerApp;
import Physical_layer.Drone_block.MyTcpAppListener;

module DroneTcpApp like IApp
{
    parameters:
        string interfaceTableModule; // Relative module path of the interface table
        //*.interfaceTableModule = default(this.interfaceTableModule);
        @display("i=block/app");
    gates:
        input socketIn;
        output socketOut;
    submodules:
        listener: MyTcpAppListener {
            parameters:
                //        input listener.tcpAppIn@directIn;  // External control input gate @directIn
                @display("p=100,100");
        }
        connection[0]: <default(listener.serverConnectionModuleType)> like IApp {
            parameters:
                @display("p=200,100,row,50");
        }
        ta: MessageDispatcher {
            parameters:
                @display("p=100,200");
        }
    connections:
        //listener.socketIn --> { serviceMapping = [{ protocol = "tcp", gate = "socketOut" }]; };
        ta.out++ --> listener.socketIn;
        listener.socketOut --> ta.in++;
        ta.out++ --> { @display("m=s"); } --> socketOut;
        socketIn --> { @display("m=s"); } --> ta.in++;
}

