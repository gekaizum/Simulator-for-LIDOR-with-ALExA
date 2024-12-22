simple SignalStrengthCalculation {
    parameters:
        string propagationModel; // "FreeSpace", "TwoRay", or "KnifeEdge"
        string communicationType; // Communication type (e.g., "WiFi" or "Radio")
        double transmittedPower; // Transmitter power in watts (W)
        double gainTransmitter; // Gain of the transmitter antenna (linear scale)
        double gainReceiver; // Gain of the receiver antenna (linear scale)
        double distance; // Distance between transmitter and receiver (meters)
        double frequency; // Frequency of the signal (Hz)
        double h_Transmitter; // Height of the transmitter antenna (meters)
        double h_Receiver; // Height of the receiver antenna (meters)
        double t_to_obst; // Distance to the obstacle from the transmitter (meters)
        double r_to_obst; // Distance to the obstacle from the receiver (meters)
        double h_obstacle; // Maximum height of the obstacle (meters)
        // Optional parameters for terrain-based calculation (if applicable)
        // double transmitterX;
        // double transmitterY;
        // double receiverX;
        // double receiverY;
    gates:
        input in;
        output out;
}
