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
//import inet.applications.tcpapp.TcpBasicClientApp;
import inet.applications.contract.IApp;
//based on TcpBasicClientApp

simple DroneTcpClient like IApp
{
    parameters:
        int Drone_ID = default(0);
        @display("i=block/app");
        string localAddress = default(""); // May be left empty ("")
        int localPort = default(1235); // Port number to listen on
        string connectAddress = default("");  // Server address (may be symbolic)
        int connectPort = default(1234); // Port number to connect to
        bool autoRead = default(true); // Whether to use "autoread" or "explicit-read" mode for TCP connection
        //volatile int readSize @unit(B) = default(-1B);    // Used only with autoRead==false
        //volatile double readDelay @unit(s) = default(-1s);    // Used only with autoRead==false; delay for issuing a READ command after the previous READ was satisfied; -1 means immediately, 0 means zero delay
        //double startTime @unit(s) = default(1s); // Time the first session begins
        //double stopTime @unit(s) = default(-1s);  // Time of finishing sending, negative values mean forever
        //volatile int numRequestsPerSession = default(1);  // Number of requests sent per session
        //volatile int requestLength @unit(B) = default(200B); // Length of a request
        //volatile int replyLength @unit(B) = default(1MiB); // Length of a reply
        //volatile double thinkTime @unit(s); // Time gap between requests
        //volatile double idleInterval @unit(s); // Time gap between sessions
        //volatile double reconnectInterval @unit(s) = default(30s);  // If the connection breaks, wait this long before trying to reconnect
        //int timeToLive = default(-1); // If not -1, set the TTL (IPv4) or Hop Limit (IPv6) field of sent packets to this value
        //int dscp = default(-1); // If not -1, set the DSCP (IPv4/IPv6) field of sent packets to this value
        //int tos = default(-1); // If not -1, set the Type Of Service (IPv4) / Traffic Class (IPv6) field of sent packets to this value
        //@lifecycleSupport;
        //double stopOperationExtraTime @unit(s) = default(-1s);    // Extra time after the lifecycle stop operation finished
        //double stopOperationTimeout @unit(s) = default(2s);    // Timeout value for the lifecycle stop operation
        //@signal[packetSent](type=inet::Packet);
        //@signal[packetReceived](type=inet::Packet);
        //@signal[connect](type=long);  // 1 for open, -1 for close
        //@statistic[packetReceived](title="packets received"; source=packetReceived; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        //@statistic[packetSent](title="packets sent"; source=packetSent; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        //@statistic[endToEndDelay](title="end-to-end delay"; source="dataAge(packetReceived)"; unit=s; record=histogram,weightedHistogram,vector; interpolationmode=none);
        //@statistic[numActiveSessions](title="number of active sessions"; source=warmup(sum(connect)); record=max,timeavg,vector; interpolationmode=sample-hold; autoWarmupFilter=false);
        //@statistic[numSessions](title="total number of sessions"; source="sum(connect+1)/2"; record=last);
    gates:
        input socketIn @labels(TcpCommand/up);
        output socketOut @labels(TcpCommand/down);
        input tcpAppIn@directIn;
}

