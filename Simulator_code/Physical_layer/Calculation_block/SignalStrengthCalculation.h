/*
 * SignalStrengthCalculation.h
 *
 *  Created on: Jan 27, 2025
 *      Author: gekaizum
 */

#ifndef PHYSICAL_LAYER_CALCULATION_BLOCK_SIGNALSTRENGTHCALCULATION_H_
#define PHYSICAL_LAYER_CALCULATION_BLOCK_SIGNALSTRENGTHCALCULATION_H_

#include <omnetpp.h>
#include <cmath>
#include <iostream>

const double SPEED_OF_LIGHT = 3e8;  // Speed of light in m/s
const double PI = 3.141592653589793;

// Enum for selecting the propagation model
enum class PropagationModel {
    FREE_SPACE,
    TWO_RAY,
    KNIFE_EDGE
};

double receivedPower(double transmittedPower, double gainTx, double gainRx, double pathLoss);
double calculateFreeSpacePathLoss(double frequencyHz, double distanceMeters);
double calculateTwoRayGroundReflection(double txHeight, double rxHeight, double distance, double frequencyHz);
double calculateKnifeEdgeDiffraction(double obstacleHeight, double t_to_obst, double r_to_obst, double frequencyHz);
double calculateSignalLoss(PropagationModel model, double frequencyHz, double distance,
                           double txHeight = 0, double rxHeight = 0,
                           double obstacleHeight = 0, double t_to_obst = 0, double r_to_obst = 0);

#endif /* PHYSICAL_LAYER_CALCULATION_BLOCK_SIGNALSTRENGTHCALCULATION_H_ */
