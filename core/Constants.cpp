#include "Constants.h"

#include <cassert>

using RubikConstants::Position;
using RubikConstants::Color;
using RubikConstants::Coordinates;

int RubikConstants::value(const Position p)
{
    return static_cast<int>(p);
}

char RubikConstants::value(const Color c)
{
    return static_cast<char>(c);
}

template <typename OnSideFunc>
static Coordinates getCoordinates(OnSideFunc onSide)
{
    int x = onSide(Position::left) ? 0
                                   : onSide(Position::right) ? 2
                                                               : 1;
    int y = onSide(Position::up) ? 0
                                 : onSide(Position::down) ? 2
                                                          : 1;
    int z = onSide(Position::front) ? 0
                                    : onSide(Position::back) ? 2
                                                             : 1;
    return Coordinates(x,y,z);
}

Coordinates RubikConstants::edgeCoordinates(std::pair<Position, Position> sides)
{
    assert(sides.first != sides.second);
    auto hasSide = [&sides] (Position pos) {
        return sides.first == pos || sides.second == pos;
    };
    return getCoordinates(hasSide);
}

Coordinates RubikConstants::cornerCoordinates(std::tuple<Position, Position, Position> sides)
{
    auto hasSide = [&sides] (Position pos) {
        return std::get<0>(sides) == pos || std::get<1>(sides) == pos || std::get<2>(sides) == pos;
    };
    return getCoordinates(hasSide);
}

bool RubikConstants::isOnSide(const Coordinates& c, Position side)
{
    switch(side) {
        case Position::front:
            return c.z == 0;
        case Position::back:
            return c.z == 2;
        case Position::left:
            return c.x == 0;
        case Position::right:
            return c.x == 2;
        case Position::up:
            return c.y == 0;
        case Position::down:
            return c.y == 2;
    }
    return false;
}

std::string RubikConstants::moveToString(Position side, bool reverse)
{
    std::string sideStr;
    switch (side)
    {
        case Position::up:
            sideStr = "U";
            break;
        case Position::down:
            sideStr = "D";
            break;
        case Position::left:
            sideStr = "L";
            break;
        case Position::right:
            sideStr = "R";
            break;
        case Position::front:
            sideStr = "F";
            break;
        case Position::back:
            sideStr = "B";
            break;
    }
    return (reverse ? "-" : "") + sideStr;
}

bool Coordinates::operator==(const Coordinates& other) const noexcept
{
    return x == other.x && y == other.y && z == other.z;
}

bool Coordinates::operator!=(const Coordinates& other) const noexcept
{
    return x != other.x || y != other.y || z != other.z;
}
