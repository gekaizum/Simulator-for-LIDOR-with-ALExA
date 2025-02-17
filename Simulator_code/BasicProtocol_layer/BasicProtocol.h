/*
 * BasicProtocol.h
 *
 *  Created on: Feb 16, 2025
 *      Author: gekaizum
 */

#ifndef BASICPROTOCOL_LAYER_BASICPROTOCOL_H_
#define BASICPROTOCOL_LAYER_BASICPROTOCOL_H_

#include <omnetpp.h>
//#include "../Physical_layer/Drone_block/DroneControl.h"
//#include "../Physical_layer/Map_block/ChargingStationManager.h"
#include "../Physical_layer/simulations/SimulationControl.h"
#include "BasicProtocolLogger.h"

using namespace omnetpp;

class BasicProtocol : public cSimpleModule {
  private:
    BasicProtocolLogger* BPLogger = nullptr;
    SimulationControl *controlModule;
    //movement cMessages
    cMessage *setBase;
    cMessage *takeOff;
    cMessage *landDrone;
    cMessage *moveTo;
    cMessage *setVelocity;
    cMessage *setAcceleration;
    //State management cMessages
    cMessage *stopDrone;
    cMessage *powerOnDrone;
    cMessage *powerOffDrone;
    //Sensor, data and monitoring cMessages
    cMessage *getStatusDrone;
    cMessage *getPositionDrone;
    cMessage *getAltitudeDrone;
    cMessage *getBatteryDrone;
    void message_init();
  protected:
    virtual void initialize(); // Initializes the drone module
    virtual void handleMessage(cMessage *msg); // Handles incoming messages
    virtual void finish();
  public:
    // int protocol:
    /*
     *    1. None - command from user algorithm, will be done using cMessages
     *    2. UDP - command from manual user, will be sent from base station (not in use in current version)
     *    3. TCP - command from manual user, will be sent from base station (not in use in current version)
    */
    /////////////////////////////
    //movement commands
    /////////////////////////////
    /**/
    //Updates the base station coordinates of chosen drone
    bool set_base(int protocol,int drone_id,double x_base, double y_base, double z_base);
    /**/
    //Drone take off sequence. You need to set up velocity and acceleration before or default values will be used
    bool take_off(int protocol,int drone_id,double x_dest, double y_dest, double z_dest);
    /**/
    // Landing on ground - movement on z-direction, after landing TAKEOFF sequence need to bee implemented again
    bool land_drone(int protocol,int drone_id);
    /**/
    //Drone movement command. You need to set up velocity and acceleration before or default values will be used
    bool move_to(int protocol,int drone_id,double x_dest, double y_dest, double z_dest);
    /**/
    //not in use in current simulator version
    bool hover();
    /**/
    //not in use in current simulator version
    bool rotate();
    /**/
    //Set velocity parameters for next move of drone
    bool set_velocity(int protocol,int drone_id,double x_vel, double y_vel, double z_vel);
    /**/
    //Set acceleration parameters for next move of drone
    bool set_acceleration(int protocol,int drone_id,double acceleration);
    /**/
    //not in use in current simulator version
    bool return_to_base();//not in use in current simulator version
    /**/
    /////////////////////////////
    //State management commands
    /////////////////////////////
    /**/
    // Stops any drone movements, drone continue hovering
    bool stop_drone(int protocol,int drone_id);
    /**/
    //not in use in current simulator version
    bool calibrate_drone();
    /**/
    //not in use in current simulator version
    bool setMode_drone();
    /**/
    //Activates drone, drone becomes active access point on ground
    bool powerOn_drone(int drone_id, double x_pos, double y_pos, double z_pos);
    /**/
    //Deactivates drone, cannot respond to any command except power on
    bool powerOff_drone(int protocol,int drone_id);
    /**/
    /////////////////////////////
    //Sensor, data and monitoring
    /////////////////////////////
    /**/
    //Saves navigation data array at nav_array: double [x_position, y_position, z_position, acceleration,
    //                                          x_velocity, y_velocity, z_velocity,
    //                                          x_destination, y_destination, z_destination, battery_ramain[mAh]
    //                                          x_chargStation, y_chargStation, z_chargStation]
    bool getDrone_navData(int drone_id, double *nav_array);
    /**/
    //Saves status array at stat_array: bool [power_on, non_operational]
    bool getDrone_status(int drone_id, double *stat_array);
    //Saves position array at pos_array: double [x_position, y_position, z_position]
    bool getDrone_position(int protocol,int drone_id, double *pos_array);
    /**/
    //Saves altitude at altitude
    bool getDrone_altitude(int protocol,int drone_id, double &altitude);
    /**/
    //Saves battery remain capacity at batteryStatus
    bool getDrone_batteryStatus(int protocol,int drone_id, double &batteryStatus);
    /**/
    //not in use in current simulator version
    bool getDrone_SensorData();
    /**/
};

Define_Module(BasicProtocol);

#endif /* BASICPROTOCOL_LAYER_BASICPROTOCOL_H_ */
