/*
 * DroneUdpApp.cpp
 *
 *  Created on: Apr 2, 2025
 *      Author: gekaizum
 */

#include "DroneUdpApp.h"
#include "inet/common/ModuleAccess.h"
//#include "inet/common/lifecycle/NodeOperations.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/common/packet/chunk/BytesChunk.h"

Define_Module(DroneUdpApp);

DroneUdpApp::~DroneUdpApp() {
    //cancelAndDelete(selfSendRequest);
}

void DroneUdpApp::initialize(int stage) {
    ApplicationBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {

    }
    else if (stage == INITSTAGE_APPLICATION_LAYER) {
        localPort = par("localPort");
        //destPort = par("destPort");
        //destAddress = L3AddressResolver().resolve(par("destAddress"));
        L3Address myIP = L3AddressResolver().resolve(getParentModule()->getFullPath().c_str());
        socket.setOutputGate(gate("socketOut"));
        socket.bind(myIP, localPort);
        EV << "Binded to: IP - " << myIP << " ,port - " << localPort << endl;
        socket.setCallback(this);
    }
}

void DroneUdpApp::handleMessageWhenUp(cMessage *msg) {
    if (msg->isSelfMessage()) {

    }
    else if (msg->arrivedOn("UdpAppIn")) {
        // External module wants us to send a UDP message
        sendUdpMessage("Hello from drone!");
    }
    else {
        socket.processMessage(msg);
    }
}

void DroneUdpApp::sendUdpMessage(const char *payload) {
    auto packet = new Packet("UDPMessage");
    // Convert C-string to vector<uint8_t>
    std::vector<uint8_t> bytes(payload, payload + strlen(payload));

    // Create BytesChunk from vector
    auto chunk = inet::makeShared<inet::BytesChunk>(bytes);
    packet->insertAtBack(chunk);
    socket.sendTo(packet, destAddress, destPort);
}

void DroneUdpApp::socketDataArrived(UdpSocket *socket, Packet *packet) {
    EV_INFO << "Received UDP packet: " << packet->getFullName() << endl;
    delete packet;
}

void DroneUdpApp::finish() {
    ApplicationBase::finish();
}



