#include <omnetpp.h>
#include <iostream>

using namespace omnetpp;

//weight - total weight of drone [kg]
//num_rotors - number of rotors
//motor_efficiency - efficiency of motors [W/N]
//E_hover = P_hover * t[sec]

// Function to calculate power needed for hovering
inline double calculateHoveringPower(float weight, float motor_efficiency) {
    float gravity = 9.81; //[m/sec^2]
	float P_hover = (weight * gravity) / motor_efficiency;
    return P_hover;
}

// Function to calculate total current consumption
// Will be used after any movement command or sending message
inline double calculateTotalCurrent(float weight, double acceleration, double velocity) {
	double accelerationEnergy =0;
	double velocityEnergy =0;
	if (acceleration != 0){ //will be done once when move command received
		accelerationEnergy = 0.5 * weight * std::pow(acceleration, 2) * (velocity/acceleration);
	}
	if (velocity != 0) {//will be done once when move command received
		double air_resistance_factor = 0.05; // Adjusted based on aerodynamics
		double drag_power = air_resistance_factor * std::pow(velocity, 2);
		velocityEnergy = drag_power * (velocity/acceleration); // Energy in Joules
	}
	double totalCurrentConsumption = accelerationEnergy + velocityEnergy;
    return totalCurrentConsumption;
}

// Function to update remaining battery capacity
// sensorPower + hoveringPower  - will be calculated periodically for each drone
// user algorithm will use it when it has additional power needs
// remainingCapacity - Remaining battery capacity in Joules (J)
inline double updateBatteryCapacity(double &remainingCapacity, double currentConsumption, bool flag, 
									double sensorPower, double additionalPower, double hoveringPower, int time_step) {
	if (flag == true){// need to be calculated periodically for each drone
		remainingCapacity = remainingCapacity - (sensorPower + hoveringPower + additionalPower)*time_step;
	}
	else { //Will be used after any movement command or sending message
		remainingCapacity -= currentConsumption;
	}
    return remainingCapacity;
}
