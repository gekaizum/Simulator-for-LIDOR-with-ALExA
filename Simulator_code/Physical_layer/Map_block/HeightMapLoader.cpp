#include <omnetpp.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

class HeightMapLoader : public cSimpleModule {
private:
    // Cache for the currently loaded block of the height map
    std::vector<std::vector<int>> blockCache;
    int blockSize;                // Size of the block to load
    int currentBlockX, currentBlockY; // Current block coordinates in the grid
    std::string heightMapFile;    // Path to the height map file

protected:
    // Initialize parameters from the NED file
    virtual void initialize() override {
        heightMapFile = par("heightmap").stringValue(); // Get the height map file path
        blockSize = par("blockSize").intValue();            // Get the size of the block
        currentBlockX = -1;  // Set initial block coordinates to invalid values
        currentBlockY = -1;
    }

    // Load a specific block from the height map file
    void loadBlock(int blockX, int blockY) {
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
            if (rowIndex >= startRow && rowIndex < endRow) { // Check if row is within the block
                std::vector<int> row;
                std::istringstream ss(line);
                int colIndex = 0, height;

                // Read each column in the row
                while (ss >> height) {
                    if (colIndex >= startCol && colIndex < endCol) { // Check if column is within the block
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

public:
    // Retrieve the height value at the specified coordinates
    int getHeightAt(int x, int y) {
        // Determine which block the coordinates belong to
        int blockX = x / blockSize;
        int blockY = y / blockSize;

        // Load the block if it is not currently loaded
        if (blockX != currentBlockX || blockY != currentBlockY) {
            loadBlock(blockX, blockY);
        }

        // Calculate local coordinates within the block
        int localX = x % blockSize;
        int localY = y % blockSize;
        return blockCache[localX][localY]; // Return the height value
    }
};
