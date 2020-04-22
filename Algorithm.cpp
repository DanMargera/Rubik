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

/*
 * First iteration of an algorithm to assemble a cross at the bottom.
 * Things to improve later:
 * - Use relative positions
 * - Break down in smaller prefab algorithms with meaningful names
 * - Improve legibility, this is unreadable
 */
void Algorithm::downCross(RubikCube& cube)
{
    Color downColor = cube.getCenterColor(Position::down);

    for (int i=0; i<4; ++i) {
        // Find current location of down-front edge cubid
        auto cubidCoordinates = cube.findCubids({downColor, cube.getCenterColor(Position::front)}, CubidType::edge)[0];
        Position currentSide = isOnSide(cubidCoordinates, Position::left) ? Position::left : Position::right;
        if (!isOnSide(cubidCoordinates, Position::front)) {
            if (!isOnSide(cubidCoordinates, Position::up)) {
                if (!isOnSide(cubidCoordinates, Position::back)) {
                    // Bottom-right and bottom-left cases
                    cube.rotateSide(currentSide, false);
                    cube.rotateSide(currentSide, false);
                    cube.rotateSide(Position::up, currentSide == Position::left);
                    cube.rotateSide(currentSide, false);
                    cube.rotateSide(currentSide, false);
                } else {
                    // Back-left, back-right and back-bottom cases
                    cube.rotateSide(Position::back, currentSide == Position::left);
                    if (isOnSide(cubidCoordinates, Position::down)) {
                        cube.rotateSide(Position::back, currentSide == Position::left);
                    }
                    cube.rotateSide(Position::up, false);
                    cube.rotateSide(Position::up, false);
                    cube.rotateSide(Position::back, currentSide != Position::left);
                    if (isOnSide(cubidCoordinates, Position::down)) {
                        cube.rotateSide(Position::back, currentSide != Position::left);
                    }
                }
            } else {
                // Up-left, up-right and up-back cases
                cube.rotateSide(Position::up, currentSide == Position::left);
                if (isOnSide(cubidCoordinates, Position::back)) {
                    cube.rotateSide(Position::up, currentSide == Position::left);
                }
            }
        } else if (!isOnSide(cubidCoordinates, Position::up)) {
            cube.rotateSide(Position::front, currentSide != Position::left);
            if (isOnSide(cubidCoordinates, Position::down)) {
                cube.rotateSide(Position::front, currentSide != Position::left);
            }
        }
        // At this point the cubid is at up-front edge [1][0][0]
        // TODO: get rid of these constant coordinates
        if (cube.getCubids()[1][0][0].getColor(Position::front) == cube.getCenterColor(Position::front)) {
            cube.rotateSide(Position::front, false);
            cube.rotateSide(Position::front, false);
        } else {
            cube.rotateSide(Position::front, false);
            cube.rotateSide(Position::down, false);
            cube.rotateSide(Position::right, true);
            cube.rotateSide(Position::down, true);
        }
        // TODO: Implement relative positions to avoid turning the cube
        cube.turn(true, false);
    }
}
