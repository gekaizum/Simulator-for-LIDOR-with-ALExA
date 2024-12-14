# Import necessary library for working with tables (optional)
import pandas as pd

# Function to initialize the drone list
def initialize_drones(num_drones, initial_positions):
    """
    Initialize the drone data for the simulation.

    Args:
        num_drones (int): Number of drones in the simulation.
        initial_positions (list of lists): Initial positions of drones in [x.r, y.r, z.r] format.

    Returns:
        list: A list of dictionaries containing drone data.
    """
    drones = []
    for i in range(num_drones):
        drone = {
            "Drone_ID": i + 1,  # Unique ID for each drone
            "Current_Position": initial_positions[i],  # Initial position of the drone
            "Destination": initial_positions[i],  # Initial route is the current position
            "Is_Moving": False,  # Initially drones are stationary
			"Next_Move": 1, # Next move direction: 1 - x axis, 2 - y axis, 3 - z axis
            "Idle_Steps": [0,0,0],  # Counter for steps without movement
        }
        drones.append(drone)
		##!!!!!!!!!!!!!!!!Here we will create all drone objects!!!!
        #drone_objects=[];
        #drone_objects.append(Drone())
        ############################################################
    return drones

# Function to update drone movement
def update_drone_position(drone_data, drone_id, new_endpoint):
    """
    Update the position and status of a specific drone.

    Args:
        drone_data (list): List of dictionaries containing drone data.
        drone_id (int): ID of the drone to update.
        new_endpoint (tuple): New position of the drone in (x.r, y.r, z.r) format.

    Returns:
        None
    """
    for drone in drone_data:
        if drone["Drone_ID"] == drone_id:
            drone["Destination"] = new_position
            drone["Is_Moving"] = True
            drone["Idle_Steps"] = [0,0,0]
            break

# Function will check and do all drones moves
def current_drone_move(drone_data,time_step=1):
	for drone in drone_data: # check drones one by one
		if drone["Is_Moving"] == True: # only drones that received movement command
            # move in x axis
			if drone["Next_Move"] == 1 && drone["Destination"][0] != drone["Current_Position"][0]: 
                x_move=drone_objects[drone["Drone_ID"]].x_velocity * time_step
				if x_move > 0.2: # move distance more than minimum
                    drone["Current_Position"][0]+=x_move
                    for col_dec in drone_data:
                        if drone["Drone_ID"] != col_dec["Drone_ID"] && drone["Current_Position"][0] == col_dec["Current_Position"][0]: #collision detected
                            drone_objects[drone["Drone_ID"]].non_operational = True
                            drone["Destination"]=drone["Current_Position"]
                            drone["Is_Moving"]=False
                            break
                        #else if
                            ##!!!!!!!!!!!!!!!!Here we will check collision with ground!!!!
                            
                            ############################################################
                    drone_objects[drone["Drone_ID"]].current_position[0]=drone["Current_Position"][0] # update drone object coordinates
                    drone["Next_Move"] = 2
                    drone["Idle_Steps"][0] = 0
					continue
                else: # move distance less than minimum step
                    drone["Next_Move"] = 2
                    drone["Idle_Steps"][0] += 1
            # move in y axis
            if drone["Next_Move"] == 2 && drone["Destination"][1] != drone["Current_Position"][1]: 
                y_move=drone_objects[drone["Drone_ID"]].y_velocity * time_step
                if y_move > 0.2:
                    drone["Current_Position"][1]+=y_move
                    for col_dec in drone_data:
                        if drone["Drone_ID"] != col_dec["Drone_ID"] && drone["Current_Position"][1] == col_dec["Current_Position"][1]: #collision detected
                            drone_objects[drone["Drone_ID"]].non_operational = True
                            drone["Destination"]=drone["Current_Position"]
                            drone["Is_Moving"]=False
                            break
                        ##!!!!!!!!!!!!!!!!Here we will check collision with ground!!!!
                            
                        ############################################################
                    drone_objects[drone["Drone_ID"]].current_position[1]=drone["Current_Position"][1]
                    drone["Next_Move"] = 3
                    drone["Idle_Steps"][1] = 0
                    continue
                else:
                    drone["Next_Move"] = 3
                    drone["Idle_Steps"][1] += 1
            # move in z axis
            if drone["Next_Move"] == 3 && drone["Destination"][2] != drone["Current_Position"][2]: 
                z_move=drone_objects[drone["Drone_ID"]].z_velocity * time_step
                if z_move > 0.2:
                    drone["Current_Position"][2]+=z_move
                    for col_dec in drone_data:
                        if drone["Drone_ID"] != col_dec["Drone_ID"] && drone["Current_Position"][2] == col_dec["Current_Position"][2]: #collision detected
                            drone_objects[drone["Drone_ID"]].non_operational = True
                            drone["Destination"]=drone["Current_Position"]
                            drone["Is_Moving"]=False
                            break
                        ##!!!!!!!!!!!!!!!!Here we will check collision with ground!!!!
                            
                        ############################################################
                    drone_objects[drone["Drone_ID"]].current_position[2]=drone["Current_Position"][2]
                    drone["Next_Move"] = 1
                    drone["Idle_Steps"][2] = 0
                    continue
                else:
                    drone["Next_Move"] = 1
                    drone["Idle_Steps"][2] += 1
            else:
                drone["Next_Move"]=1
                drone["Is_Moving"]=False

# Example usage
# Define the number of drones
#num_drones = 3

# Define initial positions for each drone (format: x.r, y.r, z.r where r=0, 2, 4, 6, 8)
#initial_positions = [
#    (0.0, 0.0, 0.0),
#    (1.2, 2.4, 0.4),
#    (3.4, 4.0, 0.6),
#]

# Initialize the drone list
#drones = initialize_drones(num_drones, initial_positions)

# Convert the list of drones to a DataFrame for better visualization (optional)
#drone_df = pd.DataFrame(drones)

# Display the table with columns
#print(drone_df)

# Example output table:
# | Drone_ID | Current_Position | Destination    | Is_Moving | Idle_Steps |
# |----------|------------------|----------------|-----------|------------|
# | 1        | (0.0, 0.0, 0.0) | (0.0, 0.0, 0.0)| False     | 0          |
# | 2        | (1.2, 2.4, 0.4) | (1.2, 2.4, 0.4)| False     | 0          |
# | 3        | (3.4, 4.0, 0.6) | (3.4, 4.0, 0.6)| False     | 0          |
