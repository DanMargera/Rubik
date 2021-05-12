#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Rubik.h"

namespace Algorithm
{

bool bruteSolve(RubikCube& cube, int maxDepth, int lastPos=-1);

void downCross(RubikCube& cube);

void downCorners(RubikCube& cube);

void middleLayer(RubikCube& cube);

void topCross(RubikCube& cube);

void topFace(RubikCube& cube);

void topLayer(RubikCube& cube);

void finishTop(RubikCube& cube);

void layerSolve(RubikCube& cube);

}

#endif
