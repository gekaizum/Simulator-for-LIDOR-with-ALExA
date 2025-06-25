/*
 * DroneTcpApp.cpp
 *
 *  Created on: Feb 27, 2025
 *      Author: gekaizum
 */

#include "MyTcpAppListener.h"
#include "inet/common/ModuleAccess.h"
#include "Physical_layer/Drone_block/CustomTcpServerSocketIo.h"
Define_Module(MyTcpAppListener);

const char *MyTcpAppListener::submoduleVectorName = "connection";

void MyTcpAppListener::initialize(int stage)
{
    ApplicationBase::initialize(stage);
    if (stage == INITSTAGE_APPLICATION_LAYER) {
        int id = getParentModule()->getParentModule()->par("Drone_ID");
        fileName = "Drone_logs/Drone_" + std::to_string(id) + "_TcpAppLogFile.log";
        droneLogFile.open(fileName, std::ios::out);  // Create/open log file
        if (!droneLogFile.is_open()) {
            EV << "Error: Could not open log file!" << endl;
        } else {
            droneLogFile << simTime() << ": Simulation started" << "\n";
            droneLogFile.flush();
        }

        localAddress = par("localAddress");
        localPort = par("localPort");
        serverSocket.setOutputGate(gate("socketOut"));
        serverSocket.setCallback(this);
        if (!getParentModule()->hasSubmoduleVector(submoduleVectorName))
            throw cRuntimeError("The submodule vector '%s' missing from %s", submoduleVectorName, getParentModule()->getFullPath().c_str());
        droneLogFile << simTime() << ": Initial socket setup completed" << endl;
        cModule *host = getContainingNode(this);
        L3Address myIP = L3AddressResolver().addressOf(host);
        droneLogFile << simTime() << ": My IP is:" << myIP << endl;
        serverSocket.bind(myIP, localPort); // Use ephemeral port
        serverSocket.listen();
        droneLogFile << simTime() << ": Socket setup completed" << endl;
    }
}

void MyTcpAppListener::handleMessageWhenUp(cMessage *msg)
{
    droneLogFile << simTime() << ": Message arrived: " << msg << endl;
    if (msg->isSelfMessage()){
    }
    else if (msg->arrivedOn("tcpAppIn")) {
        if (strcmp(msg->par("State").stringValue(), "connectionClosed") == 0){
            auto *conn = static_cast<CustomTcpServerSocketIo *>(msg->getContextPointer());
            connectionClosed(conn);
            delete(msg);
        }
    }
    else{
        droneLogFile << simTime() << ": TCP message arrived"<< ", message is: " << msg << endl;
        if (serverSocket.belongsToSocket(msg)) {
            //droneLogFile << "TCP message arrived at: "<< simTime() << " belongs to this socket" << endl; //need for debug
            serverSocket.processMessage(msg);
        }
        else {
            throw cRuntimeError("Unknown incoming message: '%s'", msg->getName());
        }
    }
}

void MyTcpAppListener::finish()
{
    droneLogFile << simTime() << ": finish() function was called" << endl;
    while (!connectionSet.empty()){
        droneLogFile << simTime() << ": Connection set is not empty yet" << endl;
        removeConnection(*connectionSet.begin());
    }
    droneLogFile << simTime() << ": Connection set is empty" << endl;
    serverSocket.close();
    droneLogFile.close();
}

void MyTcpAppListener::socketAvailable(TcpSocket *socket, TcpAvailableInfo *availableInfo)
{
    const char *serverConnectionModuleType = par("serverConnectionModuleType");
    cModuleType *moduleType = cModuleType::get(serverConnectionModuleType);
    cModule *parentModule = getParentModule();
    int submoduleIndex = parentModule->getSubmoduleVectorSize(submoduleVectorName);
    parentModule->setSubmoduleVectorSize(submoduleVectorName, submoduleIndex + 1);
    auto connection = moduleType->create(submoduleVectorName, parentModule, submoduleIndex);
    connection->finalizeParameters();
    connection->buildInside();
    connection->callInitialize();
    EV << "Creating dispatcher gate in: " << getName() << endl;
    auto dispatcher = check_and_cast<cSimpleModule *>(gate("socketIn")->getPathStartGate()->getOwnerModule());
    dispatcher->setGateSize("in", dispatcher->gateSize("in") + 1);
    dispatcher->setGateSize("out", dispatcher->gateSize("out") + 1);
    connection->gate("socketOut")->connectTo(dispatcher->gate("in", dispatcher->gateSize("in") - 1));
    dispatcher->gate("out", dispatcher->gateSize("out") - 1)->connectTo(connection->gate("socketIn"));
    auto serverSocketIo = check_and_cast<CustomTcpServerSocketIo *>(connection->gate("socketIn")->getPathEndGate()->getOwnerModule()); // @suppress("Function cannot be instantiated")

    serverSocketIo->acceptSocket(availableInfo);
    connectionSet.insert(serverSocketIo);
}

void MyTcpAppListener::socketClosed(TcpSocket *socket)
{
    if (operationalState == State::STOPPING_OPERATION && connectionSet.empty() && !serverSocket.isOpen())
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void MyTcpAppListener::removeConnection(CustomTcpServerSocketIo *connection)
{
    if (!connection) return;
    EV << "Erasing connection from list" << endl;
    for (auto conn : connectionSet) {
        EV << "Connection: " << conn << "\n";
        // Optional: print more info from each socket
    }
    connectionSet.erase(connection);
    cModule *sinkModule = connection->getParentModule()->getSubmodule("sink");
    EV << "Deleting sink module" << endl;
    sinkModule->callFinish();     // Clean up sink
    sinkModule->deleteModule();
    EV << "Deleting io module" << endl;
    cModule *con2 = connection->getParentModule();//->getParentModule()->getSubmodule(connection->getParentModule()->getName());
    connection->deleteModule();
    //con2->callFinish();
    EV << "Deleting connection module: "<< con2->getName() << endl;
    con2->callFinish();
    con2->deleteModule();
    EV << "Deleting dispatcher gates in: "<< getName() << endl;
    auto dispatcher = check_and_cast<cSimpleModule *>(gate("socketIn")->getPathStartGate()->getOwnerModule());
    if (!(dispatcher->gate("in", dispatcher->gateSize("in")-1)->isConnected())) {
        EV << "Deleting gate from: "<< dispatcher << endl;
        dispatcher->setGateSize("in", dispatcher->gateSize("in") - 1);
    }
    if (!(dispatcher->gate("out", dispatcher->gateSize("out")-1)->isConnected())) {
        EV << "Deleting gate from: "<< dispatcher << endl;
        dispatcher->setGateSize("out", dispatcher->gateSize("out") - 1);
    }
    cModule *parentModule = getParentModule();
    int submoduleIndex = parentModule->getSubmoduleVectorSize(submoduleVectorName);
    parentModule->setSubmoduleVectorSize(submoduleVectorName, submoduleIndex - 1);
    EV << "Connection vector size: " << submoduleIndex << endl;
}

void MyTcpAppListener::connectionClosed(CustomTcpServerSocketIo *connection)
{
    if (!connection) return;
    EV << "Erasing connection from list" << endl;
    for (auto conn : connectionSet) {
        EV << "Connection: " << conn << "\n";
        // Optional: print more info from each socket
    }
    connectionSet.erase(connection);
    cModule *sinkModule = connection->getParentModule()->getSubmodule("sink");
    EV << "Deleting sink module" << endl;
    sinkModule->callFinish();     // Clean up sink
    sinkModule->deleteModule();
    EV << "Deleting io module" << endl;
    cModule *con2 = connection->getParentModule();//->getParentModule()->getSubmodule(connection->getParentModule()->getName());
    connection->deleteModule();
    //con2->callFinish();
    EV << "Deleting connection module: "<< con2->getName() << endl;
    con2->callFinish();
    con2->deleteModule();
    EV << "Deleting dispatcher gates in: "<< getName() << endl;
    auto dispatcher = check_and_cast<cSimpleModule *>(gate("socketIn")->getPathStartGate()->getOwnerModule());
    if (!(dispatcher->gate("in", dispatcher->gateSize("in")-1)->isConnected())) {
        EV << "Deleting gate from: "<< dispatcher << endl;
        dispatcher->setGateSize("in", dispatcher->gateSize("in") - 1);
    }
    if (!(dispatcher->gate("out", dispatcher->gateSize("out")-1)->isConnected())) {
        EV << "Deleting gate from: "<< dispatcher << endl;
        dispatcher->setGateSize("out", dispatcher->gateSize("out") - 1);
    }
    cModule *parentModule = getParentModule();
    int submoduleIndex = parentModule->getSubmoduleVectorSize(submoduleVectorName);
    parentModule->setSubmoduleVectorSize(submoduleVectorName, submoduleIndex - 1);
    EV << "Connection vector size: " << submoduleIndex << endl;
}


