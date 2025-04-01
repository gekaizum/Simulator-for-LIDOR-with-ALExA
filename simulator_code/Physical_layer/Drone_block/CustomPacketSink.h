/*
 * CustomPacketSink.h
 *
 *  Created on: Mar 31, 2025
 *      Author: gekaizum
 */

#ifndef PHYSICAL_LAYER_DRONE_BLOCK_CUSTOMPACKETSINK_H_
#define PHYSICAL_LAYER_DRONE_BLOCK_CUSTOMPACKETSINK_H_


#include "inet/queueing/base/PassivePacketSinkBase.h"
#include "inet/common/clock/ClockUserModuleMixin.h"

#include "inet/queueing/contract/IActivePacketSource.h"

namespace inet {

extern template class ClockUserModuleMixin<queueing::PassivePacketSinkBase>;

namespace queueing {

class INET_API CustomPacketSink : public ClockUserModuleMixin<PassivePacketSinkBase>
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
    virtual ~CustomPacketSink() { cancelAndDeleteClockEvent(consumptionTimer); }

    virtual bool supportsPacketPushing(cGate *gate) const override { return gate == inputGate; }
    virtual bool supportsPacketPulling(cGate *gate) const override { return false; }

    virtual bool canPushSomePacket(cGate *gate) const override;
    virtual bool canPushPacket(Packet *packet, cGate *gate) const override;

    virtual void pushPacket(Packet *packet, cGate *gate) override;
};

} // namespace queueing
} // namespace inet




#endif /* PHYSICAL_LAYER_DRONE_BLOCK_CUSTOMPACKETSINK_H_ */
