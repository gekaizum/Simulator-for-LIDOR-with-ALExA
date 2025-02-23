/*
 * User_layer.h
 *
 *  Created on: Feb 20, 2025
 *      Author: shaked
 */

#ifndef USER_LAYER_USER_LAYER_H_
#define USER_LAYER_USER_LAYER_H_

#include <omnetpp.h>
#include "../Physical_layer/simulations/SimulationControl.h"
#include "../BasicProtocol_layer/BasicProtocol.h"

using namespace std;
using namespace omnetpp;

class UserLayer : public cSimpleModule {
    private:

      //UserLayerLogger* UserLayer_Logger = nullptr;
      BasicProtocol *basicProtocolModule;
      SimulationControl *controlModule;
      bool Start_Flag = true;
      double pos_array[3] = {0,0,0}; //  double [x_position, y_position, z_position]
      double drone1_x = 0;
      double drone3_x = 0;
      double drone4_x = 0;

    protected:
      virtual void initialize(); // Initializes the drone module
      virtual void algorithm(cMessage *msg); // The algorithm
 //     virtual void finish();
    public:
};

#endif /* USER_LAYER_USER_LAYER_H_ */



