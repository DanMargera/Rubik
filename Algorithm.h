#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Rubik.h"

namespace Algorithm
{

bool bruteSolve(RubikCube& cube, int maxDepth);

void downCross(RubikCube& cube);

}

#endif
