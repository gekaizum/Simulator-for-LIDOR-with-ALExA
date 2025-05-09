package Physical_layer.Drone_block;

import inet.applications.tcpapp.TcpServerConnection;
import inet.applications.tcpapp.TcpServerSocketIo;
import inet.queueing.contract.IActivePacketSource;
import inet.queueing.contract.IPacketFlow;
import inet.queueing.contract.IPassivePacketSink;
import Physical_layer.Drone_block.CustomTcpServerSocketIo;

module CustomTcpServerConnection //like TcpServerConnection
{
    parameters:
        @application;
        @display("i=block/cogwheel");
    gates:
        input socketIn;
        output socketOut;
    submodules:
        sink: <default("SmartPacketSink")> like IPassivePacketSink if typename != "" {
            parameters:
                @display("p=500,150");
        }
        io: CustomTcpServerSocketIo {
            parameters:
                @display("p=150,300");
        }
    connections allowunconnected:
        io.socketOut --> socketOut;
        socketIn --> io.socketIn;
        io.trafficOut --> sink.in;
}