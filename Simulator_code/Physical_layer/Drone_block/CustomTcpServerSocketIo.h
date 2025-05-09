/*
 * CustomTcpServerSocketIo.h
 *
 *  Created on: Apr 17, 2025
 *      Author: gekaizum
 */

#ifndef PHYSICAL_LAYER_DRONE_BLOCK_CUSTOMTCPSERVERSOCKETIO_H_
#define PHYSICAL_LAYER_DRONE_BLOCK_CUSTOMTCPSERVERSOCKETIO_H_

#include "inet/transportlayer/contract/tcp/TcpSocket.h"

using namespace omnetpp;
using namespace inet;

class CustomTcpServerSocketIo : public cSimpleModule, public TcpSocket::ICallback
{
protected:
  TcpSocket *socket = nullptr;

protected:
  virtual void handleMessage(cMessage *message) override;

public:
  virtual ~CustomTcpServerSocketIo() { delete socket; }

  virtual TcpSocket *getSocket() { return socket; }
  virtual void acceptSocket(TcpAvailableInfo *availableInfo);

  virtual void socketDataArrived(TcpSocket *socket, Packet *packet, bool urgent) override;
  virtual void socketAvailable(TcpSocket *socket, TcpAvailableInfo *availableInfo) override {}
  virtual void socketEstablished(TcpSocket *socket) override {}
  virtual void socketPeerClosed(TcpSocket *socket) override;
  virtual void socketClosed(TcpSocket *socket) override {}
  virtual void socketFailure(TcpSocket *socket, int code) override {}
  virtual void socketStatusArrived(TcpSocket *socket, TcpStatusInfo *status) override {}
  virtual void socketDeleted(TcpSocket *socket) override {}
};





#endif /* PHYSICAL_LAYER_DRONE_BLOCK_CUSTOMTCPSERVERSOCKETIO_H_ */
