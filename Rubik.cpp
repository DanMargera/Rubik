#include "Rubik.h"

#include <iostream>
#include <stdlib.h>

using namespace RubikConstants;

Cubid& Cubid::operator=(Cubid&& other) noexcept {
    if (this != &other) {
        m_faces = std::move(other.getFaces());
    }
    return *this;
}

void
Cubid::rotate(const Axis& axis, bool reverse)
{
    auto retrieve = [] (std::vector<Color>& vec, Position p) -> Color& {
        return vec[value(p)];
    };
    circularShift(m_faces, axis, retrieve, reverse);
}

std::vector<Coordinates>
RubikCube::findCubids(std::set<Color> colors, CubidType t)
{
    std::vector<Coordinates> retVal;
    for (size_t x=0; x<3; ++x) {
        for (size_t y=0; y<3; ++y) {
            for (size_t z=0; z<3; ++z) {
                int colorsMatched = 0;
                int colorCount = 0;
                auto& faces = m_cubids[x][y][z].getFaces();
                for (auto face : faces) {
                    if (face != Color::none) ++colorCount;
                    colorsMatched += colors.count(face);
                }
                CubidType type = static_cast<CubidType>(colorCount);

                if (colorsMatched == colors.size() && (t == type || t == CubidType::any)) {
                    retVal.emplace_back(Coordinates(x,y,z));
                }
            }
        }
    }
    return retVal;
}

void
RubikCube::turn(bool horizontal, bool reverse)
{
    std::vector<Cubid*> middleCubids;
    for (int i=0; i<3; ++i) {
        for (int j=0; j<3; ++j) {
            middleCubids.emplace_back(&m_cubids[horizontal ? i : 1][horizontal ? 1 : i][j]);
        }
    }
    // TODO: Avoid this code duplication
    auto retrieve = [] (std::vector<Cubid*>& vec, int i) -> Cubid& { return *vec[i]; };
    circularShift(middleCubids, std::array<int, 4>({0,2,8,6}), retrieve, reverse);
    circularShift(middleCubids, std::array<int, 4>({1,5,7,3}), retrieve, reverse);
    for (auto* cubid : middleCubids) {
        cubid->rotate(horizontal ? s_XZ_axis : s_YZ_axis, !reverse);
    }
    rotateSide(horizontal ? Position::up : Position::right, !reverse);
    rotateSide(horizontal ? Position::down : Position::left, reverse);
}

void
RubikCube::rotateSide(Position cubeSide, bool reverse)
{
    auto retrieve = [] (std::vector<Cubid*>& vec, int i) -> Cubid& { return *vec[i]; };
    circularShift(m_sides[value(cubeSide)], s_cornerIndexes.at(cubeSide), retrieve, reverse);
    circularShift(m_sides[value(cubeSide)], s_edgeIndexes.at(cubeSide), retrieve, reverse);

    auto rotate = [reverse] (Cubid* cubid, const Axis& axis) {
        cubid->rotate(axis, reverse);
    };

    applyTransformToSide(rotate, cubeSide, s_axisMap.at(cubeSide));
    ++m_moveCount;
    if (m_verboseMoves) {
        std::cout << moveToString(cubeSide, reverse) << " ";
    }
}

void
RubikCube::setupFaces()
{
    auto addToFace = [this] (Cubid& c, Position p) {
        m_sides[value(p)].emplace_back(&c);
    };
    for (int x=0; x<m_cubids.size(); ++x) {
        for (int y=0; y<m_cubids.size(); ++y) {
            for (int z=0; z<m_cubids.size(); ++z) {
                if (x == 0) addToFace(m_cubids[x][y][z], Position::left);
                if (x == 2) addToFace(m_cubids[x][y][z], Position::right);
                if (y == 0) addToFace(m_cubids[x][y][z], Position::up);
                if (y == 2) addToFace(m_cubids[x][y][z], Position::down);
                if (z == 0) addToFace(m_cubids[x][y][z], Position::front);
                if (z == 2) addToFace(m_cubids[x][y][z], Position::back);
            }
        }
    }

    auto setFaceColor = [] (Cubid* cubid, Position p, Color c) {
        cubid->setFaceColor(p, c);
    };

    for (const auto& posColor : s_defaultSideColors) {
        applyTransformToSide(setFaceColor, posColor.first, posColor.first, posColor.second);
    }
}

void
RubikCube::reset()
{
    m_cubids = std::move(std::array<std::array<std::array<Cubid, 3>, 3>, 3>());
    m_sides = std::move(std::array<std::vector<Cubid*>, 6>());
    m_moveCount = 0;
    m_verboseMoves = false;

    setupFaces();
}

RubikCube::RubikCube()
{
    setupFaces();
}

void
RubikCube::toggleZoom()
{
    m_zoom = !m_zoom;
}

void
RubikCube::shuffle()
{
    srand (time(NULL));
    int moves = rand() % 20 + 15;
    for (int i=0; i<moves; ++i) {
        rotateSide(static_cast<Position>(rand()%6), rand()%1);
    }
}

bool
RubikCube::isSolved()
{
    for (int p=0; p<numPositions; ++p) {
        Position pos = static_cast<Position>(p);
        for (const auto* cubid : m_sides[p]) {
            if (cubid->getColor(pos) != getCenterColor(pos)) return false;
        }
    }
    return true;
}