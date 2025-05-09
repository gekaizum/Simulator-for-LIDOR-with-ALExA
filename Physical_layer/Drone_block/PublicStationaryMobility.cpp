/*
 * PublicStationaryMobility.cpp
 *
 *  Created on: Apr 9, 2025
 *      Author: shaked
 */


#include "PublicStationaryMobility.h"


Define_Module(PublicStationaryMobility);

void PublicStationaryMobility::initialize(int stage)
{
    StationaryMobilityBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL)
        updateFromDisplayString = par("updateFromDisplayString");
}

void PublicStationaryMobility::refreshDisplay() const
{
    if (updateFromDisplayString) {
        const_cast<PublicStationaryMobility *>(this)->updateMobilityStateFromDisplayString();
        DirectiveResolver directiveResolver(const_cast<PublicStationaryMobility *>(this));
        auto text = format.formatString(&directiveResolver);
        getDisplayString().setTagArg("t", 0, text.c_str());
    }
    else{
        StationaryMobilityBase::refreshDisplay();
    }
}

void PublicStationaryMobility::updateMobilityStateFromDisplayString()
{
    char *end;
    double depth;
    cDisplayString& displayString = subjectModule->getDisplayString();
    canvasProjection->computeCanvasPoint(lastPosition, depth);
    double x = strtod(displayString.getTagArg("p", 0), &end);
    double y = strtod(displayString.getTagArg("p", 1), &end);
    auto newPosition = canvasProjection->computeCanvasPointInverse(cFigure::Point(x, y), depth);
    if (lastPosition != newPosition) {
        lastPosition = newPosition;
        emit(mobilityStateChangedSignal, const_cast<PublicStationaryMobility *>(this));
    }
    Quaternion swing;
    Quaternion twist;
    Coord vector = canvasProjection->computeCanvasPointInverse(cFigure::Point(0, 0), 1);
    vector.normalize();
    lastOrientation.getSwingAndTwist(vector, swing, twist);
    double oldAngle;
    Coord axis;
    twist.getRotationAxisAndAngle(axis, oldAngle);
    double newAngle = math::deg2rad(strtod(displayString.getTagArg("a", 0), &end));
    if (oldAngle != newAngle) {
        lastOrientation *= Quaternion(vector, newAngle - oldAngle);
        emit(mobilityStateChangedSignal, const_cast<PublicStationaryMobility *>(this));
    }
}
void PublicStationaryMobility::setPositionPublic(const Coord& pos) {
    lastPosition = pos;  // Update physical position in simulation
    emit(mobilityStateChangedSignal, this);  // Notify visualizers

    if (subjectModule) {
        subjectModule->getDisplayString().setTagArg("p", 0, pos.x);
        subjectModule->getDisplayString().setTagArg("p", 1, pos.y);
    }
    refreshDisplay();  // Force GUI refresh
}

