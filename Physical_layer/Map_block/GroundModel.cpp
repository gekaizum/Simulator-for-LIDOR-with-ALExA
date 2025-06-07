/*
 * GroundModel.cpp
 *
 *  Created on: May 14, 2025
 *      Author: shaked
 */

#include "GroundModel.h"

Define_Module(GroundModel);

void GroundModel::initialize(int stage)
{
    if (stage == INITSTAGE_LOCAL)
        elevation = loadHeightMatrix("heightmap.txt");
}

Coord GroundModel::computeGroundProjection(const Coord& position) const
{
    return Coord(position.x, position.y, elevation[position.x][position.y]);
}

Coord GroundModel::computeGroundNormal(const Coord& position) const
{
    return Coord(0, 0, 1);
}

vector<vector<int>> GroundModel::loadHeightMatrix(const string& filename) {
    vector<vector<int>> matrix;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        vector<int> row;
        int value;

        while (iss >> value) {
            row.push_back(value);
        }

        if (!row.empty()) {
            matrix.push_back(row);
        }
    }

    return matrix;
}

