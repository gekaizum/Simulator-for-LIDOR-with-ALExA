#ifndef PHYSICAL_LAYER_CALCULATION_BLOCK_BATTERYCONSUMPTION_H_
#define PHYSICAL_LAYER_CALCULATION_BLOCK_BATTERYCONSUMPTION_H_

#include <omnetpp.h>
#include <iostream>

double calculateHoveringPower(double weight, double motor_efficiency);

double calculateTotalCurrent(double weight, double acceleration, double velocity, double distance);

double updateBatteryCapacity(double &remainingCapacity, double currentConsumption, bool flag,
                                    double sensorPower, double additionalPower, double hoveringPower, int time_step);

#endif /* PHYSICAL_LAYER_CALCULATION_BLOCK_BATTERYCONSUMPTION_H_ */
