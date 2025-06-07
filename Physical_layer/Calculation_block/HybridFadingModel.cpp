/*
 * HybridFadingModel.cpp
 *
 *  Created on: May 31, 2025
 *      Author: shaked
 */

#include "HybridFadingModel.h"
#include "../Map_block/HeightMapLoader.h"

Define_Module(HybridFadingModel);


HybridFadingModel::HybridFadingModel() :
    k(1)
{
}

void HybridFadingModel::initialize(int stage)
{
    if (stage == INITSTAGE_LOCAL) {
        physicalEnvironment = getModuleFromPar<inet::physicalenvironment::IPhysicalEnvironment>(par("physicalEnvironmentModule"), this);
    }
    FreeSpacePathLoss::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        k = math::dB2fraction(par("k"));
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

//double HybridFadingModel::computePathLoss(mps propagationSpeed, Hz frequency, m distance, const ITransmission *transmission, cost IArrival *arrival) const
//double HybridFadingModel::computePathLoss(mps propagationSpeed, Hz frequency, m distance, const ITransmission *transmission, const IArrival *arrival) const
double HybridFadingModel::computePathLoss(mps propagationSpeed, Hz frequency, m distance) const
{

    //auto receiverPos = arrival->getStartPosition();
    //auto transmitterPos = transmission->getStartPosition();

    //max_Altitude = get<2>(HeightMapLoader::findMaxHeightOnLine(receiverPos.x, receiverPos.y, transmitterPos.x, transmitterPos.y));

    //if(max_Altitude <  receiverPos.z && max_Altitude < transmitterPos.z){
    if(0){
        m waveLength = propagationSpeed / frequency;
        double c = 1.0 / (2.0 * (k + 1));
        double x = normal(0, 1);
        double y = normal(0, 1);
        double rr = c * ((x + sqrt(2 * k)) * (x + sqrt(2 * k)) + y * y);
        double freeSpacePathLoss = computeFreeSpacePathLoss(waveLength, distance, alpha, systemLoss);
        return rr * freeSpacePathLoss;
    }
    else{
        m waveLength = propagationSpeed / frequency;
        double freeSpacePathLoss = computeFreeSpacePathLoss(waveLength, distance, alpha, systemLoss);
        double x = normal(0, 1);
        double y = normal(0, 1);
        return freeSpacePathLoss * sqrt(x * x + y * y);
    }
}
