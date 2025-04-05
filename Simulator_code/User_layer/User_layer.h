/*
 * User_layer.h
 *
 *  Created on: Feb 20, 2025
 *      Author: shaked
 */

#ifndef USER_LAYER_USER_LAYER_H_
#define USER_LAYER_USER_LAYER_H_

#include <omnetpp.h>
#include "../BasicProtocol_layer/BasicProtocol.h"

using namespace std;
using namespace omnetpp;

class UserLayer : public cSimpleModule {
    private:
      BasicProtocol *basicProtocolModule;
      bool relayFlag;
      double pos_array_1[3] = {0,0,0}; //  double [x_position, y_position, z_position]
      double pos_array_3[3] = {0,0,0}; //  double [x_position, y_position, z_position]
      cMessage *simMsg = new cMessage("simMsg"); // create a Simulation self cMessage

    protected:
      virtual void initialize(); // Initializes the drone module
      virtual void handleMessage(cMessage *msg); // The algorithm
};
Define_Module(UserLayer);
#endif /* USER_LAYER_USER_LAYER_H_ */



