#ifndef RUBIK_CONSTANTS_H
#define RUBIK_CONSTANTS_H

#include <array>
#include <map>
#include <string>

namespace RubikConstants
{

enum class Position
{
    up=0,
    down=1,
    left=2,
    right=3,
    front=4,
    back=5
};

enum class Color
{
    blue='b',
    green='g',
    orange='o',
    red='r',
    yellow='y',
    white='w',
    none='n'
};

int value(const Position p);

char value(const Color c);

std::string toString(const Color c);

using Axis = std::array<Position, 4>;
static constexpr Axis s_XY_axis{Position::left,
    Position::down,
    Position::right,
    Position::up};

static constexpr Axis s_XY_axis_r{Position::left,
    Position::up,
    Position::right,
    Position::down};

static constexpr Axis s_XZ_axis{Position::left,
    Position::front,
    Position::right,
    Position::back};

static constexpr Axis s_XZ_axis_r{Position::left,
    Position::back,
    Position::right,
    Position::front};

static constexpr Axis s_YZ_axis{Position::front,
    Position::down,
    Position::back,
    Position::up};

static constexpr Axis s_YZ_axis_r{Position::front,
    Position::up,
    Position::back,
    Position::down};

static std::map<Position, Axis> s_axisMap {
    {Position::left, s_YZ_axis_r},
    {Position::right, s_YZ_axis},
    {Position::up, s_XZ_axis},
    {Position::down, s_XZ_axis_r},
    {Position::front, s_XY_axis},
    {Position::back, s_XY_axis_r}
};

static std::map<Position, std::array<int,4>> s_cornerIndexes {
    {Position::left, {2,8,6,0}},
    {Position::right, {0,6,8,2}},
    {Position::up, {0,6,8,2}},
    {Position::down, {2,8,6,0}},
    {Position::front, {2,8,6,0}},
    {Position::back, {0,6,8,2}}
};

static std::map<Position, std::array<int,4>> s_edgeIndexes {
    {Position::left, {5,7,3,1}},
    {Position::right, {1,3,7,5}},
    {Position::up, {1,3,7,5}},
    {Position::down, {5,7,3,1}},
    {Position::front, {5,7,3,1}},
    {Position::back, {1,3,7,5}}
};

static std::map<Position, Color> s_defaultSideColors {
    {Position::left, Color::blue},
    {Position::right, Color::green},
    {Position::up, Color::yellow},
    {Position::down, Color::white},
    {Position::front, Color::red},
    {Position::back, Color::orange}
};

}

#endif
