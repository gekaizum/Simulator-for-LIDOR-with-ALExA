/*
 * CustomTcpServerSocketIo.cpp
 *
 *  Created on: Apr 17, 2025
 *      Author: gekaizum
 */

#include "Physical_layer/Drone_block/CustomTcpServerSocketIo.h"

#include "inet/common/socket/SocketTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"

using namespace omnetpp;
using namespace inet;

Define_Module(CustomTcpServerSocketIo);

void CustomTcpServerSocketIo::acceptSocket(TcpAvailableInfo *availableInfo)
{
    Enter_Method("acceptSocket");
    socket = new TcpSocket(availableInfo);
    socket->setOutputGate(gate("socketOut"));
    socket->setCallback(this);
    socket->accept(availableInfo->getNewSocketId());
}

void CustomTcpServerSocketIo::handleMessage(cMessage *message)
{
    if (message->arrivedOn("socketIn")) {
        ASSERT(socket != nullptr && socket->belongsToSocket(message));
        socket->processMessage(message);
    }
    else if (message->arrivedOn("trafficIn"))
        socket->send(check_and_cast<Packet *>(message));
    else
        throw cRuntimeError("Unknown message");
}

void CustomTcpServerSocketIo::socketDataArrived(TcpSocket *socket, Packet *packet, bool urgent)
{
    packet->removeTag<SocketInd>();
    send(packet, "trafficOut");
}

void CustomTcpServerSocketIo::socketPeerClosed(TcpSocket *socket){
    EV << simTime() << ": Closing socket: " << socket << endl;
    socket->close();
    cMessage *scE = new cMessage("connectionClosed");
    scE->addPar("State") = "connectionClosed";
    scE->setContextPointer(this);
    //scE->addPar("connection") = this;
    cModule *appModule = getParentModule()->getParentModule()->getSubmodule("listener");
    sendDirect(scE, 1, 0, appModule,"tcpAppIn");
}


