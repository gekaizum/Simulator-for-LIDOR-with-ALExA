package Physical_layer.Drone_block;

package simulations;

simple DroneControl {
    parameters:
		//string communicationType = default("WiFi");
        //string protocol = default("UDP");;  // "UDP" or "TCP", switchable at runtime
        
        int Drone_ID;
        
        int number_of_rotors = default(4);
        double motor_efficiency = default(0.6);
        double mass_of_drone @unit(kg) = default(1.5kg);
        double additional_mass @unit(kg) = default(0kg);
        double frequencyHz @unit(Hz) = default(24000Hz);
        double antena_power @unit(J) = default(0.5J);
        double sensor_power @unit(J) = default(0.7J);
        double additional_power @unit(J) = default(0J);
        double battery_capacity @unit(mAh) = default(8000mAh);
        double battery_remain @unit(mAh) = default(battery_capacity);
        double battery_voltage @unit(V) = default(14.88V);
        int drone_size @unit(m) = default(1m);
        //double hoveringCurrent @unit(J) = default(0J);
        double acceleration @unit(a) = default(0a);
    	double x_velocity @unit(ms) = default(0ms);
    	double y_velocity @unit(ms) = default(0ms);
    	double z_velocity @unit(ms) = default(0ms);
    	double time_step = default(10); //frequency of battery check event
    gates:
        input socketIn;
        output socketOut;
        inout socketControl;
        inout basicProtocolSocket;
        output tcpApp;
        //input in[];
        //output out[];
        input in;    // Incoming messages
        output out;  // Outgoing messages
        input tcpAppIn@directIn; //direct in for incoming tcp messages
        input tcpRequestIn@directIn; //direct in for request to send message via TCP
        input udpRequestIn@directIn; //direct in for request to send message via UDP
        input udpAppIn@directIn; //direct in for incoming udp messages
}
