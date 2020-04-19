#ifndef RUBIK_H
#define RUBIK_H

#include "Constants.h"

#include <array>
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

        void shuffle();

        bool isSolved();

        bool bruteSolve(int maxDepth);

        template <typename TransformCubidFunc, typename... Args>
        void applyTransformToSide(TransformCubidFunc tFunc, Position pos, Args... args) {
            for (Cubid* c : m_sides[value(pos)]) {
                tFunc(c, args...);
            }
        }

    private:
        void setupFaces();

        std::array<std::array<std::array<Cubid, 3>, 3>, 3> m_cubids;
        std::array<std::vector<Cubid*>, 6> m_sides;
};

#endif
