#include "HeightMapLoader.h"

using namespace omnetpp;

// Initialize parameters from the NED file
HeightMapLoader::HeightMapLoader(std::string heightmap, int bS) {
    heightMapFile = heightmap; // Get the height map file path
    //blockSize = par("blockSize").intValue();            // Get the size of the block
    blockSize = bS;
    currentBlockX = -1;  // Set initial block coordinates to invalid values
    currentBlockY = -1;
}

// Load a specific block from the height map file
void HeightMapLoader::loadBlock(int blockX, int blockY) {
    blockCache.clear(); // Clear the cache to load a new block

    // Open the height map file
    std::ifstream file(heightMapFile);
    if (!file.is_open()) {
        throw cRuntimeError("Failed to open height map file: %s", heightMapFile.c_str());
    }

    // Calculate the range of rows and columns to load
    int startRow = blockX * blockSize;
    int startCol = blockY * blockSize;
    int endRow = startRow + blockSize;
    int endCol = startCol + blockSize;

    std::string line;
    int rowIndex = 0; // Current row index being read

    // Read the file line by line
    while (std::getline(file, line)) {
        if (rowIndex >= startRow && rowIndex < endRow) { // Check if the row is within the block
            std::vector<int> row;
            std::istringstream ss(line);
            int colIndex = 0, height;

            // Read each column in the row
            while (ss >> height) {
                if (colIndex >= startCol && colIndex < endCol) { // Check if the column is within the block
                    row.push_back(height);
                }
                colIndex++;
            }
            blockCache.push_back(row); // Add the row to the cache
        }
        rowIndex++;
        if (rowIndex >= endRow) { // Stop reading when the block is fully loaded
            break;
        }
    }

    // Update the current block coordinates
    currentBlockX = blockX;
    currentBlockY = blockY;
}

// Retrieve the height value at the specified coordinates
int HeightMapLoader::getHeightAt(float x, float y) {
    // Determine which block contains the specified coordinates
    int x_int = x;
    int y_int = y;
    int blockX = x_int / blockSize;
    int blockY = y_int / blockSize;
    //EV << "Check block: " << blockX << ", " << blockY << endl;
    // Load the block if it's not currently loaded
    if (blockX != currentBlockX || blockY != currentBlockY) {
        loadBlock(blockX, blockY);
    }

    // Calculate local coordinates within the block
    int localX = x_int % blockSize;
    int localY = y_int % blockSize;
    return blockCache[localX][localY]; // Return the height value
}

// Find the point with the maximum height along the line from (x1, y1) to (x2, y2)
std::tuple<int, int, int> HeightMapLoader::findMaxHeightOnLine(int x1, int y1, int x2, int y2) {
    int dx = std::abs(x2 - x1); // Absolute difference in x-coordinates
    int dy = std::abs(y2 - y1); // Absolute difference in y-coordinates
    int sx = (x1 < x2) ? 1 : -1; // Step direction for x
    int sy = (y1 < y2) ? 1 : -1; // Step direction for y

    int err = dx - dy; // Error term for Bresenham's algorithm

    int maxZ = getHeightAt(static_cast<float>(x1), static_cast<float>(y1)); // Initialize max height with the starting point
    int maxX = x1, maxY = y1; // Initialize coordinates of the max height point

    while (true) {
        // Get the height at the current coordinates
        int currentZ = getHeightAt(static_cast<float>(x1), static_cast<float>(y1));
        if (currentZ > maxZ) {
            maxZ = currentZ;
            maxX = x1;
            maxY = y1;
        }

        // If we reach the end point, exit the loop
        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err; // Double the error term
        if (e2 > -dy) {   // Adjust x if necessary
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {    // Adjust y if necessary
            err += dx;
            y1 += sy;
        }
    }

    // Return the coordinates and height of the maximum point
    return {maxX, maxY, maxZ};
}

