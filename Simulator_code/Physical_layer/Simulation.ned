network Simulation {
    submodules:
        heightMapLoader: HeightMapLoader {
            parameters:
                heightMapFile = "heightmap.txt";
                gridSize = 5000;
                blockSize = 100;
        }
        stationManager: ChargingStationManager;
}
