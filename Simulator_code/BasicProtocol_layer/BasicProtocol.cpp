/*
 * BasicProtocol.cpp
 *
 *  Created on: Feb 16, 2025
 *      Author: gekaizum
 */
#include "BasicProtocol.h"

void BasicProtocol::initialize() { // Initializes the drone module
    BPLogger = new BasicProtocolLogger();
    BPLogger->logFile.flush();
    message_init();
    controlModule = check_and_cast<SimulationControl*>(getParentModule()->getSubmodule("simControl"));
    BPLogger->logFile << simTime() << ": Basic protocol layer online" << endl;
    getDisplayString().setTagArg("i", 0, "");
}
void BasicProtocol::handleMessage(cMessage *msg){ // Handles incoming messages

}
void BasicProtocol::finish(){
    delete(setBase);
    delete(takeOff);
    delete(landDrone);
    delete(moveTo);
    delete(setVelocity);
    delete(setAcceleration);
    delete(getStatusDrone);
    delete(getPositionDrone);
    delete(getAltitudeDrone);
    delete(getBatteryDrone);
    delete(stopDrone);
    delete(powerOnDrone);
    delete(powerOffDrone);
}
void BasicProtocol::message_init(){
    //movement cMessages
    /**/
    setBase = new cMessage("setBase");
    setBase->addPar("State") = "SETBASE";
    /**/
    takeOff = new cMessage("takeOff");
    takeOff->addPar("State") = "TAKEOFF";
    /**/
    landDrone = new cMessage("landDrone");
    landDrone->addPar("State") = "LANDING";
    /**/
    moveTo = new cMessage("moveTo");
    moveTo->addPar("State") = "MOVE";
    /**/
    setVelocity = new cMessage("setVelocity");
    setVelocity->addPar("State") = "SETVEL";
    /**/
    setAcceleration = new cMessage("setAcceleration");
    setAcceleration->addPar("State") = "SETACCEL";
    /**/
    retBase = new cMessage("retBase");
    retBase->addPar("State") = "RETURN";
    /**/
    BPLogger->logFile << simTime() << ": Movement cMessages initialized" << endl;
    ///////////////////////////////////////////////////////////////
    //State management cMessages
    /**/
    stopDrone = new cMessage("stopDrone");
    stopDrone->addPar("State") = "STOP";
    /**/
    powerOnDrone = new cMessage("powerOnDrone");
    powerOnDrone->addPar("State") = "POWER_ON";
    /**/
    powerOffDrone = new cMessage("powerOffDrone");
    powerOffDrone->addPar("State") = "POWER_OFF";
    /**/
    BPLogger->logFile << simTime() << ": State management cMessages initialized" << endl;
    /////////////////////////////////////////////////////////////////////
    //Sensor, data and monitoring cMessages
    /**/
    getStatusDrone = new cMessage("getStatusDrone");
    getStatusDrone->addPar("State") = "SEND_STAT";
    /**/
    getPositionDrone = new cMessage("getPositionDrone");
    getPositionDrone->addPar("State") = "SEND_POS";
    /**/
    getAltitudeDrone = new cMessage("getAltitudeDrone");
    getAltitudeDrone->addPar("State") = "SEND_ALT";
    /**/
    getBatteryDrone = new cMessage("getBatteryDrone");
    getBatteryDrone->addPar("State") = "SEND_BATT";
    /**/
    BPLogger->logFile << simTime() << ": Sensor, data and monitoring cMessages initialized" << endl;
    /////////////////////////////////////////////////////////////////////
}
//movement commands
bool BasicProtocol::set_base(int protocol, int drone_id_sender, int drone_id_receiver, double x_base, double y_base, double z_base){
    BPLogger->logFile << simTime() << ": set_base command received" << endl;
    Enter_Method("set_base");
    cMessage *msg = setBase->dup();
    msg->addPar("x") = x_base;
    msg->addPar("y") = y_base;
    msg->addPar("z") = z_base;
    if(protocol == 1){
        send(msg, "dronesSocket$o",drone_id_receiver-1);
        BPLogger->logFile << simTime() << ": Base station coordinates sent to drone with ID: "<< drone_id_receiver << " via cMessages." << endl;
        return true;
    }
    else if (protocol == 2){//UDP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending UDP message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("udpPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"udpRequestIn");
        BPLogger->logFile << simTime() << ": Base station coordinates sent from drone with ID: " << drone_id_sender <<
                " to drone with ID: "<< drone_id_receiver << " via UDP." << endl;
        return true;
    }
    else{//TCP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending TCP message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("connectPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"tcpRequestIn");
        BPLogger->logFile << simTime() << ": Base station coordinates sent from drone with ID: " << drone_id_sender <<
                        " to drone with ID: "<< drone_id_receiver << " via TCP." << endl;
        return true;
    }
    BPLogger->logFile << simTime() << ": set_base command: message passing protocol unrecognized, received: " << protocol << endl;
    return false;
}
bool BasicProtocol::take_off(int protocol,int drone_id_sender, int drone_id_receiver, double x_dest, double y_dest, double z_dest){
    BPLogger->logFile << simTime() << ": take_off command received" << endl;
    Enter_Method("take_off");
    //controlModule->height_checker(x_dest, y_dest,heightVal);
    //z_dest += heightVal;
    cMessage *msg = takeOff->dup();
    msg->addPar("x") = x_dest;
    msg->addPar("y") = y_dest;
    msg->addPar("z") = z_dest;
    if(protocol == 1){
        send(msg, "dronesSocket$o",drone_id_receiver-1);
        BPLogger->logFile << simTime() << ": Take off sequence sent to drone with ID: "<< drone_id_receiver << " via cMessages." << endl;
        return true;
    }
    else if (protocol == 2){//UDP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending UDP message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("udpPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"udpRequestIn");
        BPLogger->logFile << simTime() << ": Take off sequence sent from drone with ID: " << drone_id_sender <<
                        " to drone with ID: "<< drone_id_receiver << " via UDP." << endl;
        return true;
    }
    else{//TCP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending TCP message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("connectPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"tcpRequestIn");
        BPLogger->logFile << simTime() << ": Take off sequence sent from drone with ID: " << drone_id_sender <<
                                " to drone with ID: "<< drone_id_receiver << " via TCP." << endl;
        return true;
    }
    BPLogger->logFile << simTime() << ": take_off command: message passing protocol unrecognized, received: " << protocol << endl;
    return false;
}
bool BasicProtocol::land_drone(int protocol,int drone_id_sender, int drone_id_receiver){
    BPLogger->logFile << simTime() << ": land_drone command received" << endl;
    Enter_Method("land_drone");
    cMessage *msg = landDrone->dup();
    double x_c = controlModule->drone_data[drone_id_receiver-1]->Current_Position[0];
    double y_c = controlModule->drone_data[drone_id_receiver-1]->Current_Position[1];
    msg->addPar("x") = x_c;
    msg->addPar("y") = y_c;
    double z_val =0;
    height_checker(x_c, y_c, z_val);
    msg->addPar("z") = z_val;
    if(protocol == 1){
        send(msg, "dronesSocket$o",drone_id_receiver-1);
        BPLogger->logFile << simTime() << ": Landing sequence sent to drone with ID: "<< drone_id_receiver << " via cMessages." << endl;
        return true;
    }
    else if (protocol == 2){//UDP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending UDP message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("udpPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"udpRequestIn");
        BPLogger->logFile << simTime() << ": Landing sequence sent from drone with ID: " << drone_id_sender <<
                                " to drone with ID: "<< drone_id_receiver << " via UDP." << endl;
        return true;
    }
    else{//TCP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending TCP message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("connectPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"tcpRequestIn");
        BPLogger->logFile << simTime() << ": Landing sequence sent from drone with ID: " << drone_id_sender <<
                                        " to drone with ID: "<< drone_id_receiver << " via TCP." << endl;
        return true;
    }
    BPLogger->logFile << simTime() << ": land_drone command: message passing protocol unrecognized, received: " << protocol << endl;
    return false;
}

bool BasicProtocol::move_to(int protocol,int drone_id_sender, int drone_id_receiver,double x_dest, double y_dest, double z_dest){
    BPLogger->logFile << simTime() << ": move_to command received" << endl;
    Enter_Method("move_to");
    cMessage *msg = moveTo->dup();
    msg->addPar("x") = x_dest;
    msg->addPar("y") = y_dest;
    msg->addPar("z") = z_dest;
    if(protocol == 1){
        send(msg, "dronesSocket$o",drone_id_receiver-1);
        BPLogger->logFile << simTime() << ": Movement command sent to drone with ID: "<< drone_id_receiver << " via cMessages." << endl;
        return true;
    }
    else if (protocol == 2){//UDP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending UDP message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("udpPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"udpRequestIn");
        BPLogger->logFile << simTime() << ": Movement command sent from drone with ID: " << drone_id_sender <<
                                        " to drone with ID: "<< drone_id_receiver << " via UDP." << endl;
        return true;
    }
    else{//TCP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending TCP message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("connectPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"tcpRequestIn");
        BPLogger->logFile << simTime() << ": Movement command sent from drone with ID: " << drone_id_sender <<
                                        " to drone with ID: "<< drone_id_receiver << " via TCP." << endl;
        return true;
    }
    BPLogger->logFile << simTime() << ": move_to command: message passing protocol unrecognized, received: " << protocol << endl;
    return false;
}
bool BasicProtocol::hover(){return true;}//not in use in current simulator version
bool BasicProtocol::rotate() {return true;}//not in use in current simulator version
bool BasicProtocol::set_velocity(int protocol,int drone_id_sender, int drone_id_receiver,double x_vel, double y_vel, double z_vel){
    BPLogger->logFile << simTime() << ": set_velocity command received at:" << endl;
    Enter_Method("set_velocity");
    cMessage *msg = setVelocity->dup();
    msg->addPar("x_vel") = x_vel;
    msg->addPar("y_vel") = y_vel;
    msg->addPar("z_vel") = z_vel;
    if(protocol == 1){
        send(msg, "dronesSocket$o",drone_id_receiver-1);
        BPLogger->logFile << simTime() << ": Velocity values sent to drone with ID: "<< drone_id_receiver << " via cMessages." << endl;
        return true;
    }
    else if (protocol == 2){//UDP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending UDP message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("udpPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"udpRequestIn");
        BPLogger->logFile << simTime() << ": Velocity values sent from drone with ID: " << drone_id_sender <<
                                                " to drone with ID: "<< drone_id_receiver << " via UDP." << endl;
        return true;
    }
    else{//TCP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending TCP message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("connectPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"tcpRequestIn");
        BPLogger->logFile << simTime() << ": Velocity values sent from drone with ID: " << drone_id_sender <<
                                        " to drone with ID: "<< drone_id_receiver << " via TCP." << endl;
        return true;
    }
    BPLogger->logFile << simTime() << ": set_velocity command: message passing protocol unrecognized, received: " << protocol << endl;
    return false;
}
bool BasicProtocol::set_acceleration(int protocol,int drone_id_sender, int drone_id_receiver,double acceleration){
    BPLogger->logFile << simTime() << ": set_acceleration command received" << endl;
    Enter_Method("set_acceleration");
    cMessage *msg = setAcceleration->dup();
    msg->addPar("acceleration") = acceleration;
    if(protocol == 1){
        send(msg, "dronesSocket$o",drone_id_receiver-1);
        BPLogger->logFile << simTime() << ": Acceleration value sent to drone with ID: "<< drone_id_receiver << " via cMessages." << endl;
        return true;
    }
    else if (protocol == 2){//UDP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending UDO message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("udpPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"udpRequestIn");
        BPLogger->logFile << simTime() << ": Acceleration value sent from drone with ID: " << drone_id_sender <<
                                                " to drone with ID: "<< drone_id_receiver << " via UDP." << endl;
        return true;
    }
    else{//TCP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending TCP message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("connectPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"tcpRequestIn");
        BPLogger->logFile << simTime() << ": Acceleration value sent from drone with ID: " << drone_id_sender <<
                                        " to drone with ID: "<< drone_id_receiver << " via TCP." << endl;
        return true;
    }
    BPLogger->logFile << simTime() << ": set_acceleration command: message passing protocol unrecognized, received: " << protocol << endl;
    return false;
}
bool BasicProtocol::return_to_base(int protocol,int drone_id_sender, int drone_id_receiver){
    BPLogger->logFile << simTime() << ": return_to_base command received" << endl;
    Enter_Method("return_to_base");
    cMessage *msg = retBase->dup();
    if(protocol == 1){
        send(msg, "dronesSocket$o",drone_id_receiver-1);
        BPLogger->logFile << simTime() << ": Return to base command sent to drone with ID: "<< drone_id_receiver << " via cMessages." << endl;
        return true;
    }
    else if (protocol == 2){//UDP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending UDP message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("udpPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"udpRequestIn");
        BPLogger->logFile << simTime() << ": Return to base command sent from drone with ID: " << drone_id_sender <<
                                        " to drone with ID: "<< drone_id_receiver << " via UDP." << endl;
        return true;
    }
    else{//TCP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending TCP message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("connectPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"tcpRequestIn");
        BPLogger->logFile << simTime() << ": Return to base command sent from drone with ID: " << drone_id_sender <<
                                        " to drone with ID: "<< drone_id_receiver << " via TCP." << endl;
        return true;
    }
    BPLogger->logFile << simTime() << ": return_to_base command: message passing protocol unrecognized, received: " << protocol << endl;
    return false;
}

//State management cMessages
bool BasicProtocol::stop_drone(int protocol,int drone_id_sender, int drone_id_receiver){
    BPLogger->logFile << simTime() << ": stop_drone command received"<< endl;
    Enter_Method("stop_drone");
    cMessage *msg = stopDrone->dup();
    if(protocol == 1){
        send(msg, "dronesSocket$o",drone_id_receiver-1);
        BPLogger->logFile << simTime() << ": Stop command sent to drone with ID: "<< drone_id_receiver << " via cMessages." << endl;
        return true;
    }
    else if (protocol == 2){//UDP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending UDO message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("udpPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"udpRequestIn");
        BPLogger->logFile << simTime() << ": Stop command sent from drone with ID: " << drone_id_sender <<
                                                " to drone with ID: "<< drone_id_receiver << " via UDP." << endl;
        return true;
    }
    else{//TCP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending TCP message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("connectPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"tcpRequestIn");
        BPLogger->logFile << simTime() << ": Stop command sent from drone with ID: " << drone_id_sender <<
                                        " to drone with ID: "<< drone_id_receiver << " via TCP." << endl;
        return true;
    }
    BPLogger->logFile << simTime() << ": stop_drone command: message passing protocol unrecognized, received: " << protocol << endl;
    return false;
}
bool BasicProtocol::calibrate_drone(){return true;}//not in use in current simulator version
bool BasicProtocol::setMode_drone(){return true;}//not in use in current simulator version
bool BasicProtocol::powerOn_drone(int drone_id,double x_pos, double y_pos, double z_pos){
    BPLogger->logFile << simTime() << ": powerOn_drone command received" << endl;
    Enter_Method("powerOn_drone");
    cMessage *msg = powerOnDrone->dup();
    msg->addPar("x") = x_pos;
    msg->addPar("y") = y_pos;
    controlModule->height_checker(x_pos, y_pos,heightVal);
    z_pos += heightVal;
    msg->addPar("z") = z_pos;
    send(msg, "dronesSocket$o",drone_id-1);
    BPLogger->logFile << "Power on drone with ID: "<< drone_id << " via cMessages." << endl;
    return true;
}
bool BasicProtocol::powerOff_drone(int protocol,int drone_id_sender, int drone_id_receiver){
    BPLogger->logFile << simTime() << ": powerOff_drone command received" << endl;
    Enter_Method("powerOff_drone");
    cMessage *msg = powerOffDrone->dup();
    //delete(powerOffDrone);
    if(protocol == 1){
        send(msg, "dronesSocket$o",drone_id_receiver-1);
        BPLogger->logFile << simTime() << ": Power off drone with ID: "<< drone_id_receiver << "via cMessages." << endl;
        return true;
    }
    else if (protocol == 2){//UDP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending UDO message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("udpPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"udpRequestIn");
        BPLogger->logFile << simTime() << ": Power off drone command sent from drone with ID: " << drone_id_sender <<
                                                " to drone with ID: "<< drone_id_receiver << "via UDP." << endl;
        return true;
    }
    else{//TCP
        std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
        L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
        BPLogger->logFile << simTime() << ": Request for sending TCP message received. From drone: " << std::to_string(drone_id_sender)
                << " to destination L3Adress: " << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
        msg->addPar("targetAddress").setStringValue(destAddr.str().c_str());
        connectPort = par("connectPort");
        msg->addPar("targetPort") = connectPort;
        sendDirect(msg, controlModule->drone_data[drone_id_sender-1],"tcpRequestIn");
        BPLogger->logFile << simTime() << ": Power off drone command sent from drone with ID: " << drone_id_sender <<
                                        " to drone with ID: "<< drone_id_receiver << "via TCP." << endl;
        return true;
    }
    BPLogger->logFile << simTime() << ": powerOff_drone command: message passing protocol unrecognized, received: " << protocol << endl;
    return false;
}

//Sensor, data and monitoring
bool BasicProtocol::getDrone_navData(int drone_id, double *nav_array){
    nav_array[0]=controlModule->drone_data[drone_id-1]->Current_Position[0];
    nav_array[1]=controlModule->drone_data[drone_id-1]->Current_Position[1];
    nav_array[2]=controlModule->drone_data[drone_id-1]->Current_Position[2];
    nav_array[3]=controlModule->drone_data[drone_id-1]->acceleration;
    nav_array[4]=controlModule->drone_data[drone_id-1]->x_velocity;
    nav_array[5]=controlModule->drone_data[drone_id-1]->y_velocity;
    nav_array[6]=controlModule->drone_data[drone_id-1]->z_velocity;
    nav_array[7]=controlModule->drone_data[drone_id-1]->Destination[0];
    nav_array[8]=controlModule->drone_data[drone_id-1]->Destination[1];
    nav_array[9]=controlModule->drone_data[drone_id-1]->Destination[2];
    nav_array[10]=controlModule->drone_data[drone_id-1]->battery_remain;
    nav_array[11]=controlModule->drone_data[drone_id-1]->ChargeStationCoord[0];
    nav_array[12]=controlModule->drone_data[drone_id-1]->ChargeStationCoord[1];
    nav_array[13]=controlModule->drone_data[drone_id-1]->ChargeStationCoord[2];
    return true;
}
bool BasicProtocol::getDrone_status(int drone_id,double *stat_array){
    stat_array[0]=controlModule->drone_data[drone_id-1]->power_on;
    stat_array[1]=controlModule->drone_data[drone_id-1]->non_operational;
    return true;
}
bool BasicProtocol::getDrone_position(int protocol,int drone_id, double *pos_array){
    if (drone_id<=controlModule->drone_data.size()){
        pos_array[0]=controlModule->drone_data[drone_id-1]->Current_Position[0];
        pos_array[1]=controlModule->drone_data[drone_id-1]->Current_Position[1];
        pos_array[2]=controlModule->drone_data[drone_id-1]->Current_Position[2];
        return true;
    }
    else{
        return false;
    }
}
bool BasicProtocol::getDrone_altitude(int protocol,int drone_id, double &altitude){
    altitude = controlModule->drone_data[drone_id-1]->Current_Position[2];
    return true;
}
bool BasicProtocol::getDrone_batteryStatus(int protocol,int drone_id, double &batteryStatus){
    batteryStatus = controlModule->drone_data[drone_id-1]->battery_remain;
    return true;
}
bool BasicProtocol::getDrone_SensorData(){return true;}//not in use in current simulator version

void BasicProtocol::height_checker(double x_pos, double y_pos, double &z_val){
    std::string modulePath = "DroneNetwork.simControl";
    // Get the module using the path
    cModule *host = getModuleByPath(modulePath.c_str());
    SimulationControl *SC = check_and_cast<SimulationControl *>(host);
    SC->height_checker(x_pos, y_pos,z_val);
}

bool BasicProtocol::moveStation(int id, int newX, int newY, int newZ){
    controlModule->ChargStationManager->moveStation(id, newX, newY, newZ);
    return true;
}

bool BasicProtocol::getStationLocation(int id, int &x, int &y, int &z){
    controlModule->ChargStationManager->getStationLocation(id, x, y, z);
    return true;
}
