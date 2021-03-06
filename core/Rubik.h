#ifndef RUBIK_H
#define RUBIK_H

#include "Constants.h"

#include <array>
#include <set>
#include <vector>

using namespace RubikConstants;

template <typename Container, typename IndexContainer, typename RetrieveFunc>
void circularShift(Container& c, const IndexContainer& indexC, RetrieveFunc retrieve, bool reverse)
{
    auto it = reverse ? indexC.end() : indexC.begin();
    auto endIt = reverse ? indexC.begin() : indexC.end();
    if (reverse) { it--; } else { endIt--; }
    auto temp(std::move(retrieve(c, *it)));
    for (; it != endIt; reverse ? --it : ++it) {
        retrieve(c, *it) = std::move(retrieve(c, reverse ? *std::prev(it) : *std::next(it)));
    }
    retrieve(c, *endIt) = std::move(temp);
}

class Cubid
{
    public:
        Cubid() = default;
        Cubid(Cubid&& c) : m_faces(std::move(c.getFaces())) { }
        Cubid& operator=(Cubid&& other) noexcept;

        // Delete copy
        Cubid(const Cubid& c) = delete;
        Cubid& operator=(const Cubid& c) = delete;

        void setFaceColor(Position facePos, Color c) { m_faces[value(facePos)] = c; }
        Color getColor(Position pos) const { return m_faces.at(value(pos)); }

        void rotate(const Axis& axis, bool reverse);

        std::vector<Color>& getFaces() { return m_faces; }

    private:
        std::vector<Color> m_faces{6, Color::none};
};

class RubikCube
{
    public:
        RubikCube();

        void print();

        void rotateSide(Position cubeSide, bool reverse);

        std::vector<Coordinates> findCubids(std::set<Color> colors, CubidType t = CubidType::any);

        void turn(bool horizontal, bool reverse);

        void shuffle();

        bool isSolved();

        Color getCenterColor(Position side) { return m_sides[value(side)][4]->getColor(side); }

        template <typename TransformCubidFunc, typename... Args>
        void applyTransformToSide(TransformCubidFunc tFunc, Position pos, Args... args) {
            for (Cubid* c : m_sides[value(pos)]) {
                tFunc(c, args...);
            }
        }

        Cubid& getCubid(Coordinates c) { return m_cubids[c.x][c.y][c.z]; }

        std::array<std::array<std::array<Cubid, 3>, 3>, 3>& getCubids() { return m_cubids; }

        void setVerboseMoves(bool b) { m_verboseMoves = b; }

        void resetMoveCount() { m_moveCount = 0; }
        int getMoveCount() { return m_moveCount; }

        void reset();
        void toggleZoom();
        bool zoom() const { return m_zoom; };

    private:
        void setupFaces();

        std::array<std::array<std::array<Cubid, 3>, 3>, 3> m_cubids;
        std::array<std::vector<Cubid*>, 6> m_sides;

        int m_moveCount{0};
        bool m_verboseMoves{false};
        bool m_zoom{true};
};

#endif
