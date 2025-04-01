/*
 * DroneTcpClient.h
 *
 *  Created on: Mar 12, 2025
 *      Author: gekaizum
 */

#ifndef __DRONE_TCP_CLIENT_H_
#define __DRONE_TCP_CLIENT_H_

#include "inet/common/INETDefs.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"
#include <fstream>
#include "inet/applications/base/ApplicationBase.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"

using namespace omnetpp;
using namespace inet;
//based on TcpAppBase
class INET_API DroneTcpClient : public ApplicationBase, public TcpSocket::ICallback {
protected:
    TcpSocket socket;
    bool autoRead;
    // statistics
    //int numSessions;
    //int numBroken;
    //int packetsSent;
    //int packetsRcvd;
    //int bytesSent;
    //int bytesRcvd;

    // statistics:
    static simsignal_t connectSignal;

    cMessage *sendMessageEvent = nullptr;
    cMessage *trigger2 = nullptr;
    const char *localAddress;
    int localPort;
    int Drone_ID;

    virtual void handleStartOperation(LifecycleOperation *operation) override {}
    virtual void handleStopOperation(LifecycleOperation *operation) override {}
    virtual void handleCrashOperation(LifecycleOperation *operation) override {}
    virtual void handleTimer(omnetpp::cMessage*) {}

    virtual void initialize(int stage) override;

    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void socketEstablished(TcpSocket *socket) override;
    virtual void socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent) override;
    virtual void socketClosed(TcpSocket *socket) override;
    virtual void sendRequest();

    virtual void refreshDisplay() const override;
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    /* Utility functions */
    virtual void connect();
    virtual void close();
    virtual void sendPacket(Packet *pkt);
    virtual void socketAvailable(TcpSocket *socket, TcpAvailableInfo *availableInfo) override { socket->accept(availableInfo->getNewSocketId()); }
    virtual void socketPeerClosed(TcpSocket *socket) override;
    virtual void socketFailure(TcpSocket *socket, int code) override;
    virtual void socketStatusArrived(TcpSocket *socket, TcpStatusInfo *status) override {}
    virtual void socketDeleted(TcpSocket *socket) override {}
public:
    std::ofstream droneLogFile; // logger for drone
    std::string fileName;
    virtual ~DroneTcpClient();
};
Define_Module(DroneTcpClient);
#endif


