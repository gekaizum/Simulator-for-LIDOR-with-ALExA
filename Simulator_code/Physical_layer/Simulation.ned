network Simulation {
    submodules:
        heightMapLoader: HeightMapLoader {
            parameters:
                heightMapFile = "heightmap_cm.txt";
                gridSize = 5000;
                blockSize = 100;
        }
        stationManager: ChargingStationManager;
}
