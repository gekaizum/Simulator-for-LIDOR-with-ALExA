package Physical_layer.Drone_block;

import inet.applications.tcpapp.TcpServerConnection;
import inet.applications.tcpapp.TcpServerSocketIo;
import inet.queueing.contract.IActivePacketSource;
import inet.queueing.contract.IPacketFlow;
import inet.queueing.contract.IPassivePacketSink;

module CustomTcpServerConnection //like TcpServerConnection
{
    parameters:
        @application;
        @display("i=block/cogwheel");
    gates:
        input socketIn;
        output socketOut;
    submodules:
        //source: <default("ActivePacketSource")> like IActivePacketSource if typename != "" {
        //    parameters:
        //        @display("p=200,150");
        //}
        //tagger: <default("")> like IPacketFlow {
        //    parameters:
        //        @display("p=200,300");
        //}
        //measurementStarter: <default("")> like IPacketFlow {
        //    parameters:
        //        @display("p=200,450");
        //}
        //sink: <default("CustomPacketSink")> like IPassivePacketSink if typename != "" {
        sink: <default("PassivePacketSink")> like IPassivePacketSink if typename != "" {
            parameters:
                @display("p=500,150");
        }
        //measurementRecorder: <default("")> like IPacketFlow {
        //    parameters:
        //        @display("p=500,450");
        //}
        io: TcpServerSocketIo {
            parameters:
                @display("p=150,300");
        }
    connections allowunconnected:
        //source.out --> tagger.in if exists(source);
        //tagger.out --> measurementStarter.in;
        //measurementStarter.out --> io.trafficIn;
        io.socketOut --> socketOut;

        socketIn --> io.socketIn;
        //io.trafficOut --> measurementRecorder.in;
        //measurementRecorder.out --> sink.in if exists(sink);
        io.trafficOut --> sink.in;
}