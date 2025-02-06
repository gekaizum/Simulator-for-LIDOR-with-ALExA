package Physical_layer.Drone_block;
package simulations;

//****new - For wireless Network****//

import inet.node.inet.AdhocHost;
import inet.routing.aodv.Aodv;

//****new - For wireless Network****//

module Drone extends AdhocHost
{
    parameters: 
        int Drone_ID;
        int number_of_rotors = 4;
        double motor_efficiency = default(0.6);
        double mass_of_drone @unit(kg) = default(1.5kg);
        //double mass_of_drone = default(1.5);
        double additional_mass @unit(kg) = default(0kg);
        //double additional_mass = default(0);
        double frequencyHz @unit(Hz) = default(24000Hz);
        double antena_power @unit(J) = default(0.5J);
        double sensor_power @unit(J) = default(0.7J);
        double additional_power @unit(J) = default(0J);
        double battery_capacity @unit(mAh) = default(8000mAh);
        double battery_remain @unit(mAh) = default(battery_capacity);
        double battery_voltage @unit(V) = default(14.88V);
        int drone_size @unit(m) = default(1m);
        //double hoveringCurrent @unit(J) = default(0J);
        //string communicationType = default("WiFi");
        double acceleration @unit(a) = default(0a);
    	double x_velocity @unit(ms) = default(0ms);
    	double y_velocity @unit(ms) = default(0ms);
    	double z_velocity @unit(ms) = default(0ms);
    gates:
        input socketIn;
        output socketOut;
        inout socketControl;
        
//****new - For wireless Network****//
        
    submodules:
        aodv: Aodv {
            @display("p=825,226");
        }
    connections:
        aodv.socketOut --> at.in++;
        aodv.socketIn <-- at.out++;
        
//****new - For wireless Network****//

}

