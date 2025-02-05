#ifndef HEIGHTMAPLOADER_H_
#define HEIGHTMAPLOADER_H_

#include <omnetpp.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace omnetpp;

class HeightMapLoader {
private:
    std::vector<std::vector<int>> blockCache; // Cache for the currently loaded block of the height map
    int blockSize;
    int currentBlockX, currentBlockY;
    std::string heightMapFile;    // Path to the height map file

protected:
    //virtual void handleMessage(cMessage *msg) override;
    void loadBlock(int blockX, int blockY);

public:
    HeightMapLoader(std::string heightmap, int bS);
    //virtual ~HeightMapLoader();

    //void loadHeightMap(const std::string& filename);
    int getHeightAt(float x, float y);
    std::tuple<int, int, int> findMaxHeightOnLine(int x1, int y1, int x2, int y2);
};

#endif /* HEIGHTMAPLOADER_H_ */

