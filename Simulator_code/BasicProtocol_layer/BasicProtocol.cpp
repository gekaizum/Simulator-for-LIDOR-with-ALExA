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
    //controlModule = check_and_cast<SimulationControl*>(getParentModule()->getSubmodule(name, index)
    BPLogger->logFile << "Basic protocol layer online" << endl;
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
    BPLogger->logFile << "Movement cMessages initialized" << endl;
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
    BPLogger->logFile << "State management cMessages initialized" << endl;
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
    BPLogger->logFile << "Sensor, data and monitoring cMessages initialized" << endl;
    /////////////////////////////////////////////////////////////////////
}
//movement commands
bool BasicProtocol::set_base(int protocol, int drone_id,double x_base, double y_base, double z_base){
    BPLogger->logFile << "set_base command received at:" << simTime() << endl;
    Enter_Method("set_base");
    if(protocol == 1){
        cMessage *msg = setBase->dup();
        msg->addPar("x") = x_base;
        msg->addPar("y") = y_base;
        msg->addPar("z") = z_base;
        send(msg, "dronesSocket$o",drone_id-1);
        BPLogger->logFile << "Base station coordinates sent to drone with ID: "<< drone_id << "via cMessages." << endl;
        return true;
    }
    else if (protocol == 2){//UDP
        BPLogger->logFile << "Base station coordinates sent to drone with ID: "<< drone_id << "via UDP." << endl;
        return true;
    }
    else{//TCP
        BPLogger->logFile << "Base station coordinates sent to drone with ID: "<< drone_id << "via TCP." << endl;
        return true;
    }
    BPLogger->logFile << "set_base command: message passing protocol unrecognized, received: " << protocol << endl;
    return false;
}
bool BasicProtocol::take_off(int protocol,int drone_id,double x_dest, double y_dest, double z_dest){
    BPLogger->logFile << "take_off command received at:" << simTime() << endl;
    Enter_Method("take_off");
    controlModule->height_checker(x_dest, y_dest,heightVal);
    z_dest += heightVal;
    if(protocol == 1){
        cMessage *msg = takeOff->dup();
        msg->addPar("x") = x_dest;
        msg->addPar("y") = y_dest;
        msg->addPar("z") = z_dest;
        send(msg, "dronesSocket$o",drone_id-1);
        BPLogger->logFile << "Take off sequence sent to drone with ID: "<< drone_id << "via cMessages." << endl;
        return true;
    }
    else if (protocol == 2){//UDP
        BPLogger->logFile << "Take off sequence sent to drone with ID: "<< drone_id << "via UDP." << endl;
        return true;
    }
    else{//TCP
        BPLogger->logFile << "Take off sequence sent to drone with ID: "<< drone_id << "via TCP." << endl;
        return true;
    }
    BPLogger->logFile << "take_off command: message passing protocol unrecognized, received: " << protocol << endl;
    return false;
}
bool BasicProtocol::land_drone(int protocol,int drone_id){
    BPLogger->logFile << "land_drone command received at:" << simTime() << endl;
    Enter_Method("land_drone");
    if(protocol == 1){
        cMessage *msg = landDrone->dup();
        send(msg, "dronesSocket$o",drone_id-1);
        BPLogger->logFile << "Landing sequence sent to drone with ID: "<< drone_id << "via cMessages." << endl;
        return true;
    }
    else if (protocol == 2){//UDP
        BPLogger->logFile << "Landing sequence sent to drone with ID: "<< drone_id << "via UDP." << endl;
        return true;
    }
    else{//TCP
        BPLogger->logFile << "Landing sequence sent to drone with ID: "<< drone_id << "via TCP." << endl;
        return true;
    }
    BPLogger->logFile << "land_drone command: message passing protocol unrecognized, received: " << protocol << endl;
    return false;
}
bool BasicProtocol::move_to(int protocol,int drone_id,double x_dest, double y_dest, double z_dest){
    BPLogger->logFile << "move_to command received at:" << simTime() << endl;
    Enter_Method("move_to");
    if(protocol == 1){
        cMessage *msg = moveTo->dup();
        msg->addPar("x") = x_dest;
        msg->addPar("y") = y_dest;
        msg->addPar("z") = z_dest;
        send(msg, "dronesSocket$o",drone_id-1);
        BPLogger->logFile << "Movement command sent to drone with ID: "<< drone_id << "via cMessages." << endl;
        return true;
    }
    else if (protocol == 2){//UDP
        BPLogger->logFile << "Movement command sent to drone with ID: "<< drone_id << "via UDP." << endl;
        return true;
    }
    else{//TCP
        BPLogger->logFile << "Movement command sent to drone with ID: "<< drone_id << "via TCP." << endl;
        return true;
    }
    BPLogger->logFile << "move_to command: message passing protocol unrecognized, received: " << protocol << endl;
    return false;
}
bool BasicProtocol::hover(){return true;}//not in use in current simulator version
bool BasicProtocol::rotate() {return true;}//not in use in current simulator version
bool BasicProtocol::set_velocity(int protocol,int drone_id,double x_vel, double y_vel, double z_vel){
    BPLogger->logFile << "set_velocity command received at:" << simTime() << endl;
    Enter_Method("set_velocity");
    if(protocol == 1){
        cMessage *msg = setVelocity->dup();
        msg->addPar("x_vel") = x_vel;
        msg->addPar("y_vel") = y_vel;
        msg->addPar("z_vel") = z_vel;
        send(msg, "dronesSocket$o",drone_id-1);
        BPLogger->logFile << "Velocity values sent to drone with ID: "<< drone_id << "via cMessages." << endl;
        return true;
    }
    else if (protocol == 2){//UDP
        BPLogger->logFile << "Velocity values sent to drone with ID: "<< drone_id << "via UDP." << endl;
        return true;
    }
    else{//TCP
        BPLogger->logFile << "Velocity values sent to drone with ID: "<< drone_id << "via TCP." << endl;
        return true;
    }
    BPLogger->logFile << "set_velocity command: message passing protocol unrecognized, received: " << protocol << endl;
    return false;
}
bool BasicProtocol::set_acceleration(int protocol,int drone_id,double acceleration){
    BPLogger->logFile << "set_acceleration command received at:" << simTime() << endl;
    Enter_Method("set_acceleration");
    if(protocol == 1){
        cMessage *msg = setAcceleration->dup();
        msg->addPar("acceleration") = acceleration;
        send(msg, "dronesSocket$o",drone_id-1);
        BPLogger->logFile << "Acceleration value sent to drone with ID: "<< drone_id << "via cMessages." << endl;
        return true;
    }
    else if (protocol == 2){//UDP
        BPLogger->logFile << "Acceleration value sent to drone with ID: "<< drone_id << "via UDP." << endl;
        return true;
    }
    else{//TCP
        BPLogger->logFile << "Acceleration value sent to drone with ID: "<< drone_id << "via TCP." << endl;
        return true;
    }
    BPLogger->logFile << "set_acceleration command: message passing protocol unrecognized, received: " << protocol << endl;
    return false;
}
bool BasicProtocol::return_to_base(){return true;}//not in use in current simulator version

//State management cMessages
bool BasicProtocol::stop_drone(int protocol,int drone_id){
    BPLogger->logFile << "stop_drone command received at:" << simTime() << endl;
    Enter_Method("stop_drone");
    if(protocol == 1){
        cMessage *msg = stopDrone->dup();
        send(msg, "dronesSocket$o",drone_id-1);
        BPLogger->logFile << "Stop command sent to drone with ID: "<< drone_id << "via cMessages." << endl;
        return true;
    }
    else if (protocol == 2){//UDP
        BPLogger->logFile << "Stop command sent to drone with ID: "<< drone_id << "via UDP." << endl;
        return true;
    }
    else{//TCP
        BPLogger->logFile << "Stop command sent to drone with ID: "<< drone_id << "via TCP." << endl;
        return true;
    }
    BPLogger->logFile << "stop_drone command: message passing protocol unrecognized, received: " << protocol << endl;
    return false;
}
bool BasicProtocol::calibrate_drone(){return true;}//not in use in current simulator version
bool BasicProtocol::setMode_drone(){return true;}//not in use in current simulator version
bool BasicProtocol::powerOn_drone(int drone_id, double x_pos, double y_pos, double z_pos){
    BPLogger->logFile << "powerOn_drone command received at:" << simTime() << endl;
    Enter_Method("powerOn_drone");
    cMessage *msg = powerOnDrone->dup();
    msg->addPar("x") = x_pos;
    msg->addPar("y") = y_pos;
    controlModule->height_checker(x_pos, y_pos,heightVal);
    z_pos += heightVal;
    msg->addPar("z") = z_pos;

    send(msg, "dronesSocket$o",drone_id-1);
    BPLogger->logFile << "Power on drone with ID: "<< drone_id << "via cMessages." << endl;
    return true;
}
bool BasicProtocol::powerOff_drone(int protocol,int drone_id){
    BPLogger->logFile << "powerOff_drone command received at:" << simTime() << endl;
    Enter_Method("powerOff_drone");
    if(protocol == 1){
        cMessage *msg = powerOffDrone->dup();
        send(msg, "dronesSocket$o",drone_id-1);
        BPLogger->logFile << "Power off drone with ID: "<< drone_id << "via cMessages." << endl;
        return true;
    }
    else if (protocol == 2){//UDP
        BPLogger->logFile << "Power off drone with ID: "<< drone_id << "via UDP." << endl;
        return true;
    }
    else{//TCP
        BPLogger->logFile << "Power off drone with ID: "<< drone_id << "via TCP." << endl;
        return true;
    }
    BPLogger->logFile << "powerOff_drone command: message passing protocol unrecognized, received: " << protocol << endl;
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
bool BasicProtocol::getDrone_status(int drone_id, double *stat_array){
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

//Connection
bool BasicProtocol::sendMsgTCP(int drone_id_sender, int drone_id_receiver){
    //std::string targetAddress=msg->par("targetAddress").stringValue();
    //int targetPort = msg->par("targetPort").doubleValue();
    std::string dest_address = "drones[" + std::to_string(drone_id_receiver-1) + "]";
    L3Address destAddr = L3AddressResolver().resolve(dest_address.c_str());
    BPLogger->logFile << "L3Adress is:" << destAddr << " of drone " << std::to_string(drone_id_receiver) << endl;
    cMessage *tcpRequest = new cMessage("tcpConnect");
    //tcpRequest->addPar("targetAddress") = destAddr;
    tcpRequest->addPar("targetAddress").setStringValue(destAddr.str().c_str());
    tcpRequest->addPar("targetPort") = 1235;
    BPLogger->logFile << "L3Adress is:" << tcpRequest->par("targetAddress") << " of drone " << std::to_string(drone_id_receiver) << endl;
    //controlModule->drone_data[drone_id_sender-1]->socketTcp.connect(destAddr, 1000);
    //Packet *packet = new Packet("CustomPacket");
    //controlModule->drone_data[drone_id_sender-1]->socketTcp.send(packet);
    controlModule->drone_data[drone_id_sender-1]->handleSendTcp(tcpRequest);
    cMessage *tcpRequest2 = tcpRequest->dup();
    //controlModule->drone_data[2]->handleSendTcp(tcpRequest2);
    return true;
}
