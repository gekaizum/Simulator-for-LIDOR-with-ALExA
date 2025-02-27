/*
 * DroneTcpApp.h
 *
 *  Created on: Feb 27, 2025
 *      Author: gekaizum
 */

#ifndef PHYSICAL_LAYER_DRONE_BLOCK_DRONETCPAPP_H_
#define PHYSICAL_LAYER_DRONE_BLOCK_DRONETCPAPP_H_

#include <omnetpp.h>
#include <inet/transportlayer/contract/tcp/TcpSocket.h>
#include <inet/applications/tcpapp/TcpAppBase.h>
#include "inet/networklayer/common/L3AddressResolver.h"

using namespace omnetpp;
using namespace inet;

class DroneTcpApp : public TcpAppBase
{
  protected:
    TcpSocket socket;
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;  // Updated
    virtual void socketDataArrived(TcpSocket *socket, Packet *pkt, bool urgent) override;
    virtual void establishTcpConnection(const std::string &targetAddress, int targetPort);
    virtual void sendTcpMessage(cMessage *msg);
    virtual void handleStartOperation(LifecycleOperation *operation){}
    virtual void handleStopOperation(LifecycleOperation *operation){}
    virtual void handleCrashOperation(LifecycleOperation *operation){}
    virtual void handleTimer(cMessage *msg){}
};


#endif /* PHYSICAL_LAYER_DRONE_BLOCK_DRONETCPAPP_H_ */
