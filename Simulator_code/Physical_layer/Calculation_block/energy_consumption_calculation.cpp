#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

class DroneEnergyCalculator {
private:
    double batteryCapacity;       // Battery capacity in mAh
    double hoverCurrent;          // Hovering current in A
    double accelerationCurrent;   // Acceleration current in A
    double antennaCurrent;        // Antenna current in A
    double sensorsCurrent;        // Sensors current in A
    double hoverTime;             // Hovering time in seconds
    double accelerationTime;      // Acceleration time in seconds

public:
    DroneEnergyCalculator(double batteryCapacity, double hoverCurrent, double accelerationCurrent,
                          double antennaCurrent, double sensorsCurrent, double hoverTime, double accelerationTime)
        : batteryCapacity(batteryCapacity), hoverCurrent(hoverCurrent), accelerationCurrent(accelerationCurrent),
          antennaCurrent(antennaCurrent), sensorsCurrent(sensorsCurrent), hoverTime(hoverTime), accelerationTime(accelerationTime) {}

    // Calculate total current
    double calculateTotalCurrent() {
        double totalHovering = hoverCurrent * hoverTime;
        double totalAcceleration = accelerationCurrent * accelerationTime;
        double totalTime = hoverTime + accelerationTime;

        double totalCurrent = (totalHovering + totalAcceleration) / totalTime + antennaCurrent + sensorsCurrent;
        return totalCurrent;
    }

    void calculateBatteryStatus() {
        double totalCurrent = calculateTotalCurrent();

        // Convert time to hours
        double totalTimeInHours = (hoverTime + accelerationTime) / 3600.0;
        double batteryConsumption = totalCurrent * totalTimeInHours * 1000.0; // Convert to mAh
        double remainingCapacity = batteryCapacity - batteryConsumption;
        double remainingPercentage = (remainingCapacity / batteryCapacity) * 100;

        // Display results
        cout << "Remaining Battery Capacity: " << fixed << setprecision(2) << remainingCapacity << " mAh\n";
        cout << "Remaining Battery Percentage: " << fixed << setprecision(2) << remainingPercentage << " %\n";
    }
};

int main() {
    // Realistic parameters for DJI EVO
    double batteryCapacity = 7100.0;  // Battery capacity in mAh
    double hoverCurrent = 5.0;        // Hovering current in A
    double accelerationCurrent = 7.0; // Acceleration current in A
    double antennaCurrent = 0.3;      // Antenna current in A
    double sensorsCurrent = 0.2;      // Sensors current in A
    double hoverTime = 3000.0;        // Hovering time in seconds
    double accelerationTime = 360.0;  // Acceleration time in seconds

    DroneEnergyCalculator calculator(batteryCapacity, hoverCurrent, accelerationCurrent, antennaCurrent, sensorsCurrent, hoverTime, accelerationTime);

    // Calculate and display battery status
    calculator.calculateBatteryStatus();

    return 0;
}
