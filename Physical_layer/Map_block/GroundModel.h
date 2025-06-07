/*
 * GroundModel.h
 *
 *  Created on: May 14, 2025
 *      Author: shaked
 */

#ifndef PHYSICAL_LAYER_MAP_BLOCK_GROUNDMODEL_H_
#define PHYSICAL_LAYER_MAP_BLOCK_GROUNDMODEL_H_

#include "inet/environment/contract/IGround.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace inet;
using namespace physicalenvironment;
using namespace std;

class  GroundModel : public IGround, public cModule
{
  protected:
    vector<vector<int>> elevation;

  protected:
    virtual void initialize(int stage) override;

  public:
    virtual Coord computeGroundProjection(const Coord& position) const override;
    virtual Coord computeGroundNormal(const Coord& position) const override;
    virtual vector<vector<int>> loadHeightMatrix(const string& filename);
};


#endif /* PHYSICAL_LAYER_MAP_BLOCK_GROUNDMODEL_H_ */
