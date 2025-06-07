/*
 * SimulationControlLogger.h
 *
 *  Created on: Feb 1, 2025
 *      Author: gekaizum
 */

#ifndef PHYSICAL_LAYER_SIMULATIONS_SIMULATIONCONTROLLOGGER_H_
#define PHYSICAL_LAYER_SIMULATIONS_SIMULATIONCONTROLLOGGER_H_

#include <omnetpp.h>
#include <fstream>

class SimulationControlLogger {
    private:
    protected:
    public:
        std::ofstream logFile;
        SimulationControlLogger();
        ~SimulationControlLogger();
};



#endif /* PHYSICAL_LAYER_SIMULATIONS_SIMULATIONCONTROLLOGGER_H_ */
