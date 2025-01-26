#include <omnetpp.h>
#include <cmath>
#include <iostream>
//#include <vector>
//#include <string>
//#include <sstream>

using namespace omnetpp;

/*
 * The SignalStrengthCalculation class computes signal strength based on various propagation models,
 * including Free-Space Path Loss, Two-Ray Ground Reflection, and Knife-Edge Diffraction. It also
 * accounts for terrain data to calculate effective obstacle heights for the Knife-Edge model.
 */
 
 
 // First you need to calculate signal loss using calculateSignalLoss()
 // Then received power in dBm using receivedPower()
 
const double SPEED_OF_LIGHT = 3e8;  // Speed of light in m/s
const double PI = 3.141592653589793;

// Enum for selecting the propagation model
enum class PropagationModel {
    FREE_SPACE,
    TWO_RAY,
    KNIFE_EDGE
};

// Function to compute received power in dBm
double receivedPower(double transmittedPower, double gainTx, double gainRx, double pathLoss) {
    return transmittedPower + gainTx + gainRx - pathLoss;
}

// Function to calculate Free-Space Path Loss (FSPL)
double calculateFreeSpacePathLoss(double frequencyHz, double distanceMeters) {
    if (distanceMeters <= 0) return 0;
    double wavelength = SPEED_OF_LIGHT / frequencyHz;
    return 20 * log10(distanceMeters) + 20 * log10(frequencyHz) - 147.55;  // dB
}

// Function to calculate Two-Ray Ground Reflection
double calculateTwoRayGroundReflection(double txHeight, double rxHeight, double distance, double frequencyHz) {
    if (distance <= 0) return 0;
    double directPathLoss = freeSpacePathLoss(frequencyHz, distance);
    double groundReflectionLoss = 10 * log10((txHeight * rxHeight) / (distance * distance));
    return directPathLoss - groundReflectionLoss;
}

// Function to calculate Knife-Edge Diffraction
double calculateKnifeEdgeDiffraction(double obstacleHeight, double t_to_obst, double r_to_obst, double frequencyHz) {
	double wavelength = SPEED_OF_LIGHT/frequencyHz;
	// Calculate the diffraction parameter v
    double v = (sqrt(2) * obstacleHeight) / sqrt((wavelength * (t_to_obst + r_to_obst)) / (t_to_obst * r_to_obst));
    if (v <= -0.78) return 0;   // If v <= -0.78, there is no significant attenuation
	// Compute the diffraction loss (in dB)
    double diffractionLoss = 6.9 + 20 * log10(sqrt((v - 0.1) * (v - 0.1) + 1) + v - 0.1);
    return diffractionLoss;
}

// Unified function to compute signal loss based on the selected model
double calculateSignalLoss(PropagationModel model, double frequencyHz, double distance, 
                           double txHeight = 0, double rxHeight = 0, 
                           double obstacleHeight = 0, double t_to_obst = 0, double r_to_obst = 0) {
    double pathLoss = 0;

    switch (model) {
        case PropagationModel::FREE_SPACE:
            pathLoss = freeSpacePathLoss(frequencyHz, distance);
            break;
        case PropagationModel::TWO_RAY:
            pathLoss = twoRayGroundReflection(txHeight, rxHeight, distance, frequencyHz);
            break;
        case PropagationModel::KNIFE_EDGE:
            pathLoss = knifeEdgeDiffraction(obstacleHeight, t_to_obst, r_to_obst, frequencyHz);
            break;
        default:
            std::cerr << "Invalid propagation model selected!" << std::endl;
            return -1;
    }
    return pathLoss;
}
