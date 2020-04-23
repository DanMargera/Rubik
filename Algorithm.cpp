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
                m_relativeMap[horizontalSidePositions[i]] = horizontalSidePositions[(i+distance)%horizontalSidePositions.size()];
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

    for (auto side : horizontalSidePositions) {
        relative.setFrontView(side);
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

void Algorithm::downCorners(RubikCube& cube)
{
    RelativeCubeView relative;

    for (auto side : horizontalSidePositions) {
        relative.setFrontView(side);
        // Locate down-left-front corner
        auto cubidCoordinates = cube.findCubids({cube.getCenterColor(relative.down()),
                                                 cube.getCenterColor(relative.front()),
                                                 cube.getCenterColor(relative.left())},
                                                 CubidType::corner)[0];
        if (!isOnSide(cubidCoordinates, relative.up())) {
            if (cubidCoordinates == cornerCoordinates({relative.down(), relative.front(), relative.left()})) {
                // Cubid is in the correct position, check the orientation
                if (cube.getCubid(cubidCoordinates).getColor(relative.down()) == cube.getCenterColor(relative.down())) {
                    continue;
                }
                cube.rotateSide(relative.front(), false);
                cube.rotateSide(relative.up(), false);
                cube.rotateSide(relative.front(), true);
                cube.rotateSide(relative.up(), true);
            } else {
                Position currentSide = isOnSide(cubidCoordinates, relative.front()) ? relative.front()
                                     : isOnSide(cubidCoordinates, relative.right()) ? relative.right()
                                     : relative.back();
                cube.rotateSide(currentSide, true); // Bring cubid to up face
                cube.rotateSide(relative.up(), false);
                cube.rotateSide(relative.up(), false);
                cube.rotateSide(currentSide, false); // Undo
                if (cubidCoordinates != cornerCoordinates({relative.down(), relative.back(), relative.right()})) {
                    cube.rotateSide(relative.up(), isOnSide(cubidCoordinates, relative.right()));
                }
            }
        } else if (cubidCoordinates != cornerCoordinates({relative.up(), relative.front(), relative.left()})) {
            cube.rotateSide(relative.up(), isOnSide(cubidCoordinates, relative.left()));
            if (cubidCoordinates == cornerCoordinates({relative.up(), relative.back(), relative.right()})) {
                // It was on the opposite corner, so rotate once more
                cube.rotateSide(relative.up(), isOnSide(cubidCoordinates, relative.left()));
            }
        }
        // Now the cubid is positioned above it's correct position, at up-front-left corner
        auto& cubid = cube.getCubid(cornerCoordinates({relative.up(), relative.front(), relative.left()}));
        if (cubid.getColor(relative.up()) == cube.getCenterColor(relative.down())) {
            cube.rotateSide(relative.front(), false);
            cube.rotateSide(relative.right(), false);
            cube.rotateSide(relative.up(), false);
            cube.rotateSide(relative.up(), false);
            cube.rotateSide(relative.right(), true);
            cube.rotateSide(relative.front(), true);
        } else {
            auto currentSide = cubid.getColor(relative.front()) == cube.getCenterColor(relative.down()) ? relative.front()
                                                                                                        : relative.left();
            cube.rotateSide(currentSide, currentSide == relative.left());
            cube.rotateSide(relative.up(), currentSide == relative.left());
            cube.rotateSide(currentSide, currentSide != relative.left());
        }
    }
}

static void ururufuf(RubikCube& cube, RelativeCubeView& relative)
{
    // U R -U -R -U -F U F
    cube.rotateSide(relative.up(), false);
    cube.rotateSide(relative.right(), false);
    cube.rotateSide(relative.up(), true);
    cube.rotateSide(relative.right(), true);
    cube.rotateSide(relative.up(), true);
    cube.rotateSide(relative.front(), true);
    cube.rotateSide(relative.up(), false);
    cube.rotateSide(relative.front(), false);
}

void Algorithm::middleLayer(RubikCube& cube)
{
    RelativeCubeView relative;

    for (auto side : horizontalSidePositions) {
        relative.setFrontView(side);
        // Locate left-front edge cubid
        auto cubidCoordinates = cube.findCubids({cube.getCenterColor(relative.left()),
                                                 cube.getCenterColor(relative.front())},
                                                 CubidType::edge)[0];
        auto matchingSide = Position::up; // Assigning any value
        if (!isOnSide(cubidCoordinates, relative.up())) {
            // Cubid is in the correct position, check the orientation
            if (cubidCoordinates == edgeCoordinates({relative.left(), relative.front()}) &&
                    cube.getCubid(cubidCoordinates).getColor(relative.left()) == cube.getCenterColor(relative.left())) {
                continue;
            }
            // Cubid is trapped, need to bring it to the up side
            auto currentSide = isOnSide(cubidCoordinates, relative.front()) && !isOnSide(cubidCoordinates, relative.left()) ? relative.front()
                             : isOnSide(cubidCoordinates, relative.right()) ? relative.right()
                             : isOnSide(cubidCoordinates, relative.back()) ? relative.back()
                             : relative.left();
            relative.setFrontView(currentSide);
            ururufuf(cube, relative);
            // Ururufuf will untrap the cubid, and leave it at up-back edge
            auto& cubid = cube.getCubid(edgeCoordinates({relative.up(), relative.back()}));
            relative.setFrontView(side);
            matchingSide = cubid.getColor(relative.up()) == cube.getCenterColor(relative.front()) ?
                           relative.left() : relative.front();
        } else {
            matchingSide = cube.getCubid(cubidCoordinates).getColor(relative.up()) == cube.getCenterColor(relative.front()) ?
                           relative.left() : relative.front();
        }

        // Cubid is now somewhere on up side, need to align it with matchingSide
        auto& targetCubid = cube.getCubid(edgeCoordinates({matchingSide, relative.up()}));
        auto otherSide = matchingSide == relative.left() ? relative.front() : relative.left();
        // Rotate until the cubid is on the matching side
        while (targetCubid.getColor(matchingSide) != cube.getCenterColor(matchingSide) ||
               targetCubid.getColor(relative.up()) != cube.getCenterColor(otherSide)) {
            cube.rotateSide(relative.up(), false);
        }
        relative.setFrontView(matchingSide);

        // Cubid is now up-front on relative view
        auto& cubid = cube.getCubid(edgeCoordinates({relative.up(), relative.front()}));
        if (cubid.getColor(relative.up()) == cube.getCenterColor(relative.right())) {
            // U -R -U -R -U -F U F
            ururufuf(cube, relative);
        } else {
            // -U -L U L U F -U -F
            cube.rotateSide(relative.up(), true);
            cube.rotateSide(relative.left(), true);
            cube.rotateSide(relative.up(), false);
            cube.rotateSide(relative.left(), false);
            cube.rotateSide(relative.up(), false);
            cube.rotateSide(relative.front(), false);
            cube.rotateSide(relative.up(), true);
            cube.rotateSide(relative.front(), true);
        }
    }
}

void Algorithm::layerSolve(RubikCube& cube)
{
    downCross(cube);
    downCorners(cube);
    middleLayer(cube);
}
