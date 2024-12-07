simple HeightMapLoader {
    parameters:
        string heightMapFile;  // Path to the heightmap file
        int gridSize;          // Size of the map (e.g., 5000 for a 5000x5000 map)
        int blockSize = default(100); // Size of blocks for block-based loading (optional)
}

network Simulation {
    submodules:
        heightMapLoader: HeightMapLoader {
            parameters:
                heightMapFile = "heightmap.txt"; // Path to the heightmap file
                gridSize = 5000;                   // Define the size of the map
                blockSize = 100;                   // Optional: define block size
        }
}
