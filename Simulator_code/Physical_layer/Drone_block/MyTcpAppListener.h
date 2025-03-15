/*
 * DroneTcpApp.h
 *
 *  Created on: Feb 27, 2025
 *      Author: gekaizum
 */

#ifndef PHYSICAL_LAYER_DRONE_BLOCK_MYTCPAPPLISTENER_H_
#define PHYSICAL_LAYER_DRONE_BLOCK_MYTCPAPPLISTENER_H_

#include <omnetpp.h>
#include <inet/transportlayer/contract/tcp/TcpSocket.h>
#include <inet/applications/tcpapp/TcpAppBase.h>
#include "inet/networklayer/common/L3AddressResolver.h"
#include <inet/applications/tcpapp/TcpServerListener.h>
#include <fstream>
using namespace omnetpp;
using namespace inet;

class INET_API MyTcpAppListener : public ApplicationBase, public TcpSocket::ICallback
{
  protected:
    //TcpSocket socket;
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;  // Updated
    //virtual void socketDataArrived(TcpSocket *socket, Packet *pkt, bool urgent) override;
    //virtual void establishTcpConnection(const std::string &targetAddress, int targetPort);
    //virtual void sendTcpMessage(cMessage *msg);
    int connectionId = 0;
    TcpSocket serverSocket;
    std::set<TcpServerSocketIo *> connectionSet;
    static const char *submoduleVectorName;
    const char *localAddress;
    int localPort;
    //static simsignal_t connectSignal;

    //virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void finish() override;

    virtual void handleStartOperation(LifecycleOperation *operation) override {}
    virtual void handleStopOperation(LifecycleOperation *operation) override {}
    virtual void handleCrashOperation(LifecycleOperation *operation) override {}
  public:
    std::ofstream droneLogFile; // logger for drone
    std::string fileName;
    virtual void socketDataArrived(TcpSocket *socket, Packet *packet, bool urgent) override { throw cRuntimeError("Unexpected data"); }
    virtual void socketAvailable(TcpSocket *socket, TcpAvailableInfo *availableInfo) override;
    virtual void socketEstablished(TcpSocket *socket) override {}
    virtual void socketPeerClosed(TcpSocket *socket) override {}
    virtual void socketClosed(TcpSocket *socket) override;
    virtual void socketFailure(TcpSocket *socket, int code) override {}
    virtual void socketStatusArrived(TcpSocket *socket, TcpStatusInfo *status) override {}
    virtual void socketDeleted(TcpSocket *socket) override {}

    virtual void removeConnection(TcpServerSocketIo *connection);
    virtual void connectionClosed(TcpServerSocketIo *connection);
};


#endif /* PHYSICAL_LAYER_DRONE_BLOCK_MYTCPAPPLISTENER_H_ */
