/*
 * DroneUdpApp.cpp
 *
 *  Created on: Apr 2, 2025
 *      Author: gekaizum
 */

#include "DroneUdpIo.h"
#include "inet/common/ModuleAccess.h"
//#include "inet/common/lifecycle/NodeOperations.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/common/packet/chunk/BytesChunk.h"
#include "inet/common/socket/SocketTag_m.h"

Define_Module(DroneUdpIo);

DroneUdpIo::~DroneUdpIo() {
    //cancelAndDelete(selfSendRequest);
}

void DroneUdpIo::initialize(int stage) {
    ApplicationBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {

    }
    else if (stage == INITSTAGE_APPLICATION_LAYER) {
        localPort = par("localPort");
        L3Address myIP = L3AddressResolver().resolve(getParentModule()->getParentModule()->getFullPath().c_str());
        socket.setOutputGate(gate("socketOut"));
        socket.bind(myIP, localPort);
        socket.setCallback(this);
        Drone_ID = getParentModule()->getParentModule()->par("Drone_ID");
        fileName = "Drone_logs/Drone_" + std::to_string(Drone_ID) + "_UdpAppLogFile.log";
        droneLogFile.open(fileName, std::ios::out);  // Create/open log file
        droneLogFile << simTime() << ": Bindport is: " << localPort << endl;
        droneLogFile << simTime() << ": BindAddress is: " << myIP << endl;
    }
}

void DroneUdpIo::handleMessageWhenUp(cMessage *msg) {
    if (msg->arrivedOn("udpAppIn")) {
        destPort = msg->par("targetPort");
        L3AddressResolver().tryResolve(msg->par("targetAddress"), destAddress);
        if (destAddress.isUnspecified()) {
            EV_ERROR << "Connecting to " << msg->par("targetAddress") << " port=" << destPort << ": cannot resolve destination address" << endl;
        }
        else {
            msg->removeObject("targetAddress");
            msg->removeObject("targetPort");
            sendMessageEvent = msg->dup();
            scheduleAt(simTime(), sendMessageEvent);
        }
        delete(msg);
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
        socket.sendTo(packet, destAddress, destPort);
        cancelAndDelete(sendMessageEvent);
    }
    else {
        socket.processMessage(msg);
    }
}

void DroneUdpIo::socketDataArrived(UdpSocket *socket, Packet *packet) {
    droneLogFile << simTime() << ": Received UDP packet: " << packet->getFullName() << endl;
    //delete packet;
    packet->removeTag<SocketInd>();
    send(packet, "trafficOut");
}

void DroneUdpIo::finish() {
    ApplicationBase::finish();
}



