/*
 * This file defines function for position tracking and execution of drone movements including collision detection
 */

#include "drone_positions_c.h"


// Function to update drone movement
// Function will be used when movement command is received to update parameters only
void update_drone_position(std::vector<DroneControl*>& drone_data, int drone_id, float* new_endpoint) {
    drone_data[drone_id-1]->Destination[0] = new_endpoint[0];
    drone_data[drone_id-1]->Destination[1] = new_endpoint[1];
    drone_data[drone_id-1]->Destination[2] = new_endpoint[2];
    drone_data[drone_id-1]->Is_Moving = true;
    drone_data[drone_id-1]->Idle_Steps[0] = 0;
    drone_data[drone_id-1]->Idle_Steps[1] = 0;
    drone_data[drone_id-1]->Idle_Steps[2] = 0;
}

void update_mobility(DroneControl* drone, SimulationControlLogger *SimControlLogger){
    //updates the mobility module
    cModule* mobilityMod = drone->getParentModule()->getSubmodule("mobility");
    PublicStationaryMobility* mobility = check_and_cast<PublicStationaryMobility*>(mobilityMod);
    Coord newPos(drone->Current_Position[0], drone->Current_Position[1], drone->Current_Position[2]);
    mobility->setPositionPublic(newPos);
    EV << " ***********************im here********************" << endl;
}

// Function will check and do all drones moves
void current_drone_move(std::vector<DroneControl*>& drone_objects, float time_step, HeightMapLoader* Current_Map, SimulationControlLogger *SimControlLogger) {
    //SimControlLogger->logFile << "Checking the drone moves"<< endl;
    for (auto& drone : drone_objects) {//Check all existing drone objects
        if (drone->Is_Moving) {//Check only of drone is in motion
            SimControlLogger->logFile << simTime() << ": Drone " << drone->Drone_ID <<" in motion"<< endl;

            //updates the mobility module
            cModule* mobilityMod = drone->getParentModule()->getSubmodule("mobility");
            PublicStationaryMobility* mobility = check_and_cast<PublicStationaryMobility*>(mobilityMod);
            Coord newPos(drone->Current_Position[0], drone->Current_Position[1], drone->Current_Position[2]);
            mobility->setPositionPublic(newPos);
            EV << " ***********************im here********************" << endl;

            // Move in x-axis
            if (drone->Next_Move == 1 && drone->Destination[0] != drone->Current_Position[0]) {// Motion in x axis if is isn't reached it's destination
                SimControlLogger->logFile << simTime() << ": Drone " << drone->Drone_ID <<" move in x direction"<< endl;
                float x_move = drone_objects[drone->Drone_ID-1]->x_velocity * time_step*drone->Idle_Steps[0];//distance passed during the timestep*number_of_idle_time_steps
                if (x_move > 0.1) { //Move must be greater than 10 cm
                    drone->Current_Position[0] += x_move;//update current position of drone
                    if(drone->x_velocity>0){ //Motion in x+ direction
                        if (drone->Current_Position[0]>drone_objects[drone->Drone_ID-1]->Destination[0]){//Drone must stop at its destination
                            drone->Current_Position[0]=drone_objects[drone->Drone_ID-1]->Destination[0];
                        }
                    }
                    else{//Motion in x- direction
                        if (drone->Current_Position[0]<drone_objects[drone->Drone_ID-1]->Destination[0]){
                            drone->Current_Position[0]=drone_objects[drone->Drone_ID-1]->Destination[0];
                        }
                    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    for (auto& col_dec : drone_objects) { //Check collision with other drones
                        if (drone->Drone_ID != col_dec->Drone_ID && col_dec->non_operational == false && drone->Current_Position[0] == col_dec->Current_Position[0] &&
                                                drone->Current_Position[1] == col_dec->Current_Position[1] && drone->Current_Position[2] == col_dec->Current_Position[2]){ //collision detected
                            drone_objects[drone->Drone_ID-1]->non_operational = true;
                            drone->Destination[0]=drone->Current_Position[0];
                            drone->Destination[1]=drone->Current_Position[1];
                            drone->Destination[2]=drone->Current_Position[2];
                            drone->Is_Moving=false;
                            drone->Next_Move = 1;
                            SimControlLogger->logFile << simTime() << ": Drone " << drone->Drone_ID <<" non operational. Collision with drone: " << col_dec->Drone_ID << endl;
                            drone_objects[col_dec->Drone_ID-1]->non_operational = true;
                            col_dec->Destination[0]=col_dec->Current_Position[0];
                            col_dec->Destination[1]=col_dec->Current_Position[1];
                            col_dec->Destination[2]=col_dec->Current_Position[2];
                            col_dec->Is_Moving=false;
                            col_dec->Next_Move = 1;
                            break;
                        }
                    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        //collision with ground
                    if (Current_Map->getHeightAt(drone->Current_Position[0], drone->Current_Position[1]) > drone->Current_Position[2]){
                        drone_objects[drone->Drone_ID-1]->non_operational = true;
                        drone->Destination[0]=drone->Current_Position[0];
                        drone->Destination[1]=drone->Current_Position[1];
                        drone->Destination[2]=drone->Current_Position[2];
                        drone->Is_Moving=false;
                        drone->Next_Move = 1;
                        SimControlLogger->logFile << simTime() << ": Drone " << drone->Drone_ID <<" non operational. Collision with ground at x_position"<< endl;
                        break;
                    }
                    drone_objects[drone->Drone_ID-1]->Current_Position[0]=drone->Current_Position[0]; // update drone object coordinates
                    drone->Next_Move = 2;
                    drone->Idle_Steps[0] = 1;
                }
                else {
                    drone->Next_Move = 2;
                    drone->Idle_Steps[0] += 1;
                }
            }
            else if (drone->Next_Move == 1){ // Move in x direction completed
                drone->Next_Move = 2;
            }
            // Move in y-axis
            if (drone->Next_Move == 2 && drone->Destination[1] != drone->Current_Position[1]) {// Motion in y axis if is isn't reached it's destination
                SimControlLogger->logFile << simTime() << ": Drone " << drone->Drone_ID <<" move in y direction"<< endl;
                float y_move = drone_objects[drone->Drone_ID-1]->y_velocity * time_step * drone->Idle_Steps[1];//distance passed during the timestep*number_of_idle_time_steps
                if (y_move > 0.1) {
                    drone->Current_Position[1] += y_move;
                    if(drone->y_velocity>0){ //Motion in y+ direction
                        if (drone->Current_Position[1]>drone_objects[drone->Drone_ID-1]->Destination[1]){//Drone must stop at its destination
                            drone->Current_Position[1]=drone_objects[drone->Drone_ID-1]->Destination[1];
                        }
                    }
                    else{//Motion in y- direction
                        if (drone->Current_Position[1]<drone_objects[drone->Drone_ID-1]->Destination[1]){
                            drone->Current_Position[1]=drone_objects[drone->Drone_ID-1]->Destination[1];
                        }
                    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    for (auto& col_dec : drone_objects) {//Check collision with other drones
                        if (drone->Drone_ID != col_dec->Drone_ID && col_dec->non_operational == false && drone->Current_Position[0] == col_dec->Current_Position[0] &&
                                                drone->Current_Position[1] == col_dec->Current_Position[1] && drone->Current_Position[2] == col_dec->Current_Position[2]){ //collision detected
                            drone_objects[drone->Drone_ID-1]->non_operational = true;
                            drone->Destination[0]=drone->Current_Position[0];
                            drone->Destination[1]=drone->Current_Position[1];
                            drone->Destination[2]=drone->Current_Position[2];
                            drone->Is_Moving=false;
                            drone->Next_Move = 1;
                            SimControlLogger->logFile << simTime() << ": Drone " << drone->Drone_ID <<" non operational. Collision with drone: " << col_dec->Drone_ID << endl;
                            drone_objects[col_dec->Drone_ID-1]->non_operational = true;
                            col_dec->Destination[0]=col_dec->Current_Position[0];
                            col_dec->Destination[1]=col_dec->Current_Position[1];
                            col_dec->Destination[2]=col_dec->Current_Position[2];
                            col_dec->Is_Moving=false;
                            col_dec->Next_Move = 1;
                            break;
                        }
                    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //collision with ground
                    if (Current_Map->getHeightAt(static_cast<int>(drone->Current_Position[0]), static_cast<int>(drone->Current_Position[1])) > drone->Current_Position[2]){
                        drone_objects[drone->Drone_ID-1]->non_operational = true;
                        drone->Destination[0]=drone->Current_Position[0];
                        drone->Destination[1]=drone->Current_Position[1];
                        drone->Destination[2]=drone->Current_Position[2];
                        drone->Is_Moving=false;
                        drone->Next_Move = 1;
                        SimControlLogger->logFile << simTime() << ": Drone " << drone->Drone_ID <<" non operational. Collision with ground at y_position."<< endl;
                        break;
                    }
                    drone_objects[drone->Drone_ID-1]->Current_Position[1]=drone->Current_Position[1]; // update drone object coordinates
                    drone->Next_Move = 3;
                    drone->Idle_Steps[1] = 1;
                }
                else {
                    drone->Next_Move = 3;
                    drone->Idle_Steps[1] += 1;
                }
            }
            else if (drone->Next_Move == 2){ // Move in x direction completed
                drone->Next_Move = 3;
            }
            // Move in z-axis
            if (drone->Next_Move == 3 && drone->Destination[2] != drone->Current_Position[2]) {// Motion in z axis if is isn't reached it's destination
                SimControlLogger->logFile << simTime() << ": Drone " << drone->Drone_ID <<" move in z direction"<< endl;
                float z_move = drone_objects[drone->Drone_ID-1]->z_velocity * time_step * drone->Idle_Steps[1];//distance passed during the timestep*number_of_idle_time_steps
                if (z_move > 0.1) {
                    drone->Current_Position[2] += z_move;
                    if(drone->z_velocity>0){ //Motion in z+ direction
                        if (drone->Current_Position[2]>drone_objects[drone->Drone_ID-1]->Destination[2]){//Drone must stop at its destination
                            drone->Current_Position[2]=drone_objects[drone->Drone_ID-1]->Destination[2];
                        }
                    }
                    else{//Motion in z- direction
                        if (drone->Current_Position[2]<drone_objects[drone->Drone_ID-1]->Destination[2]){
                            drone->Current_Position[2]=drone_objects[drone->Drone_ID-1]->Destination[2];
                        }
                    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    for (auto& col_dec : drone_objects) {//Check collision with other drones
                        if (drone->Drone_ID != col_dec->Drone_ID && col_dec->non_operational == false && drone->Current_Position[0] == col_dec->Current_Position[0] &&
                                                drone->Current_Position[1] == col_dec->Current_Position[1] && drone->Current_Position[2] == col_dec->Current_Position[2]){ //collision detected
                            drone_objects[drone->Drone_ID-1]->non_operational = true;
                            drone->Destination[0]=drone->Current_Position[0];
                            drone->Destination[1]=drone->Current_Position[1];
                            drone->Destination[2]=drone->Current_Position[2];
                            drone->Is_Moving=false;
                            drone->Next_Move = 1;
                            SimControlLogger->logFile << simTime() << ": Drone " << drone->Drone_ID <<" non operational Collision with drone: " << col_dec->Drone_ID << endl;
                            drone_objects[col_dec->Drone_ID-1]->non_operational = true;
                            col_dec->Destination[0]=col_dec->Current_Position[0];
                            col_dec->Destination[1]=col_dec->Current_Position[1];
                            col_dec->Destination[2]=col_dec->Current_Position[2];
                            col_dec->Is_Moving=false;
                            col_dec->Next_Move = 1;
                            break;
                        }
                    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //collision with ground
                    if (Current_Map->getHeightAt(static_cast<int>(drone->Current_Position[0]), static_cast<int>(drone->Current_Position[1])) > drone->Current_Position[2]){
                        drone_objects[drone->Drone_ID-1]->non_operational = true;
                        drone->Destination[0]=drone->Current_Position[0];
                        drone->Destination[1]=drone->Current_Position[1];
                        drone->Destination[2]=drone->Current_Position[2];
                        drone->Is_Moving=false;
                        drone->Next_Move = 1;
                        SimControlLogger->logFile << simTime() << ": Drone " << drone->Drone_ID <<" non operational. Collision with ground at z_position."<< endl;
                        SimControlLogger->logFile << simTime() << ": Drone " << drone->Drone_ID <<" current x="<< drone->Current_Position[0]
                               << " y=" << drone->Current_Position[1] <<" z="<< drone->Current_Position[2] << " .Terrain height: "
                                << Current_Map->getHeightAt(static_cast<int>(drone->Current_Position[0]), static_cast<int>(drone->Current_Position[1])) << endl;
                        break;
                    }
                    drone_objects[drone->Drone_ID-1]->Current_Position[2]=drone->Current_Position[2]; // update drone object coordinates
                    drone->Next_Move = 1;
                    drone->Idle_Steps[2] = 1;
                }
                else {
                    drone->Next_Move = 1;
                    drone->Idle_Steps[2] += 1;
                }
            }
            else if (drone->non_operational == false && drone->Current_Position[0] == drone->Destination[0] &&
                    drone->Current_Position[1] == drone->Destination[1] && drone->Current_Position[2] == drone->Destination[2]){
                drone->Next_Move = 1;
                drone->Is_Moving = false;
                SimControlLogger->logFile << simTime() << ": Drone " << drone->Drone_ID << " reached destination: x=" << drone->Current_Position[0] <<
                        ", y=" << drone->Current_Position[1] << ", z=" << drone->Current_Position[2] << endl;
            }
            else if (drone->non_operational == false) {
                drone->Next_Move = 1;
            }
        //SimControlLogger->logFile << "Drone " << drone->Drone_ID << " coordinates: x=" << drone->Current_Position[0] <<
        //                                    ", y=" << drone->Current_Position[1] << ", z=" << drone->Current_Position[2] << endl;
        }
    }
}
