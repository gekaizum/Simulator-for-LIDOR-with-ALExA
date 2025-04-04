/*
 * DroneTcpClient.cpp
 *
 *  Created on: Mar 12, 2025
 *      Author: gekaizum
 */
#include "DroneTcpClient.h"
#include "inet/common/ModuleAccess.h"
#include "inet/networklayer/common/L3AddressResolver.h"

simsignal_t DroneTcpClient::connectSignal = registerSignal("connect");

void DroneTcpClient::initialize(int stage) {
    ApplicationBase::initialize(stage);
    if (stage == INITSTAGE_APPLICATION_LAYER) {
        localAddress = par("localAddress");
        localPort = par("localPort");
        //autoRead = par("autoRead");
        socket.setOutputGate(gate("socketOut"));
        socket.setCallback(this);
        int localPort = par("localPort");
        L3Address myIP = L3AddressResolver().resolve(getParentModule()->getFullPath().c_str());
        socket.bind(myIP, localPort);
        sendMessageEvent = new cMessage("myTrigger");
        scheduleAt(simTime() + 5.0, sendMessageEvent);
        Drone_ID = getParentModule()->par("Drone_ID");
        fileName = "Drone_logs/Drone_" + std::to_string(Drone_ID) + "_TcpClientLogFile.log";
        droneLogFile.open(fileName, std::ios::out);  // Create/open log file
        droneLogFile << "Bindport is: " << localPort << endl;
        droneLogFile << "BindAddress is: " << myIP << endl;

    }
}

void DroneTcpClient::handleMessageWhenUp(cMessage *msg) {

    if (msg == sendMessageEvent) {
        if(Drone_ID == 2 ){
            L3Address destAddr = L3AddressResolver().resolve("drones[0]");
            int connectPort = 1234;
            droneLogFile << "TargetPort is: " << 1234 << " .Time: " << simTime() << endl;
            droneLogFile << "TargetAddress is: " << destAddr << " .Time: " << simTime() << endl;
            if (destAddr.isUnspecified()) {
                EV_ERROR << "Connecting to " << destAddr << " port=" << connectPort << ": cannot resolve destination address" << endl;
            }
            else {
                droneLogFile << "Connecting to " << destAddr << " port=" << connectPort << endl;
                socket.connect(destAddr, connectPort);
                //numSessions++;
                emit(connectSignal, 1L);
            }
            droneLogFile << "Socket connecting processes started at "<< simTime() << endl;
            /*int timeToLive = par("timeToLive");
            if (timeToLive != -1)
                socket.setTimeToLive(timeToLive);*/

            //int dscp = par("dscp");
            //int dscp = 0;
            //if (dscp != -1)
                //socket.setDscp(dscp);

            //int tos = par("tos");
            //int tos = 0;
            //if (tos != -1)
            //    socket.setTos(tos);

            // connect
            /*const char *connectAddress = "10.0.0.1";
            int connectPort = 1234;

            L3Address destination;
            L3AddressResolver().tryResolve(connectAddress, destination);
            if (destination.isUnspecified()) {
                EV_ERROR << "Connecting to " << connectAddress << " port=" << connectPort << ": cannot resolve destination address" << endl;
            }
            else {
                droneLogFile << "Connecting to " << connectAddress << "(" << destination << ") port=" << connectPort << endl;

                //socket.setAutoRead(par("autoRead"));
                socket.connect(destination, connectPort);

                //numSessions++;
                emit(connectSignal, 1L);
            }
            //socket.connect(destAddr, 1234);
            droneLogFile << "Socket connected" << endl;
            trigger2 = new cMessage("myTrigger");
            scheduleAt(simTime() + 2.0, trigger2);*/

        }

    }
    /*else if (msg == trigger2) {
        droneLogFile << "Sending packet" << endl;
        sendRequest();
    }
    else if (msg -> isSelfMessage()){
        L3Address myIP = L3AddressResolver().resolve(getParentModule()->getFullPath().c_str());
        droneLogFile << "My IP is:" << myIP << endl;
        droneLogFile << "Tcp client port:" << localPort << endl;
        socket.bind(myIP, localPort); // Use ephemeral port
        delete(msg);
        sendMessageEvent = new cMessage("sendMessage");
        scheduleAt(simTime() + 10.0, sendMessageEvent);
        //Drone_ID = getParentModule()->getParentModule()->par("Drone_ID");
    }*/
    else {
        droneLogFile << "Received tcp msg: " << msg << endl;
        socket.processMessage(msg);
    }
}

void DroneTcpClient::socketEstablished(TcpSocket *socket) {
    EV << "TCP connection established, ready to send data." << endl;
    sendRequest();
    droneLogFile << "Sending packet at "<< simTime() << endl;
}

void DroneTcpClient::socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent) {
    EV << "Received response: " << msg->str() << endl;
    delete msg;
}

void DroneTcpClient::socketClosed(TcpSocket *socket) {
    EV << "Connection closed by server." << endl;
}

void DroneTcpClient::sendRequest() {
    droneLogFile << "Sending data to server." << endl;
/*
    auto packet = new Packet("DroneData");
    auto payload = makeShared<ByteCountChunk>();
    payload->setLength(B(100));  // Set non-empty payload
*/

    // Create payload with "start"
    const char *data = "moveTo 100 200 50";
    auto payload = makeShared<BytesChunk>(std::vector<uint8_t>(data, data + strlen(data)));

    // Create and send packet
    auto packet = new Packet("cmdPacket");

    packet->insertAtBack(payload);  // Attach payload to the packet
    socket.send(packet);
    //scheduleAt(simTime() + uniform(1, 5), sendMessageEvent);
}

DroneTcpClient::~DroneTcpClient() {
    cancelAndDelete(sendMessageEvent);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DroneTcpClient::connect()
{
    // we need a new connId if this is not the first connection
    /*socket.renewSocket();

    const char *localAddress = par("localAddress");
    int localPort = par("localPort");
    socket.bind(*localAddress ? L3AddressResolver().resolve(localAddress) : L3Address(), localPort);

    int timeToLive = par("timeToLive");
    if (timeToLive != -1)
        socket.setTimeToLive(timeToLive);

    int dscp = par("dscp");
    if (dscp != -1)
        socket.setDscp(dscp);

    int tos = par("tos");
    if (tos != -1)
        socket.setTos(tos);

    // connect
    const char *connectAddress = par("connectAddress");
    int connectPort = par("connectPort");

    L3Address destination;
    L3AddressResolver().tryResolve(connectAddress, destination);
    if (destination.isUnspecified()) {
        EV_ERROR << "Connecting to " << connectAddress << " port=" << connectPort << ": cannot resolve destination address\n";
    }
    else {
        EV_INFO << "Connecting to " << connectAddress << "(" << destination << ") port=" << connectPort << endl;

        socket.setAutoRead(par("autoRead"));
        socket.connect(destination, connectPort);

        //numSessions++;
        //emit(connectSignal, 1L);
    }*/
}

void DroneTcpClient::close()
{
    EV_INFO << "issuing CLOSE command\n";
    socket.close();
    //emit(connectSignal, -1L);
}

void DroneTcpClient::sendPacket(Packet *msg)
{
    int numBytes = msg->getByteLength();
    emit(packetSentSignal, msg);
    socket.send(msg);

    //packetsSent++;
    //bytesSent += numBytes;
}

void DroneTcpClient::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();
    getDisplayString().setTagArg("t", 0, TcpSocket::stateName(socket.getState()));
}

void DroneTcpClient::socketPeerClosed(TcpSocket *socket_)
{
    ASSERT(socket_ == &socket);
    // close the connection (if not already closed)
    if (socket.getState() == TcpSocket::PEER_CLOSED) {
        EV_INFO << "remote TCP closed, closing here as well\n";
        close();
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DroneTcpClient::socketFailure(TcpSocket *, int code)
{
    // subclasses may override this function, and add code try to reconnect after a delay.
    EV_WARN << "connection broken\n";
    //numBroken++;
}

/*
void TcpAppBase::finish()
{
    std::string modulePath = getFullPath();

    EV_INFO << modulePath << ": opened " << numSessions << " sessions\n";
    EV_INFO << modulePath << ": sent " << bytesSent << " bytes in " << packetsSent << " packets\n";
    EV_INFO << modulePath << ": received " << bytesRcvd << " bytes in " << packetsRcvd << " packets\n";
}
*/

