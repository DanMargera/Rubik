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
}

std::string RubikConstants::toString(const Color c)
{
    std::string retVal;
    switch (c)
    {
        case Color::blue:
            retVal = "\e[38;5;27m";
            break;
        case Color::green:
            retVal = "\e[38;5;118m";
            break;
        case Color::orange:
            retVal = "\e[38;5;208m";
            break;
        case Color::red:
            retVal = "\e[38;5;196m";
            break;
        case Color::yellow:
            retVal = "\e[38;5;226m";
            break;
        case Color::white:
            retVal = "\e[1m";
            break;
        case Color::none:
            retVal = "\e[38;5;165m";
            break;
        default:
            break;
    }
    return retVal + value(c) + "\e[0m";
}
