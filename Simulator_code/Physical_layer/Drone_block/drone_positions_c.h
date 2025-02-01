/*
 * drone_positions_c.h
 *
 *  Created on: Jan 29, 2025
 *      Author: gekaizum
 */

#ifndef PHYSICAL_LAYER_DRONE_BLOCK_DRONE_POSITIONS_C_H_
#define PHYSICAL_LAYER_DRONE_BLOCK_DRONE_POSITIONS_C_H_

#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <unordered_map>
#include "../Map_block/HeightMapLoader.h"
#include "Drone.h"
#include <omnetpp.h>
#include "../simulations/SimulationControlLogger.h"

void update_drone_position(std::vector<Drone*>& drone_data, int drone_id, float* new_endpoint);
void current_drone_move(std::vector<Drone*>& drone_objects, float time_step, HeightMapLoader* Current_Map, SimulationControlLogger *SimControlLogger);



#endif /* PHYSICAL_LAYER_DRONE_BLOCK_DRONE_POSITIONS_C_H_ */
