/*
 * DroneUdpApp.h
 *
 *  Created on: Apr 2, 2025
 *      Author: gekaizum
 */

#ifndef PHYSICAL_LAYER_DRONE_BLOCK_DRONEUDPAPP_H_
#define PHYSICAL_LAYER_DRONE_BLOCK_DRONEUDPAPP_H_


#include "inet/common/INETDefs.h"
#include "inet/applications/base/ApplicationBase.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"

using namespace inet;

class DroneUdpApp : public ApplicationBase, public UdpSocket::ICallback
{
  protected:
    // UDP socket
    UdpSocket socket;

    // Parameters
    L3Address destAddress;
    int localPort = -1;
    int destPort = -1;

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void finish() override;
    virtual void sendUdpMessage(const char *payload);

    // UdpSocket::ICallback
    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
    virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override {}
    virtual void socketClosed(UdpSocket *socket) override {}

    virtual void handleStartOperation(LifecycleOperation *operation) override {}
    virtual void handleStopOperation(LifecycleOperation *operation) override {}
    virtual void handleCrashOperation(LifecycleOperation *operation) override {}

  public:
    DroneUdpApp() {}
    virtual ~DroneUdpApp();
};




#endif /* PHYSICAL_LAYER_DRONE_BLOCK_DRONEUDPAPP_H_ */
