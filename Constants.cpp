#include "Constants.h"

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
