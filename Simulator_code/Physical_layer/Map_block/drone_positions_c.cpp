#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <unordered_map>
#include "HeightMapLoader.cpp"

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
void update_drone_position(std::vector<Drone>& drone_data, int drone_id, const std::array<float, 3>& new_endpoint) {
	drone_data[drone_id-1].Destination = new_endpoint;
	drone_data[drone_id-1].Is_Moving = true;
	drone_data[drone_id-1].Idle_Steps = {0, 0, 0};
}

// Function will check and do all drones moves
void current_drone_move(std::vector<Drone>& drone_data, float time_step = 1.0f) {
    for (auto& drone : drone_data) {
        if (drone.Is_Moving) {
            // Move in x-axis
            if (drone.Next_Move == 1 && drone.Destination[0] != drone.Current_Position[0]) {
                float x_move = drone_objects[drone.Drone_ID].x_velocity * time_step; // Example velocity
                if (x_move > 0.2f) {
                    drone.Current_Position[0] += x_move;
					for (auto& col_dec : drone_data) {
                        if (drone.Drone_ID != col_dec.Drone_ID && drone.Current_Position[0] == col_dec.Current_Position[0]){ //collision detected
                            drone_objects[drone.Drone_ID].non_operational = True;
                            drone.Destination=drone.Current_Position;
                            drone.Is_Moving=False
                            break
						}
						//collision with ground
                        else if (getHeightAt(static_cast<int>(drone.Current_Position[0]), static_cast<int>(drone.Current_Position[1])) >= drone.Current_Position[2]){
                            drone_objects[drone.Drone_ID].non_operational = True;
                            drone.Destination=drone.Current_Position;
                            drone.Is_Moving=False
                            break
						}
					}
                    drone_objects[drone.Drone_ID].current_position[0]=drone.Current_Position[0]; // update drone object coordinates               
                    drone.Next_Move = 2;
                    drone.Idle_Steps[0] = 0;
                } else {
                    drone.Next_Move = 2;
                    drone.Idle_Steps[0] += 1;
                }
                continue;
            }
            // Move in y-axis
            if (drone.Next_Move == 2 && drone.Destination[1] != drone.Current_Position[1]) {
                float y_move = drone_objects[drone.Drone_ID].y_velocity * time_step; // y-axis velocity
                if (y_move > 0.2f) {
                    drone.Current_Position[1] += y_move;
					for (auto& col_dec : drone_data) {
                        if (drone.Drone_ID != col_dec.Drone_ID && drone.Current_Position[1] == col_dec.Current_Position[1]){ //collision detected
                            drone_objects[drone.Drone_ID].non_operational = True;
                            drone.Destination=drone.Current_Position;
                            drone.Is_Moving=False
                            break
						}
                        //collision with ground
                        else if (getHeightAt(static_cast<int>(drone.Current_Position[0]), static_cast<int>(drone.Current_Position[1])) >= drone.Current_Position[2]){
                            drone_objects[drone.Drone_ID].non_operational = True;
                            drone.Destination=drone.Current_Position;
                            drone.Is_Moving=False
                            break
						}
					}
                    drone_objects[drone.Drone_ID].current_position[1]=drone.Current_Position[1]; // update drone object coordinates               
                    drone.Next_Move = 3;
                    drone.Idle_Steps[1] = 0;
                } else {
                    drone.Next_Move = 3;
                    drone.Idle_Steps[1] += 1;
                }
                continue;
            }
            // Move in z-axis
            if (drone.Next_Move == 3 && drone.Destination[2] != drone.Current_Position[2]) {
                float z_move = drone_objects[drone.Drone_ID].z_velocity * time_step; // Example velocity
                if (z_move > 0.2f) {
                    drone.Current_Position[2] += z_move;
					for (auto& col_dec : drone_data) {
                        if (drone.Drone_ID != col_dec.Drone_ID && drone.Current_Position[2] == col_dec.Current_Position[2]){ //collision detected
                            drone_objects[drone.Drone_ID].non_operational = True;
                            drone.Destination=drone.Current_Position;
                            drone.Is_Moving=False
                            break
						}
                        //collision with ground
                        else if (getHeightAt(static_cast<int>(drone.Current_Position[0]), static_cast<int>(drone.Current_Position[1])) >= drone.Current_Position[2]){
                            drone_objects[drone.Drone_ID].non_operational = True;
                            drone.Destination=drone.Current_Position;
                            drone.Is_Moving=False
                            break
						}
					}
                    drone_objects[drone.Drone_ID].current_position[2]=drone.Current_Position[2]; // update drone object coordinates               
                    drone.Next_Move = 1;
                    drone.Idle_Steps[2] = 0;
                } else {
                    drone.Next_Move = 1;
                    drone.Idle_Steps[2] += 1;
                }
                continue;
            } else {
                drone.Next_Move = 1;
                drone.Is_Moving = false;
            }
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
