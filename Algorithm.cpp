#include "Algorithm.h"

#include <algorithm>
#include <cassert>

namespace
{
std::vector<Position> horizontalSidePositions{Position::front, Position::right, Position::back, Position::left};
// For now it's used only for horizontal views
// TODO: Implement views from any angle
class RelativeCubeView
{
    public:
        RelativeCubeView()
        {
            setFrontView(Position::front);
        }

        RelativeCubeView(Position frontView)
        {
            assert(frontView != Position::up && frontView != Position::down);
            setFrontView(frontView);
        }

        void setFrontView(Position frontView)
        {
            assert(frontView != Position::up && frontView != Position::down);
            const auto frontViewIt = std::find(horizontalSidePositions.begin(), horizontalSidePositions.end(), frontView);
            int distance = std::distance(horizontalSidePositions.begin(), frontViewIt);
            for (int i=0; i<horizontalSidePositions.size(); ++i) {
                m_relativeMap[horizontalSidePositions[(i+distance)%horizontalSidePositions.size()]] = horizontalSidePositions[i];
            }
            m_relativeMap[Position::up] = Position::up;
            m_relativeMap[Position::down] = Position::down;
        }

        Position up() { return m_relativeMap.at(Position::up); }
        Position down() { return m_relativeMap.at(Position::down); }
        Position left() { return m_relativeMap.at(Position::left); }
        Position right() { return m_relativeMap.at(Position::right); }
        Position front() { return m_relativeMap.at(Position::front); }
        Position back() { return m_relativeMap.at(Position::back); }

    private:
        std::map<Position, Position> m_relativeMap;

};
}

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
 * - Break down in smaller prefab algorithms with meaningful names
 * - Improve legibility, this is unreadable
 */
void Algorithm::downCross(RubikCube& cube)
{
    Color downColor = cube.getCenterColor(Position::down);

    RelativeCubeView relative;

    for (int i=0; i<horizontalSidePositions.size(); ++i) {
        relative.setFrontView(horizontalSidePositions[i]);
        // Find current location of down-front edge cubid
        auto cubidCoordinates = cube.findCubids({downColor, cube.getCenterColor(relative.front())}, CubidType::edge)[0];
        Position currentSide = isOnSide(cubidCoordinates, relative.left()) ? relative.left() : relative.right();
        if (!isOnSide(cubidCoordinates, relative.front())) {
            if (!isOnSide(cubidCoordinates, relative.up())) {
                if (!isOnSide(cubidCoordinates, relative.back())) {
                    // Bottom-right and bottom-left cases
                    cube.rotateSide(currentSide, false);
                    cube.rotateSide(currentSide, false);
                    cube.rotateSide(relative.up(), currentSide == relative.left());
                    cube.rotateSide(currentSide, false);
                    cube.rotateSide(currentSide, false);
                } else {
                    // Back-left, back-right and back-bottom cases
                    cube.rotateSide(relative.back(), currentSide == relative.left());
                    if (isOnSide(cubidCoordinates, relative.down())) {
                        cube.rotateSide(relative.back(), currentSide == relative.left());
                    }
                    cube.rotateSide(relative.up(), false);
                    cube.rotateSide(relative.up(), false);
                    cube.rotateSide(relative.back(), currentSide != relative.left());
                    if (isOnSide(cubidCoordinates, relative.down())) {
                        cube.rotateSide(relative.back(), currentSide != relative.left());
                    }
                }
            } else {
                // Up-left, up-right and up-back cases
                cube.rotateSide(relative.up(), currentSide == relative.left());
                if (isOnSide(cubidCoordinates, relative.back())) {
                    cube.rotateSide(relative.up(), currentSide == relative.left());
                }
            }
        } else if (!isOnSide(cubidCoordinates, relative.up())) {
            cube.rotateSide(relative.front(), currentSide != relative.left());
            if (isOnSide(cubidCoordinates, relative.down())) {
                cube.rotateSide(relative.front(), currentSide != relative.left());
            }
        }
        // Now the cubid is positioned at the relative upper-front position
        if (cube.getCubid(edgeCoordinates({relative.front(), relative.up()})).getColor(relative.front()) == cube.getCenterColor(relative.front())) {
            cube.rotateSide(relative.front(), false);
            cube.rotateSide(relative.front(), false);
        } else {
            cube.rotateSide(relative.front(), false);
            cube.rotateSide(relative.down(), false);
            cube.rotateSide(relative.right(), true);
            cube.rotateSide(relative.down(), true);
        }
    }
}
