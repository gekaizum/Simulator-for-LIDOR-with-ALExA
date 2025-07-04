/*
 * SmartPacketSink.cpp
 *
 *  Created on: Apr 3, 2025
 *      Author: shaked
 */

//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include "SmartPacketSink.h"

Define_Module(SmartPacketSink);

void SmartPacketSink::initialize(int stage)
{
    ClockUserModuleMixin::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        initialConsumptionOffset = par("initialConsumptionOffset");
        consumptionIntervalParameter = &par("consumptionInterval");
        consumptionTimer = new ClockEvent("ConsumptionTimer");
        scheduleForAbsoluteTime = par("scheduleForAbsoluteTime");
    }
    else if (stage == INITSTAGE_QUEUEING) {
        checkPacketOperationSupport(inputGate);
        if (producer != nullptr)
            producer->handleCanPushPacketChanged(inputGate->getPathStartGate());
        if (!consumptionTimer->isScheduled() && initialConsumptionOffset != 0)
            scheduleConsumptionTimer(initialConsumptionOffset);
    }
}

void SmartPacketSink::handleMessage(cMessage *message)
{
    if (message == consumptionTimer) {
        if (producer != nullptr)
            producer->handleCanPushPacketChanged(inputGate->getPathStartGate());
    }
    else
        PassivePacketSinkBase::handleMessage(message);
}

bool SmartPacketSink::canPushSomePacket(cGate *gate) const
{
    return getClockTime() >= initialConsumptionOffset && !consumptionTimer->isScheduled() && PassivePacketSinkBase::canPushSomePacket(gate);
}

bool SmartPacketSink::canPushPacket(Packet *packet, cGate *gate) const
{
    return canPushSomePacket(gate);
}

void SmartPacketSink::scheduleConsumptionTimer(clocktime_t delay)
{
    if (delay != 0 || consumptionTimer->getArrivalModule() == nullptr) {
        if (scheduleForAbsoluteTime)
            scheduleClockEventAt(getClockTime() + delay, consumptionTimer);
        else
            scheduleClockEventAfter(delay, consumptionTimer);
    }
}

void SmartPacketSink::pushPacket(Packet *packet, cGate *gate)
{
    Enter_Method("pushPacket");
    take(packet);
    if (consumptionTimer->isScheduled() && consumptionTimer->getArrivalTime() > simTime())
        throw cRuntimeError("Another packet is already being consumed");
    else {
        emit(packetPushedSignal, packet);
        consumePacket(packet);
        scheduleConsumptionTimer(consumptionIntervalParameter->doubleValue());
    }
}

void SmartPacketSink::consumePacket(Packet *packet)
{
    EV_INFO << "Consuming packet" << EV_FIELD(packet) << EV_ENDL;
    numProcessedPackets++;
    processedTotalLength += packet->getDataLength();
    updateDisplayString();

    const auto& chunk = packet->peekDataAt<BytesChunk>(b(0), packet->getDataLength());
    vector<uint8_t> rawBytes = chunk->getBytes();
    string payload(rawBytes.begin(), rawBytes.end());

    istringstream iss(payload);
    string command;
    iss >> command;

    string paramLine;
    getline(iss, paramLine);
    if (!paramLine.empty() && paramLine[0] == ' ')
        paramLine = paramLine.substr(1);

    cMessage *msg = nullptr;

    if (command == "SETBASE") {
        double x, y, z;
        istringstream(paramLine) >> x >> y >> z;
        msg = new cMessage("setBase");
        msg->addPar("State") = "SETBASE";
        msg->addPar("x") = x;
        msg->addPar("y") = y;
        msg->addPar("z") = z;
    }
    else if (command == "TAKEOFF") {
        double x, y, z;
        istringstream(paramLine) >> x >> y >> z;
        msg = new cMessage("takeOff");
        msg->addPar("State") = "TAKEOFF";
        msg->addPar("x") = x;
        msg->addPar("y") = y;
        msg->addPar("z") = z;
    }
    else if (command == "LANDING") {
        msg = new cMessage("landDrone");
        msg->addPar("State") = "LANDING";
    }
    else if (command == "MOVE") {
        double x, y, z;
        istringstream(paramLine) >> x >> y >> z;
        msg = new cMessage("moveTo");
        msg->addPar("State") = "MOVE";
        msg->addPar("x") = x;
        msg->addPar("y") = y;
        msg->addPar("z") = z;
    }
    else if (command == "SETVEL") {
        double vx, vy, vz;
        istringstream(paramLine) >> vx >> vy >> vz;
        msg = new cMessage("setVelocity");
        msg->addPar("State") = "SETVEL";
        msg->addPar("x_vel") = vx;
        msg->addPar("y_vel") = vy;
        msg->addPar("z_vel") = vz;
    }
    else if (command == "SETACCEL") {
        double accel;
        istringstream(paramLine) >> accel;
        msg = new cMessage("setAcceleration");
        msg->addPar("State") = "SETACCEL";
        msg->addPar("acceleration") = accel;
    }
    else if (command == "STOP") {
        msg = new cMessage("stopDrone");
        msg->addPar("State") = "STOP";
    }
    else if (command == "POWER_ON") {
        double x, y, z;
        istringstream(paramLine) >> x >> y >> z;
        msg = new cMessage("powerOnDrone");
        msg->addPar("State") = "POWER_ON";
        msg->addPar("x") = x;
        msg->addPar("y") = y;
        msg->addPar("z") = z;
    }
    else if (command == "POWER_OFF") {
        msg = new cMessage("powerOffDrone");
        msg->addPar("State") = "POWER_OFF";
    }
    else if (command == "SEND_STAT") {
        msg = new cMessage("getStatusDrone");
        msg->addPar("State") = "SEND_STAT";
    }
    else if (command == "SEND_POS") {
        msg = new cMessage("getPositionDrone");
        msg->addPar("State") = "SEND_POS";
    }
    else if (command == "SEND_ALT") {
        msg = new cMessage("getAltitudeDrone");
        msg->addPar("State") = "SEND_ALT";
    }
    else if (command == "SEND_BATT") {
        msg = new cMessage("getBatteryDrone");
        msg->addPar("State") = "SEND_BATT";
    }
    else {
        EV<< "not CMD packet - dropPacket" << endl;
        dropPacket(packet, OTHER_PACKET_DROP);
    }
    // Send message to the BasicProtocol module
    if (msg != nullptr) {
        EV_INFO << "Forwarding command [" << msg->getName() << "] to droneControl\n";
        EV_INFO << "Message object: " << msg->str() << "\n";

        //cModule *drone = getParentModule()->getParentModule()->getParentModule();
        cModule *drone = getContainingNode(this);
        int droneIndex = drone->getIndex();
        string path = "DroneNetwork.drones[" + std::to_string(droneIndex) + "].droneControl";
        cModule *droneControl = getModuleByPath(path.c_str());

        sendDirect(msg, droneControl, "tcpAppIn");
        dropPacket(packet, OTHER_PACKET_DROP);
    }
}
