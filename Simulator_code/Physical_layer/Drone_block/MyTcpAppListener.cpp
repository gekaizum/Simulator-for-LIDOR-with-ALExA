/*
 * DroneTcpApp.cpp
 *
 *  Created on: Feb 27, 2025
 *      Author: gekaizum
 */

#include "MyTcpAppListener.h"
#include "inet/common/ModuleAccess.h"
Define_Module(MyTcpAppListener);

//simsignal_t TcpAppBase::connectSignal = registerSignal("connect");
const char *MyTcpAppListener::submoduleVectorName = "connection";

void MyTcpAppListener::initialize(int stage)
{
    ApplicationBase::initialize(stage);
    //LifecycleOperation *operation = nullptr;
    //TcpServerListener::handleStartOperation(operation);
    if (stage == INITSTAGE_LOCAL) {
        //getParentModule()->Drone_ID;                         // Get the drone's ID from parameters
        int id = getParentModule()->getParentModule()->par("Drone_ID");
        fileName = "Drone_logs/Drone_" + std::to_string(id) + "_TcpAppLogFile.log";
        droneLogFile.open(fileName, std::ios::out);  // Create/open log file
        if (!droneLogFile.is_open()) {
            EV << "Error: Could not open log file!" << endl;
        } else {
            droneLogFile << "Simulation started at: " << simTime() << "\n";
            droneLogFile.flush();
        }

        localAddress = par("localAddress");
        localPort = par("localPort");
        serverSocket.setAutoRead(par("autoRead"));
        serverSocket.setOutputGate(gate("socketOut"));
        serverSocket.setCallback(this);
        cMessage *temp = new cMessage("socketSetup");
        scheduleAt(simTime() + 1.0, temp);
        if (!getParentModule()->hasSubmoduleVector(submoduleVectorName))
            throw cRuntimeError("The submodule vector '%s' missing from %s", submoduleVectorName, getParentModule()->getFullPath().c_str());
        droneLogFile << "Initial socket setup completed" << endl;
    }
}

void MyTcpAppListener::handleMessageWhenUp(cMessage *msg)
{
    droneLogFile << "Message arrived at: "<< simTime() << endl;
    if (msg->isSelfMessage()){
        cModule *host = getContainingNode(this);
        L3Address myIP = L3AddressResolver().addressOf(host);
        //L3Address myIP = L3AddressResolver().resolve(getParentModule()->getFullPath().c_str());
        droneLogFile << "My IP is:" << myIP << endl;
        serverSocket.bind(myIP, localPort); // Use ephemeral port
        //serverSocket.bind(localAddress[0] ? L3Address(localAddress) : L3Address(), localPort);
        serverSocket.listen();
        droneLogFile << "Socket setup completed" << endl;
    }
    else if (msg->arrivedOn("tcpAppIn")) {
        // External request to establish a new TCP connection
        droneLogFile << "Control asks to open new connection" << endl;
        //sendTcpMessage(msg);
        // if (strcmp(msg->getName(), "sendTCP") == 0)
        droneLogFile << "Packet sent" << endl;
    }
    else{
        //TcpServerListener::handleMessageWhenUp(msg);
        serverSocket.processMessage(msg);
        if (serverSocket.belongsToSocket(msg))
            serverSocket.processMessage(msg);
        else
            throw cRuntimeError("Unknown incoming message: '%s'", msg->getName());

    }

    delete msg;
}
/*
void MyTcpAppListener::establishTcpConnection(const std::string &targetAddress, int targetPort)
{
    if (targetAddress.empty()) {
        droneLogFile << "No target address specified, skipping TCP connection." << endl;
        return;
    }

    droneLogFile << "Attempting to establish TCP connection to " << targetAddress << ":" << targetPort << endl;
    serverSocket.connect(L3AddressResolver().resolve(targetAddress.c_str()), targetPort);
}

void MyTcpAppListener::sendTcpMessage(cMessage *msg)
{

    /*if (!socket.isOpen()) {
        EV << "Socket is not connected. Ignoring message send request.\n";
        return;
    }
    //std::string addrStr = msg->par("targetAddress").stringValue();
    //inet::L3Address targetAddress = inet::L3Address(addrStr.c_str());
    std::string targetAddress=msg->par("targetAddress").stringValue();
    int targetPort = msg->par("targetPort").doubleValue();
    droneLogFile << "targetPort is:" << targetPort << endl;
    droneLogFile << "targetAddress is:" << targetAddress << endl;
    establishTcpConnection(targetAddress, targetPort);

    int msgLen = 5;//par("messageLength").intValue();
    auto packet = new Packet("TCPData");
    // Create a byte vector of the correct length (filled with dummy data)
    std::vector<uint8_t> byteData(msgLen, 0);

    // Create a BytesChunk from the byte vector
    const auto& payload = makeShared<BytesChunk>(byteData);
    packet->insertAtBack(payload);
    droneLogFile << "Sending TCP packet to " << serverSocket.getRemoteAddress() << endl;
    serverSocket.send(packet);
}
*/
/*
void MyTcpAppListener::socketDataArrived(TcpSocket *socket, Packet *pkt, bool urgent)
{
    droneLogFile << "📩 Received TCP packet from " << socket->getRemoteAddress() << endl;

    // Extract message payload safely
    auto payload = pkt->peekDataAt<BytesChunk>(B(0), pkt->getDataLength());
    std::string receivedData(reinterpret_cast<const char*>(payload->getBytes().data()), payload->getBytes().size());

    droneLogFile << "📩 Message Content: " << receivedData << endl;

    // Example: Responding back (Optional)
    auto response = new Packet("TCPResponse");
    auto responsePayload = makeShared<BytesChunk>();  // Correct way to instantiate BytesChunk
    std::vector<uint8_t> responseData(512, 'A');  // Create a response with 512 'A' characters
    responsePayload->setBytes(responseData);
    response->insertAtBack(responsePayload);

    droneLogFile << "📤 Sending Acknowledgment to " << socket->getRemoteAddress() << endl;
    socket->send(response);

    // Cleanup
    delete pkt;
}
*/

void MyTcpAppListener::finish()
{
    while (!connectionSet.empty())
        removeConnection(*connectionSet.begin());
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
    auto dispatcher = check_and_cast<cSimpleModule *>(gate("socketIn")->getPathStartGate()->getOwnerModule());
    dispatcher->setGateSize("in", dispatcher->gateSize("in") + 1);
    dispatcher->setGateSize("out", dispatcher->gateSize("out") + 1);
    connection->gate("socketOut")->connectTo(dispatcher->gate("in", dispatcher->gateSize("in") - 1));
    dispatcher->gate("out", dispatcher->gateSize("out") - 1)->connectTo(connection->gate("socketIn"));
    auto serverSocketIo = check_and_cast<TcpServerSocketIo *>(connection->gate("socketIn")->getPathEndGate()->getOwnerModule());
    serverSocketIo->acceptSocket(availableInfo);
    connectionSet.insert(serverSocketIo);
}

void MyTcpAppListener::socketClosed(TcpSocket *socket)
{
    if (operationalState == State::STOPPING_OPERATION && connectionSet.empty() && !serverSocket.isOpen())
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void MyTcpAppListener::removeConnection(TcpServerSocketIo *connection)
{
    connectionSet.erase(connection);
    connection->deleteModule();
}

void MyTcpAppListener::connectionClosed(TcpServerSocketIo *connection)
{
    connectionSet.erase(connection);
    socketClosed(connection->getSocket());
    connection->deleteModule();
}


