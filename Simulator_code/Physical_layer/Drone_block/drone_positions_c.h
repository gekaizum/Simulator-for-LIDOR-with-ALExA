/*
 * This file defines function for position tracking and execution of drone movements including collision detection
 */

#ifndef PHYSICAL_LAYER_DRONE_BLOCK_DRONE_POSITIONS_C_H_
#define PHYSICAL_LAYER_DRONE_BLOCK_DRONE_POSITIONS_C_H_

#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <unordered_map>
#include <omnetpp.h>
#include "../Map_block/HeightMapLoader.h"
#include "../simulations/SimulationControlLogger.h"
#include "DroneControl.h"

void update_drone_position(std::vector<DroneControl*>& drone_data, int drone_id, float* new_endpoint);
void current_drone_move(std::vector<DroneControl*>& drone_objects, float time_step, HeightMapLoader* Current_Map, SimulationControlLogger *SimControlLogger);



#endif /* PHYSICAL_LAYER_DRONE_BLOCK_DRONE_POSITIONS_C_H_ */
