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

package BasicProtocol_layer;

simple BasicProtocol
{
    parameters:
        int numOfdronesSockets;
        int numOfchargeStationSockets;
        int localPort = default(1235); // Port number to listen on (TCP)
        int connectPort = default(1234); // Port number to connect to (TCP)
        int udpPort = default(5001); // Port number for UDP connections/bind
    gates:
        inout controlSocket;
        inout dronesSocket[numOfdronesSockets];
        inout chargingStationSocket[numOfchargeStationSockets];   
}