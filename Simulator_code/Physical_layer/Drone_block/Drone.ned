package Physical_layer.Drone_block;

package simulations;

simple Drone
{
    parameters:
        int Drone_ID;
        int number_of_rotors = 4;
        double motor_efficiency = default(1.5);
        double mass_of_drone @unit(kg) = default(1.5kg);
        double additional_mass @unit(kg) = default(0kg);
        double frequencyHz @unit(Hz) = default(24000Hz);
        double antena_power @unit(J) = default(100J);
        double sensor_power @unit(J) = default(100J);
        double additional_power @unit(J) = default(0J);
        double batteryCapacity @unit(mAh) = default(5000mAh);
        double battery_remain @unit(mAh) = default(batteryCapacity);
        double battery_voltage @unit(V) = default(3V);
        int drone_size @unit(m) = default(1m);
        double hoveringCurrent @unit(J) = default(0J);
        //string communicationType = default("WiFi");
        double acceleration @unit(a) = default(0a);
    	double x_velocity @unit(ms) = default(0ms);
    	double y_velocity @unit(ms) = default(0ms);
    	double z_velocity @unit(ms) = default(0ms);
    gates:
        input socketIn;
        output socketOut;
}

