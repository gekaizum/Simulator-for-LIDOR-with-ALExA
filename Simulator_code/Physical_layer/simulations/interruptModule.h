/*
 * interruptModule.h
 *
 *  Created on: May 14, 2025
 *      Author: gekaizum
 */

#ifndef PHYSICAL_LAYER_SIMULATIONS_INTERRUPTMODULE_H_
#define PHYSICAL_LAYER_SIMULATIONS_INTERRUPTMODULE_H_

#include <omnetpp.h>

using namespace omnetpp;

class interruptModule : public cSimpleModule {
  private:
    bool chooseOpt;
    cMessage *interruptSc;
    cMessage *interruptSc2;
    cMessage *deleteMe;
  protected:
    virtual void initialize();                               // Initializes the charging station module
    virtual void handleMessage(cMessage *msg);               // Handles incoming messages
    virtual void finish();
  public:
};
Define_Module(interruptModule);



#endif /* PHYSICAL_LAYER_SIMULATIONS_INTERRUPTMODULE_H_ */
