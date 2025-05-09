//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef PHYSICAL_LAYER_DRONE_BLOCK_SMARTPACKETSINK_H_
#define PHYSICAL_LAYER_DRONE_BLOCK_SMARTPACKETSINK_H_

#include <omnetpp.h>
#include "inet/common/clock/ClockUserModuleMixin.h"
#include "inet/queueing/base/PassivePacketSinkBase.h"
#include "inet/queueing/contract/IActivePacketSource.h"


#include "inet/common/packet/Packet.h"
#include "inet/common/packet/printer/PacketPrinter.h"
#include "inet/common/packet/PacketFilter.h"
#include <iomanip>

using namespace inet;
using namespace queueing;
using namespace std;


extern template class ClockUserModuleMixin<queueing::PassivePacketSinkBase>;



class INET_API SmartPacketSink : public ClockUserModuleMixin<PassivePacketSinkBase>
{
  protected:
    clocktime_t initialConsumptionOffset;
    cPar *consumptionIntervalParameter = nullptr;
    ClockEvent *consumptionTimer = nullptr;
    bool scheduleForAbsoluteTime = false;

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *message) override;

    virtual void scheduleConsumptionTimer(clocktime_t delay);
    virtual void consumePacket(Packet *packet);

  public:
    virtual ~SmartPacketSink() { cancelAndDeleteClockEvent(consumptionTimer); }

    virtual bool supportsPacketPushing(cGate *gate) const override { return gate == inputGate; }
    virtual bool supportsPacketPulling(cGate *gate) const override { return false; }

    virtual bool canPushSomePacket(cGate *gate) const override;
    virtual bool canPushPacket(Packet *packet, cGate *gate) const override;

    virtual void pushPacket(Packet *packet, cGate *gate) override;
    virtual void finish(){};
};


#endif
