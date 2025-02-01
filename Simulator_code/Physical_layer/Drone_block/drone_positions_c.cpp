#include "drone_positions_c.h"
//All this parameters will be in drone struct object outside this file
//Need to add apropriate include

/*struct Drone {
    int Drone_ID;  // Unique ID for each drone
    std::array<float, 3> current_Position;  // Current position of the drone
    std::array<float, 3> destination;  // Destination position of the drone
    bool Is_Moving;  // Indicates if the drone is moving
    int Next_Move;  // Next move direction: 1 - x axis, 2 - y axis, 3 - z axis
    std::array<int, 3> idle_steps;  // Counter for steps without movement
};

//Init function will be updated according to drone init procedure and maybe also will be in drone object file
std::vector<Drone> initialize_drones(int num_drones, const std::vector<std::array<float, 3>>& initial_positions) {
    std::vector<Drone> drones;
    for (int i = 1; i < num_drones; ++i) {
        Drone drone = {
            i + 1,  // Drone_ID
            initial_positions[i],  // Current_Position
            initial_positions[i],  // Destination
            false,  // Is_Moving
            1,  // Next_Move
            {0, 0, 0}  // Idle_Steps
        };
        drones.push_back(drone);
		//!!!!!!!!!!!!!!!!Here we will create all drone objects!!!!
        //drone_objects=[];
        //drone_objects.append(Drone());
        //###########################################################
    }
    return drones;
}*/

// Function to update drone movement
// Function will be used when movement command is received to update parameters only
void update_drone_position(std::vector<Drone*>& drone_data, int drone_id, float* new_endpoint) {
	drone_data[drone_id-1]->Destination[0] = new_endpoint[0];
	drone_data[drone_id-1]->Destination[1] = new_endpoint[1];
	drone_data[drone_id-1]->Destination[2] = new_endpoint[2];
	drone_data[drone_id-1]->Is_Moving = true;
	drone_data[drone_id-1]->Idle_Steps[0] = 0;
	drone_data[drone_id-1]->Idle_Steps[1] = 0;
	drone_data[drone_id-1]->Idle_Steps[2] = 0;
}

// Function will check and do all drones moves
void current_drone_move(std::vector<Drone*>& drone_objects, float time_step, HeightMapLoader* Current_Map, SimulationControlLogger *SimControlLogger) {
    //SimControlLogger->logFile << "Checking the drone moves"<< endl;
    for (auto& drone : drone_objects) {
        if (drone->Is_Moving) {
            SimControlLogger->logFile << "Drone " << drone->Drone_ID <<" in motion"<< endl;
            // Move in x-axis
            if (drone->Next_Move == 1 && drone->Destination[0] != drone->Current_Position[0]) {
                SimControlLogger->logFile << "Drone " << drone->Drone_ID <<" move in x direction"<< endl;
                float x_move = drone_objects[drone->Drone_ID-1]->x_velocity * time_step;
                if (x_move > 0.2f) {
                    drone->Current_Position[0] += x_move;
                    if (drone->Current_Position[0]>drone_objects[drone->Drone_ID-1]->Destination[0]){
                        drone->Current_Position[0]=drone_objects[drone->Drone_ID-1]->Destination[0];
                    }
					for (auto& col_dec : drone_objects) {
                        if (drone->Drone_ID != col_dec->Drone_ID && drone->Current_Position[0] == col_dec->Current_Position[0]){ //collision detected
                            drone_objects[drone->Drone_ID-1]->non_operational = true;
                            drone->Destination[0]=drone->Current_Position[0];
                            drone->Destination[1]=drone->Current_Position[1];
                            drone->Destination[2]=drone->Current_Position[2];
                            drone->Is_Moving=false;
                            drone->Next_Move = 1;
                            SimControlLogger->logFile << "Drone " << drone->Drone_ID <<" non operational. Collision with drone: " << col_dec->Drone_ID << endl;
                            drone_objects[col_dec->Drone_ID-1]->non_operational = true;
                            col_dec->Destination[0]=col_dec->Current_Position[0];
                            col_dec->Destination[1]=col_dec->Current_Position[1];
                            col_dec->Destination[2]=col_dec->Current_Position[2];
                            col_dec->Is_Moving=false;
                            col_dec->Next_Move = 1;
                            break;
						}
						//collision with ground
                        else if (Current_Map->getHeightAt(drone->Current_Position[0], drone->Current_Position[1]) > drone->Current_Position[2]){
                            drone_objects[drone->Drone_ID-1]->non_operational = true;
                            drone->Destination[0]=drone->Current_Position[0];
                            drone->Destination[1]=drone->Current_Position[1];
                            drone->Destination[2]=drone->Current_Position[2];
                            drone->Is_Moving=false;
                            drone->Next_Move = 1;
                            SimControlLogger->logFile << "Drone " << drone->Drone_ID <<" non operational. Collision with ground at x_position"<< endl;
                            break;
						}
					}
                    drone_objects[drone->Drone_ID-1]->Current_Position[0]=drone->Current_Position[0]; // update drone object coordinates
                    drone->Next_Move = 2;
                    drone->Idle_Steps[0] = 0;
                } else {
                    drone->Next_Move = 2;
                    drone->Idle_Steps[0] += 1;
                }
                continue;
            }
            // Move in y-axis
            if (drone->Next_Move == 2 && drone->Destination[1] != drone->Current_Position[1]) {
                SimControlLogger->logFile << "Drone " << drone->Drone_ID <<" move in y direction"<< endl;
                float y_move = drone_objects[drone->Drone_ID-1]->y_velocity * time_step; // y-axis velocity
                if (y_move > 0.2f) {
                    drone->Current_Position[1] += y_move;
                    if (drone->Current_Position[1]>drone_objects[drone->Drone_ID-1]->Destination[1]){
                        drone->Current_Position[1]=drone_objects[drone->Drone_ID-1]->Destination[1];
                    }
					for (auto& col_dec : drone_objects) {
                        if (drone->Drone_ID != col_dec->Drone_ID && drone->Current_Position[1] == col_dec->Current_Position[1]){ //collision detected
                            drone_objects[drone->Drone_ID-1]->non_operational = true;
                            drone->Destination[0]=drone->Current_Position[0];
                            drone->Destination[1]=drone->Current_Position[1];
                            drone->Destination[2]=drone->Current_Position[2];
                            drone->Is_Moving=false;
                            drone->Next_Move = 1;
                            SimControlLogger->logFile << "Drone " << drone->Drone_ID <<" non operational. Collision with drone: " << col_dec->Drone_ID << endl;
                            drone_objects[col_dec->Drone_ID-1]->non_operational = true;
                            col_dec->Destination[0]=col_dec->Current_Position[0];
                            col_dec->Destination[1]=col_dec->Current_Position[1];
                            col_dec->Destination[2]=col_dec->Current_Position[2];
                            col_dec->Is_Moving=false;
                            col_dec->Next_Move = 1;
                            break;
						}
                        //collision with ground
                        else if (Current_Map->getHeightAt(static_cast<int>(drone->Current_Position[0]), static_cast<int>(drone->Current_Position[1])) > drone->Current_Position[2]){
                            drone_objects[drone->Drone_ID-1]->non_operational = true;
                            drone->Destination[0]=drone->Current_Position[0];
                            drone->Destination[1]=drone->Current_Position[1];
                            drone->Destination[2]=drone->Current_Position[2];
                            drone->Is_Moving=false;
                            drone->Next_Move = 1;
                            SimControlLogger->logFile << "Drone " << drone->Drone_ID <<" non operational. Collision with ground at y_position."<< endl;
                            break;
						}
					}
                    drone_objects[drone->Drone_ID-1]->Current_Position[1]=drone->Current_Position[1]; // update drone object coordinates
                    drone->Next_Move = 3;
                    drone->Idle_Steps[1] = 0;
                } else {
                    drone->Next_Move = 3;
                    drone->Idle_Steps[1] += 1;
                }
                continue;
            }
            // Move in z-axis
            if (drone->Next_Move == 3 && drone->Destination[2] != drone->Current_Position[2]) {
                SimControlLogger->logFile << "Drone " << drone->Drone_ID <<" move in z direction"<< endl;
                float z_move = drone_objects[drone->Drone_ID-1]->z_velocity * time_step; // Example velocity
                if (z_move > 0.2f) {
                    drone->Current_Position[2] += z_move;
                    if (drone->Current_Position[2]>drone_objects[drone->Drone_ID-1]->Destination[2]){
                        drone->Current_Position[2]=drone_objects[drone->Drone_ID-1]->Destination[2];
                    }
					for (auto& col_dec : drone_objects) {
                        if (drone->Drone_ID != col_dec->Drone_ID && drone->Current_Position[2] == col_dec->Current_Position[2]){ //collision detected
                            drone_objects[drone->Drone_ID-1]->non_operational = true;
                            drone->Destination[0]=drone->Current_Position[0];
                            drone->Destination[1]=drone->Current_Position[1];
                            drone->Destination[2]=drone->Current_Position[2];
                            drone->Is_Moving=false;
                            drone->Next_Move = 1;
                            SimControlLogger->logFile << "Drone " << drone->Drone_ID <<" non operational Collision with drone: " << col_dec->Drone_ID << endl;
                            drone_objects[col_dec->Drone_ID-1]->non_operational = true;
                            col_dec->Destination[0]=col_dec->Current_Position[0];
                            col_dec->Destination[1]=col_dec->Current_Position[1];
                            col_dec->Destination[2]=col_dec->Current_Position[2];
                            col_dec->Is_Moving=false;
                            col_dec->Next_Move = 1;
                            break;
						}
                        //collision with ground
                        else if (Current_Map->getHeightAt(static_cast<int>(drone->Current_Position[0]), static_cast<int>(drone->Current_Position[1])) > drone->Current_Position[2]){
                            drone_objects[drone->Drone_ID-1]->non_operational = true;
                            drone->Destination[0]=drone->Current_Position[0];
                            drone->Destination[1]=drone->Current_Position[1];
                            drone->Destination[2]=drone->Current_Position[2];
                            drone->Is_Moving=false;
                            drone->Next_Move = 1;
                            SimControlLogger->logFile << "Drone " << drone->Drone_ID <<" non operational. Collision with ground at z_position."<< endl;
                            SimControlLogger->logFile << "Drone " << drone->Drone_ID <<" current x="<< drone->Current_Position[0]
                                   << " y=" << drone->Current_Position[1] <<" z="<< drone->Current_Position[2] << " .Terrain height: "
                                    << Current_Map->getHeightAt(static_cast<int>(drone->Current_Position[0]), static_cast<int>(drone->Current_Position[1])) << endl;
                            break;
						}
					}
                    drone_objects[drone->Drone_ID-1]->Current_Position[2]=drone->Current_Position[2]; // update drone object coordinates
                    drone->Next_Move = 1;
                    drone->Idle_Steps[2] = 0;
                } else {
                    drone->Next_Move = 1;
                    drone->Idle_Steps[2] += 1;
                }
                continue;
            } else {
                drone->Next_Move = 1;
                drone->Is_Moving = false;
                SimControlLogger->logFile << "Drone " << drone->Drone_ID << " reached destination: x=" << drone->Current_Position[0] <<
                        ", y=" << drone->Current_Position[1] << ", z=" << drone->Current_Position[2] << endl;
            }
            SimControlLogger->logFile << "Drone " << drone->Drone_ID << " coordinates: x=" << drone->Current_Position[0] <<
                                            ", y=" << drone->Current_Position[1] << ", z=" << drone->Current_Position[2] << endl;
        }
    }
}

// Example usage
// Define the number of drones
// num_drones = 3

// Define initial positions for each drone (format: x.r, y.r, z.r where r=0, 2, 4, 6, 8)

//int main() {
//    int num_drones = 3;
//    std::vector<std::array<float, 3>> initial_positions = {
//        {0.0f, 0.0f, 0.0f},
//        {1.2f, 2.4f, 0.4f},
//        {3.4f, 4.0f, 0.6f}
//    };

//    std::vector<Drone> drones = initialize_drones(num_drones, initial_positions);

//   for (const auto& drone : drones) {
//        std::cout << "Drone ID: " << drone.Drone_ID << ", Position: ("
//                  << drone.Current_Position[0] << ", "
//                  << drone.Current_Position[1] << ", "
//                  << drone.Current_Position[2] << ")\n";
//    }

//    return 0;
//}
