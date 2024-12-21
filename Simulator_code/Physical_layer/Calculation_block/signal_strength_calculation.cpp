#include <omnetpp.h>
#include <cmath>
#include <vector>

using namespace omnetpp;

/**
 * The SignalStrengthCalculation class computes signal strength based on various propagation models,
 * including Free-Space Path Loss, Two-Ray Ground Reflection, and Knife-Edge Diffraction. It also
 * accounts for terrain data to calculate effective obstacle heights for the Knife-Edge model.
 */
class SignalStrengthCalculation : public cSimpleModule {
  private:
    double calculateFreeSpacePathLoss(double transmittedPower, double gainTransmitter, double gainReceiver, double distance, double frequency);
    double calculateTwoRayGroundReflection(double transmittedPower, double gainTransmitter, double gainReceiver, double h_Transmitter, double h_Receiver, double distance);
    double calculateKnifeEdgeDiffraction(double transmittedPower, double wavelength, double t_to_obst, double r_to_obst, double h_Eff);
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(SignalStrengthCalculation);

void SignalStrengthCalculation::initialize() {
    // Initialization logic if required
}

void SignalStrengthCalculation::handleMessage(cMessage *msg) {
    // Extract parameters from the message or module
    std::string propagationModel = par("propagationModel");
    std::string communicationType = par("communicationType");
    double transmittedPower = par("transmittedPower"); // [W]
    double gainTransmitter = par("gainTransmitter");   // [linear scale]
    double gainReceiver = par("gainReceiver");         // [linear scale]
    double distance = par("distance");                // [m]
    double frequency = par("frequency");              // [Hz]
    double h_Transmitter = par("h_Transmitter"); // [m]
    double h_Receiver = par("h_Receiver");       // [m]
    double t_to_obst = par("t_to_obst"); // Distance to obstacle from transmitter [m]
    double r_to_obst = par("r_to_obst"); // Distance to obstacle from receiver [m]
    double h_obstacle = par("h_obstacle"); // max height [m] in the map along the line of sight

/*
    // Coordinates for terrain-based height calculation
    double transmitterX = par("transmitterX");
    double transmitterY = par("transmitterY");
    double receiverX = par("receiverX");
    double receiverY = par("receiverY");
*/    


    double h_Eff = calculateEffectiveHeight(t_to_obst, r_to_obst, h_Transmitter, h_Receiver, h_obstacle);

    double signalStrength = 0.0;

    if (propagationModel == "FreeSpace") {
        signalStrength = calculateFreeSpacePathLoss(transmittedPower, gainTransmitter, gainReceiver, distance, frequency);
    } else if (propagationModel == "TwoRay") {
        signalStrength = calculateTwoRayGroundReflection(transmittedPower, gainTransmitter, gainReceiver, h_Transmitter, h_Receiver, distance);
    } else if (propagationModel == "KnifeEdge") {
        double wavelength = 3e8 / frequency; // Speed of light / frequency
        signalStrength = calculateKnifeEdgeDiffraction(transmittedPower, wavelength, t_to_obst, r_to_obst, h_Eff);
    }

    EV << "Calculated Signal Strength: " << signalStrength << " W" << endl;

    delete msg;
}

double SignalStrengthCalculation::calculateFreeSpacePathLoss(double transmittedPower, double gainTransmitter, double gainReceiver, double distance, double frequency) {
    double c = 3e8; // Speed of light [m/s]
    double p_r = transmittedPower + gainTransmitter + gainReceiver -20*log10(distance) - 20*log10(frequency) - 20*log10(4*M_PI/c);
    return p_r;
}

double SignalStrengthCalculation::calculateTwoRayGroundReflection(double transmittedPower, double gainTransmitter, double gainReceiver, double h_Transmitter, double h_Receiver, double distance) {
    double p_r = transmittedPower - 40*log10(distance) + 10*log10(gainTransmitter*gainReceiver*pow(h_Receiver*h_Transmitter,2));
    return p_r;
}

double SignalStrengthCalculation::calculateKnifeEdgeDiffraction(double transmittedPower, double wavelength, double t_to_obst, double r_to_obst, double h_Eff) {
    double v = (h_Eff * sqrt(2 * (t_to_obst + r_to_obst) / (wavelength * t_to_obst * r_to_obst)));
    double p_r = transmittedPower - 6.9 -20 * log10(sqrt(pow((v - 0.1),2) + 1)) + v - 0.1;
    return p_r;
}

double SignalStrengthCalculation::calculateEffectiveHeight(double t_to_obst, double r_to_obst, double h_Transmitter, double h_Receiver, double h_obstacle) {
    double h_Eff = h_obstacle - (h_Transmitter + t_to_obst * (h_Receiver - h_Transmitter) / (t_to_obst + r_to_obst));
    return h_Eff;
}
