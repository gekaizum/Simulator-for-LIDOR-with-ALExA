/*
 * BasicProtocolLogger.h
 *
 *  Created on: Feb 17, 2025
 *      Author: gekaizum
 */

#ifndef BASICPROTOCOL_LAYER_BASICPROTOCOLLOGGER_H_
#define BASICPROTOCOL_LAYER_BASICPROTOCOLLOGGER_H_

#include <omnetpp.h>
#include <fstream>

class BasicProtocolLogger {
    private:
    protected:
    public:
        std::ofstream logFile;
        BasicProtocolLogger();
        ~BasicProtocolLogger();
};



#endif /* BASICPROTOCOL_LAYER_BASICPROTOCOLLOGGER_H_ */
