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

// Manages an established TCP connection. It handles data transmission to and
// from a module connected via the `trafficIn` and `trafficOut` gates. When data is
// received on the `trafficIn` gate, it is forwarded on the TCP connection via the
// `socketOut` gate, and vice versa. This module simplifies the task of data
// exchange over an established TCP connection.
//
// Note that reading from the socket is only rate limited if the connection was
// originally created with the autoRead=false setting. Then, you can use the
// `readSize` and `readDelay` parameters to set a rate limit. This will allow TCP
// flow control to come into effect.
//
// @see ~TcpServerConnection, ~TcpServerListener, ~TcpClientSocketIo
//
simple CustomTcpServerSocketIo
{
    parameters:
        @display("i=block/socket");
        volatile int readSize @unit(B) = default(-1B);    // Used only with autoRead==false
        volatile double readDelay @unit(s) = default(-1s);    // Used only with autoRead==false; delay for issuing a READ command after previous READ was satisfied; -1 means immediately, 0 means zero delay
    gates:
        input trafficIn;
        output trafficOut;
        input socketIn;
        output socketOut;
}