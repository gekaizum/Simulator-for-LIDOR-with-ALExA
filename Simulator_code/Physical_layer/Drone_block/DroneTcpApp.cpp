/*
 * DroneTcpApp.cpp
 *
 *  Created on: Feb 27, 2025
 *      Author: gekaizum
 */

#include "DroneTcpApp.h"

Define_Module(DroneTcpApp);

void DroneTcpApp::initialize(int stage)
{
    TcpAppBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        socket.setOutputGate(gate("socketOut"));
        socket.setCallback(this);
    }
}

void DroneTcpApp::handleMessage(cMessage *msg)
{
    if (msg->arrivedOn("controlIn")) {
        // External request to establish a new TCP connection
        std::string targetAddress;// = msg->par("targetAddress").setStringValue();
        int targetPort = msg->par("targetPort").doubleValue();

        establishTcpConnection(targetAddress, targetPort);
    }
    else if (strcmp(msg->getName(), "sendTCP") == 0) {
        EV<< "TcpHandler" << endl;
        sendTcpMessage(msg);
    }

    delete msg;
}

void DroneTcpApp::establishTcpConnection(const std::string &targetAddress, int targetPort)
{
    if (targetAddress.empty()) {
        EV << "No target address specified, skipping TCP connection.\n";
        return;
    }

    EV << "Attempting to establish TCP connection to " << targetAddress << ":" << targetPort << "\n";
    socket.connect(L3AddressResolver().resolve(targetAddress.c_str()), targetPort);
}

void DroneTcpApp::sendTcpMessage(cMessage *msg)
{

    /*if (!socket.isOpen()) {
        EV << "Socket is not connected. Ignoring message send request.\n";
        return;
    }*/
    //std::string addrStr = msg->par("targetAddress").stringValue();
    //inet::L3Address targetAddress = inet::L3Address(addrStr.c_str());
    std::string targetAddress=msg->par("targetAddress").stringValue();
    int targetPort = msg->par("targetPort").doubleValue();
    EV << "targetPort is:" << targetPort << endl;
    EV<< "targetAddress is:" << targetAddress << endl;
    establishTcpConnection(targetAddress, targetPort);

    int msgLen = par("messageLength").intValue();
    auto packet = new Packet("TCPData");
    // Create a byte vector of the correct length (filled with dummy data)
    std::vector<uint8_t> byteData(msgLen, 0);

    // Create a BytesChunk from the byte vector
    const auto& payload = makeShared<BytesChunk>(byteData);
    packet->insertAtBack(payload);

    EV << "Sending TCP packet to " << socket.getRemoteAddress() << endl;
    socket.send(packet);
}

void DroneTcpApp::socketDataArrived(TcpSocket *socket, Packet *pkt, bool urgent)
{
    EV << "📩 Received TCP packet from " << socket->getRemoteAddress() << endl;

    // Extract message payload safely
    auto payload = pkt->peekDataAt<BytesChunk>(B(0), pkt->getDataLength());
    std::string receivedData(reinterpret_cast<const char*>(payload->getBytes().data()), payload->getBytes().size());

    EV << "📩 Message Content: " << receivedData << endl;

    // Example: Responding back (Optional)
    auto response = new Packet("TCPResponse");
    auto responsePayload = makeShared<BytesChunk>();  // Correct way to instantiate BytesChunk
    std::vector<uint8_t> responseData(512, 'A');  // Create a response with 512 'A' characters
    responsePayload->setBytes(responseData);
    response->insertAtBack(responsePayload);

    EV << "📤 Sending Acknowledgment to " << socket->getRemoteAddress() << "\n";
    socket->send(response);

    // Cleanup
    delete pkt;
}





