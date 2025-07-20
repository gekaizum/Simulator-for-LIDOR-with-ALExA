/*
 * HybridFadingModel.cpp
 *
 *  Created on: May 31, 2025
 *      Author: shaked
 */

#include "HybridFadingModel.h"

Define_Module(HybridFadingModel);


HybridFadingModel::HybridFadingModel() :
    k(1)
{
}

void HybridFadingModel::initialize(int stage)
{
        if (stage == INITSTAGE_LOCAL) {
            FreeSpacePathLoss::initialize(stage);
            k = math::dB2fraction(par("k"));
        }
        fileName = "Sim_logs/FadingModelLogFile.log";
        fadingLogFile.open(fileName, std::ios::out);  // Create/open log file
        if (!fadingLogFile.is_open()) {
            EV << "Error: Could not open log file!" << endl;
        } else {
            fadingLogFile << simTime() << ": Simulation start" << "\n";
            fadingLogFile.flush();
        }
        if (stage == numInitStages() - 1) {
            string path = "DroneNetwork.simControl";
            SimulationControl*  simControl = check_and_cast<SimulationControl*> (getModuleByPath(path.c_str()));
            fadingLogFile << simTime() << ": Hybrid Fading model initialization" << endl;
            Current_map = simControl->Current_map; // pointer to map object
            fadingLogFile << simTime() << ": Map object loaded: " << Current_map << endl;
        }

}

std::ostream& HybridFadingModel::printToStream(std::ostream& stream, int level, int evFlags) const
{
    stream << "HybridFadingModel";
    if (level <= PRINT_LEVEL_TRACE)
        stream << EV_FIELD(alpha)
               << ", system loss = " << systemLoss
               << EV_FIELD(k);
    return stream;
}

double HybridFadingModel::computePathLoss(const ITransmission *transmission, const IArrival *arrival) const
{
    std::string word = ": Choosing path loss type....";
    loggerFunc(word);
    Coord receiverPos = arrival->getStartPosition();
    Coord transmitterPos = transmission->getStartPosition();
    m distance = static_cast<m>(receiverPos.distance(transmitterPos));

    auto radio = check_and_cast<const inet::physicallayer::Radio *>(transmission->getTransmitter());
    auto transmitterModule = (check_and_cast<const cModule *>(radio->getAntenna()))->getParentModule()->getParentModule()->getParentModule();
    auto moduleName =check_and_cast<const DroneControl*>(transmitterModule->getSubmodule("droneControl"));
    word = std::string(" Transmitter is: ") + transmitterModule->getName() + ", ID = " + std::to_string(moduleName->Drone_ID);
    loggerFunc(word);

    //auto reception = check_and_cast<const inet::physicallayer::IReception *>(arrival);
    //auto receiverRadio = reception->getReceiver();
    //auto receiverrModule = (check_and_cast<const cModule *>(receiverRadio->getAntenna()))->getParentModule()->getParentModule()->getParentModule();
    //auto moduleName2 =check_and_cast<const DroneControl*>(receiverrModule->getSubmodule("droneControl"));
    //word = std::string(" Receiver is: ") + receiverrModule->getName() + ", ID = " + std::to_string(moduleName2->Drone_ID);
    //loggerFunc(word);
    //word = ": Receiver coordinates: " + std::to_string(static_cast<int>(receiverPos.x)) + ", " + std::to_string(static_cast<int>(receiverPos.y)) +
    //        "; Transmitter coordinates: " + std::to_string(static_cast<int>(transmitterPos.x)) + ", " + std::to_string(static_cast<int>(transmitterPos.y)) +
    //        ", height check: " + std::to_string(Current_map->getHeightAt(270,270));
    //loggerFunc(word);
    tuple<int, int, int> max_Altitude = Current_map->findMaxHeightOnLine(static_cast<int>(receiverPos.x), static_cast<int>(receiverPos.y), static_cast<int>(transmitterPos.x), static_cast<int>(transmitterPos.y));
    word = ": Checking max height between sender and receiver";
    loggerFunc(word);
    double recHeight = Current_map->getHeightAt(receiverPos.x,receiverPos.y);
    double transHeight = Current_map->getHeightAt(transmitterPos.x,transmitterPos.y);
    double losFunc = recHeight + (transHeight - recHeight) * ((receiverPos.x - get<0>(max_Altitude))/distance.get()); ///need to be improved!!!!!!!!!!!!!!!!!!!!!!!!!!
    if(get<2>(max_Altitude) > losFunc){
        m waveLength = propagationSpeed / frequency;
        double c = 1.0 / (2.0 * (k + 1));
        double x = normal(0, 1);
        double y = normal(0, 1);
        double rr = c * ((x + sqrt(2 * k)) * (x + sqrt(2 * k)) + y * y);
        double freeSpacePathLoss = computeFreeSpacePathLoss(waveLength, distance, alpha, systemLoss);
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(14) << freeSpacePathLoss * rr;
        std::string word = ": Pathloss type: Rayleigh, loss value: " + stream.str() + ", distance: " + std::to_string(distance.get()) + " m";
        loggerFunc(word);
        std::string word2 = " Wavelength: "+std::to_string(waveLength.get())+", alpha: "+std::to_string(alpha)+", systemLoss: "+std::to_string(systemLoss);
        loggerFunc(word2);
        return rr * freeSpacePathLoss;
    }
    else{
        m waveLength = propagationSpeed / frequency;
        double freeSpacePathLoss = computeFreeSpacePathLoss(waveLength, distance, alpha, systemLoss);
        double x = normal(0, 1);
        double y = normal(0, 1);
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(14) << freeSpacePathLoss * sqrt(x * x + y * y);
        std::string word = ": Pathloss type: Rician, loss value: " + stream.str() + ", distance: " + std::to_string(distance.get()) + " m";
        loggerFunc(word);
        std::string word2 = " Wavelength: "+std::to_string(waveLength.get())+", alpha: "+std::to_string(alpha)+", systemLoss: "+std::to_string(systemLoss);
        loggerFunc(word2);
        return freeSpacePathLoss * sqrt(x * x + y * y);
    }
}

void HybridFadingModel::loggerFunc(const std::string word) const{
    fadingLogFile << simTime() << word << endl;
}
