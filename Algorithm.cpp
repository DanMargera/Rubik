#include "Algorithm.h"

bool Algorithm::bruteSolve(RubikCube& cube, int maxDepth)
{
    if (maxDepth == 0) return false;
    for (int i=0; i<6; ++i) {
        cube.rotateSide(static_cast<Position>(i), false);
        if (cube.isSolved() || bruteSolve(cube, maxDepth-1)) return true;
        cube.rotateSide(static_cast<Position>(i), true);
        cube.rotateSide(static_cast<Position>(i), true);
        if (cube.isSolved() || bruteSolve(cube, maxDepth-1)) return true;
        cube.rotateSide(static_cast<Position>(i), false);
    }
    return false;
}
