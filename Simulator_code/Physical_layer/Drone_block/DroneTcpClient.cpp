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
        //sendMessageEvent = new cMessage("myTrigger");
        //scheduleAt(simTime() + 5.0, sendMessageEvent);
        Drone_ID = getParentModule()->par("Drone_ID");
        fileName = "Drone_logs/Drone_" + std::to_string(Drone_ID) + "_TcpClientLogFile.log";
        droneLogFile.open(fileName, std::ios::out);  // Create/open log file
        droneLogFile << "Bindport is: " << localPort << endl;
        droneLogFile << "BindAddress is: " << myIP << endl;

    }
}

void DroneTcpClient::handleMessageWhenUp(cMessage *msg) {
    if(msg->getArrivalGate() == gate("tcpAppIn")){
        connect(); //renew socket
        L3Address destination;
        int connectPort = msg->par("targetPort");
        L3AddressResolver().tryResolve(msg->par("targetAddress"), destination);
        if (destination.isUnspecified()) {
            EV_ERROR << "Connecting to " << msg->par("targetAddress") << " port=" << connectPort << ": cannot resolve destination address" << endl;
        }
        else {
            droneLogFile << "Connecting to " << destination << "(" << destination << ") port=" << connectPort << endl;
            socket.connect(destination, connectPort);
            msg->removeObject("targetAddress");
            msg->removeObject("targetPort");
            sendMessageEvent = msg->dup();
        }
    }
    else if (msg == sendMessageEvent){
        droneLogFile << simTime() << ": Sending data to server." << endl;
        std::ostringstream oss;

        int n = (msg->getParListPtr())->size();
        for (int i = 0; i < n; ++i) {
            auto param = msg->par(i);
            switch (param.getType()) {
                case cPar::BOOL:    oss << (param.boolValue() ? "true" : "false"); break;
                case cPar::DOUBLE:  oss << param.doubleValue(); break;
                case cPar::STRING:  oss << param.stringValue(); break;
                default:            oss << "[unknown]"; break;
            }
            if (i != n - 1) oss << " ";
        }
        std::string strData = oss.str();              // standard string
        const char* data = strdup(strData.c_str());   // C-style string (heap allocated)
        droneLogFile << simTime() << ": Data is: " << data << endl;
        auto payload = makeShared<BytesChunk>(std::vector<uint8_t>(data, data + strlen(data)));
        auto packet = new Packet("DroneData");
        packet->insertAtBack(payload);  // Attach payload to the packet
        socket.send(packet);
        cancelAndDelete(sendMessageEvent);
        close();
        droneLogFile << simTime() << ": Sending packet"<< endl;
    }
    //L3Address myIP = L3AddressResolver().resolve(getParentModule()->getFullPath().c_str());
    else {
        droneLogFile << "Received tcp msg: " << msg << endl;
        socket.processMessage(msg);
    }
}

void DroneTcpClient::socketEstablished(TcpSocket *socket) {
    droneLogFile << simTime() << ": TCP connection established, ready to send data." << endl;
    scheduleAt(simTime(), sendMessageEvent);
}

void DroneTcpClient::socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent) {
    EV << "Received response: " << msg->str() << endl;
    delete msg;
}

void DroneTcpClient::socketClosed(TcpSocket *socket) {
    EV << "Connection closed by server." << endl;
}
/*
void DroneTcpClient::sendRequest() {
    droneLogFile << "Sending data to server." << endl;
    auto packet = new Packet("DroneData");
    auto payload = makeShared<ByteCountChunk>();
    payload->setLength(B(100));  // Set non-empty payload

    packet->insertAtBack(payload);  // Attach payload to the packet
    socket.send(packet);
    //scheduleAt(simTime() + uniform(1, 5), sendMessageEvent);
}
*/
DroneTcpClient::~DroneTcpClient() {
    //cancelAndDelete(sendMessageEvent);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//After each connection socket need to be "renewed" - so we will use inherited function connect() to do that
void DroneTcpClient::connect()
{
    // we need a new connId if this is not the first connection
    socket.renewSocket();
    const char *localAddress = par("localAddress");
    int localPort = par("localPort");
    socket.bind(*localAddress ? L3AddressResolver().resolve(localAddress) : L3Address(), localPort);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DroneTcpClient::close() //Close the connection if not in use anymore
{
    EV_INFO << "issuing CLOSE command\n";
    socket.close();
    //emit(connectSignal, -1L);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


void DroneTcpClient::finish()
{
    std::string modulePath = getFullPath();

    //EV_INFO << modulePath << ": opened " << numSessions << " sessions\n";
    //EV_INFO << modulePath << ": sent " << bytesSent << " bytes in " << packetsSent << " packets\n";
    //EV_INFO << modulePath << ": received " << bytesRcvd << " bytes in " << packetsRcvd << " packets\n";
}


