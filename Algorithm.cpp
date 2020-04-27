#include "Algorithm.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>

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

        Position operator()(Position pos) const noexcept { return m_relativeMap.at(pos); }

        Position up() { return m_relativeMap.at(Position::up); }
        Position down() { return m_relativeMap.at(Position::down); }
        Position left() { return m_relativeMap.at(Position::left); }
        Position right() { return m_relativeMap.at(Position::right); }
        Position front() { return m_relativeMap.at(Position::front); }
        Position back() { return m_relativeMap.at(Position::back); }

    private:
        std::map<Position, Position> m_relativeMap;

};

// TODO: move operations and sequence to constants/rubik
namespace operations
{
    // Clockwise rotations
    static const int R=1, L=2, F=3, B=4, U=5, D=6;
    // Counter clockwise
    static const int _R=-1, _L=-2, _F=-3, _B=-4, _U=-5, _D=-6;
    // Half-turns
    static const int R2=11, L2=12, F2=13, B2=14, U2=15, D2=16;
};

Position pos(int op)
{
    using namespace operations;
    using std::abs;
    return (abs(op)%10 == R) ? Position::right
         : (abs(op)%10 == L) ? Position::left
         : (abs(op)%10 == F) ? Position::front
         : (abs(op)%10 == B) ? Position::back
         : (abs(op)%10 == U) ? Position::up
         : Position::down;
}

bool clockwise(int op)
{
    return op > 0;
}

bool isHalfTurn(int op)
{
    return op > 10;
}

void moveSequence(RubikCube& cube, RelativeCubeView& relative, std::vector<int> ops)
{
    static const auto exec = [] (RubikCube& c, RelativeCubeView& r, int op) {
        c.rotateSide(r(pos(op)), !clockwise(op));
        if (isHalfTurn(op)) {
            c.rotateSide(r(pos(op)), !clockwise(op));
        }
    };
    for (auto op : ops) {
        exec(cube, relative, op);
    }
}
} // namespace

bool Algorithm::bruteSolve(RubikCube& cube, int maxDepth, int lastPos)
{
    if (maxDepth == 0) return false;
    for (int i=0; i<6; ++i) {
        if (i == lastPos) continue;
        cube.rotateSide(static_cast<Position>(i), false);
        if (cube.isSolved() || bruteSolve(cube, maxDepth-1, i)) return true;
        cube.rotateSide(static_cast<Position>(i), false);
        if (cube.isSolved() || bruteSolve(cube, maxDepth-1, i)) return true;
        cube.rotateSide(static_cast<Position>(i), false);
        if (cube.isSolved() || bruteSolve(cube, maxDepth-1, i)) return true;
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
    using namespace operations;
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
            moveSequence(cube, relative, {F2}); // TODO: Call F2 directly when half-turn is implemented
        } else {
            moveSequence(cube, relative, {F, D, -R, -D});
        }
    }
}

void Algorithm::downCorners(RubikCube& cube)
{
    using namespace operations;
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
                moveSequence(cube, relative, {F, U, -F, -U});
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
            moveSequence(cube, relative, {F, R, U2, -R, -F});
        } else {
            auto currentSide = cubid.getColor(relative.front()) == cube.getCenterColor(relative.down()) ? relative.front()
                                                                                                        : relative.left();
            cube.rotateSide(currentSide, currentSide == relative.left());
            cube.rotateSide(relative.up(), currentSide == relative.left());
            cube.rotateSide(currentSide, currentSide != relative.left());
        }
    }
}

void Algorithm::middleLayer(RubikCube& cube)
{
    using namespace operations;
    RelativeCubeView relative;

    for (auto side : horizontalSidePositions) {
        relative.setFrontView(side);
        // Locate left-front edge cubid
        auto cubidCoordinates = cube.findCubids({cube.getCenterColor(relative.left()),
                                                 cube.getCenterColor(relative.front())},
                                                 CubidType::edge)[0];
        if (isOnSide(cubidCoordinates, relative.down())) {
            // Error, should not solve middle layer before bottom layer
            return;
        }
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
            moveSequence(cube, relative, {U, R, -U, -R, -U, -F, U, F});
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
            moveSequence(cube, relative, {U, R, -U, -R, -U, -F, U, F});
        } else {
            moveSequence(cube, relative, {-U, -L, U, L, U, F, -U, -F});
        }
    }
}

void Algorithm::topCross(RubikCube& cube)
{
    using namespace operations;
    RelativeCubeView relative;

    auto& cubidL = cube.getCubid(edgeCoordinates({relative.up(), relative.left()}));
    auto& cubidR = cube.getCubid(edgeCoordinates({relative.up(), relative.right()}));
    auto& cubidF = cube.getCubid(edgeCoordinates({relative.up(), relative.front()}));
    auto& cubidB = cube.getCubid(edgeCoordinates({relative.up(), relative.back()}));

    auto upColor = cube.getCenterColor(relative.up());

    bool left = cubidL.getColor(relative.up()) == upColor;
    bool right = cubidR.getColor(relative.up()) == upColor;
    bool front = cubidF.getColor(relative.up()) == upColor;
    bool back = cubidB.getColor(relative.up()) == upColor;

    if (left && right && front && back) {
        return;
    }

    if (!left && !front && !right) {
        // Dot shape, execute once to form "L"
        moveSequence(cube, relative, {F, R, U, -R, -U, -F});
    }

    left = cubidL.getColor(relative.up()) == upColor;
    right = cubidR.getColor(relative.up()) == upColor;
    front = cubidF.getColor(relative.up()) == upColor;
    back = cubidB.getColor(relative.up()) == upColor;

    if ((left && right) || (front && back)) {
        // Cubids form a line on top, execute once
        relative.setFrontView(!front ? relative.front() : relative.right());
        moveSequence(cube, relative, {F, R, U, -R, -U, -F});
    } else if (left || front || right) {
        // Cubids form an "L" shape, execute twice
        auto relativeSide = left ? (front ? relative.right() : relative.front())
                          : front ? relative.back()
                          : relative.left();
        relative.setFrontView(relativeSide);
        moveSequence(cube, relative, {F, R, U, -R, -U, -F});
        moveSequence(cube, relative, {F, R, U, -R, -U, -F});
    } else {
        assert(0);
    }
}

void Algorithm::topFace(RubikCube& cube)
{
    using namespace operations;
    RelativeCubeView relative;

    std::array<Cubid*, 4> corners{
        &cube.getCubid(cornerCoordinates({relative.up(), relative.left(), relative.front()})),
        &cube.getCubid(cornerCoordinates({relative.up(), relative.left(), relative.back()})),
        &cube.getCubid(cornerCoordinates({relative.up(), relative.right(), relative.front()})),
        &cube.getCubid(cornerCoordinates({relative.up(), relative.right(), relative.back()}))
    };

    auto upColor = cube.getCenterColor(relative.up());

    auto count = [&relative,&corners,upColor] () {
        int c = 0;
        for (auto* cubid : corners) {
            if (cubid->getColor(relative.up()) == upColor) c++;
        }
        return c;
    };

    int solved = count();

    auto& cubidLF = *corners[0];
    while (solved != 4) { //!all colors in top
        if (solved == 0) {
            while (cubidLF.getColor(relative.left()) != upColor) {
                cube.rotateSide(relative.up(), false);
            }
        } else if (solved == 1) {
            while (cubidLF.getColor(relative.up()) != upColor) {
                cube.rotateSide(relative.up(), false);
            }
        } else {
            while (cubidLF.getColor(relative.front()) != upColor) {
                cube.rotateSide(relative.up(), false);
            }
        }
        moveSequence(cube, relative, {R, U, -R, U, R, U2, -R});
        solved = count();
    }
}

void Algorithm::topLayer(RubikCube& cube)
{
    using namespace operations;
    RelativeCubeView relative;

    std::array<Cubid*, 4> corners{
        &cube.getCubid(cornerCoordinates({relative.up(), relative.left(), relative.front()})),
        &cube.getCubid(cornerCoordinates({relative.up(), relative.left(), relative.back()})),
        &cube.getCubid(cornerCoordinates({relative.up(), relative.right(), relative.front()})),
        &cube.getCubid(cornerCoordinates({relative.up(), relative.right(), relative.back()}))
    };

    auto countMatchingSides = [&relative,&corners] () {
        int c = 0;
        c += corners[0]->getColor(relative.left()) == corners[1]->getColor(relative.left());
        c += corners[2]->getColor(relative.right()) == corners[3]->getColor(relative.right());
        c += corners[0]->getColor(relative.front()) == corners[2]->getColor(relative.front());
        c += corners[1]->getColor(relative.back()) == corners[3]->getColor(relative.back());
        return c;
    };

    int solved = countMatchingSides();

    auto& cubidLF = *corners[0];
    while (solved != 4) {
        if (solved == 0) {
            // Diagonals, rotate until something matches with the front
            while (corners[0]->getColor(relative.front()) != cube.getCenterColor(relative.front()) &&
                   corners[2]->getColor(relative.front()) != cube.getCenterColor(relative.front())) {
                cube.rotateSide(relative.up(), false);
            }
        } else {
            // Rotate until matching side is at the back
            while (corners[1]->getColor(relative.back()) != corners[3]->getColor(relative.back())) {
                cube.rotateSide(relative.up(), false);
            }
        }
        moveSequence(cube, relative, {-R, F, -R, B2, R, -F, -R, B2, R2, -U});
        solved = countMatchingSides();
    }
}

void Algorithm::layerSolve(RubikCube& cube)
{
    downCross(cube);
    downCorners(cube);
    middleLayer(cube);
    topCross(cube);
    topFace(cube);
    topLayer(cube);
}
