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
}

RubikCube::RubikCube()
{
    setupFaces();

    auto setFaceColor = [] (Cubid* cubid, Position p, Color c) {
        cubid->setFaceColor(p, c);
    };

    for (const auto& posColor : s_defaultSideColors) {
        applyTransformToSide(setFaceColor, posColor.first, posColor.first, posColor.second);
    }
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

void
RubikCube::print()
{
    std::string u1 = toString(m_cubids[0][0][0].getColor(Position::up));
    std::string u2 = toString(m_cubids[1][0][0].getColor(Position::up));
    std::string u3 = toString(m_cubids[2][0][0].getColor(Position::up));
    std::string u4 = toString(m_cubids[0][0][1].getColor(Position::up));
    std::string u5 = toString(m_cubids[1][0][1].getColor(Position::up));
    std::string u6 = toString(m_cubids[2][0][1].getColor(Position::up));
    std::string u7 = toString(m_cubids[0][0][2].getColor(Position::up));
    std::string u8 = toString(m_cubids[1][0][2].getColor(Position::up));
    std::string u9 = toString(m_cubids[2][0][2].getColor(Position::up));

    std::string r1 = toString(m_cubids[2][2][0].getColor(Position::right));
    std::string r2 = toString(m_cubids[2][2][1].getColor(Position::right));
    std::string r3 = toString(m_cubids[2][2][2].getColor(Position::right));
    std::string r4 = toString(m_cubids[2][1][0].getColor(Position::right));
    std::string r5 = toString(m_cubids[2][1][1].getColor(Position::right));
    std::string r6 = toString(m_cubids[2][1][2].getColor(Position::right));
    std::string r7 = toString(m_cubids[2][0][0].getColor(Position::right));
    std::string r8 = toString(m_cubids[2][0][1].getColor(Position::right));
    std::string r9 = toString(m_cubids[2][0][2].getColor(Position::right));

    std::string f1 = toString(m_cubids[0][2][0].getColor(Position::front));
    std::string f2 = toString(m_cubids[1][2][0].getColor(Position::front));
    std::string f3 = toString(m_cubids[2][2][0].getColor(Position::front));
    std::string f4 = toString(m_cubids[0][1][0].getColor(Position::front));
    std::string f5 = toString(m_cubids[1][1][0].getColor(Position::front));
    std::string f6 = toString(m_cubids[2][1][0].getColor(Position::front));
    std::string f7 = toString(m_cubids[0][0][0].getColor(Position::front));
    std::string f8 = toString(m_cubids[1][0][0].getColor(Position::front));
    std::string f9 = toString(m_cubids[2][0][0].getColor(Position::front));

    std::string l1 = toString(m_cubids[0][2][2].getColor(Position::left));
    std::string l2 = toString(m_cubids[0][2][1].getColor(Position::left));
    std::string l3 = toString(m_cubids[0][2][0].getColor(Position::left));
    std::string l4 = toString(m_cubids[0][1][2].getColor(Position::left));
    std::string l5 = toString(m_cubids[0][1][1].getColor(Position::left));
    std::string l6 = toString(m_cubids[0][1][0].getColor(Position::left));
    std::string l7 = toString(m_cubids[0][0][2].getColor(Position::left));
    std::string l8 = toString(m_cubids[0][0][1].getColor(Position::left));
    std::string l9 = toString(m_cubids[0][0][0].getColor(Position::left));

    std::string d1 = toString(m_cubids[2][2][0].getColor(Position::down));
    std::string d2 = toString(m_cubids[1][2][0].getColor(Position::down));
    std::string d3 = toString(m_cubids[0][2][0].getColor(Position::down));
    std::string d4 = toString(m_cubids[2][2][1].getColor(Position::down));
    std::string d5 = toString(m_cubids[1][2][1].getColor(Position::down));
    std::string d6 = toString(m_cubids[0][2][1].getColor(Position::down));
    std::string d7 = toString(m_cubids[2][2][2].getColor(Position::down));
    std::string d8 = toString(m_cubids[1][2][2].getColor(Position::down));
    std::string d9 = toString(m_cubids[0][2][2].getColor(Position::down));

    std::string b1 = toString(m_cubids[2][2][2].getColor(Position::back));
    std::string b2 = toString(m_cubids[1][2][2].getColor(Position::back));
    std::string b3 = toString(m_cubids[0][2][2].getColor(Position::back));
    std::string b4 = toString(m_cubids[2][1][2].getColor(Position::back));
    std::string b5 = toString(m_cubids[1][1][2].getColor(Position::back));
    std::string b6 = toString(m_cubids[0][1][2].getColor(Position::back));
    std::string b7 = toString(m_cubids[2][0][2].getColor(Position::back));
    std::string b8 = toString(m_cubids[1][0][2].getColor(Position::back));
    std::string b9 = toString(m_cubids[0][0][2].getColor(Position::back));

    std::string y = toString(Color::yellow);
    std::string r = toString(Color::red);
    std::string g = toString(Color::green);
    std::string b = toString(Color::blue);
    std::string w = toString(Color::white);

    std::cout << "                                               _____________________ \n"
              << "                                              /"<<u7<<""<<u7<<""<<u7<<""<<u7<<""<<u7<<""<<u7<<"/"<<u8<<""<<u8<<""<<u8<<""<<u8<<""<<u8<<""<<u8<<"/"<<u9<<""<<u9<<""<<u9<<""<<u9<<""<<u9<<""<<u9<<"/|\n"
              << "                                             /"<<u7<<""<<u7<<""<<u7<<""<<u7<<""<<u7<<""<<u7<<"/"<<u8<<""<<u8<<""<<u8<<""<<u8<<""<<u8<<""<<u8<<"/"<<u9<<""<<u9<<""<<u9<<""<<u9<<""<<u9<<""<<u9<<"/"<<r9<<"|\n"
              << "                                            /______/______/______/"<<r9<<""<<r9<<"|\n"
              << "                                           /"<<u4<<""<<u4<<""<<u4<<""<<u4<<""<<u4<<""<<u4<<"/"<<u5<<u5<<u5<<u5<<u5<<u5<<"/"<<u6<<""<<u6<<""<<u6<<""<<u6<<""<<u6<<""<<u6<<"/|"<<r9<<""<<r9<<"|\n"


              << "        _____________________       " << "      /"<<u4<<""<<u4<<""<<u4<<""<<u4<<""<<u4<<""<<u4<<"/"<<u5<<u5<<u5<<u5<<u5<<u5<<"/"<<u6<<""<<u6<<""<<u6<<""<<u6<<""<<u6<<""<<u6<<"/"<<r8<<"|"<<r9<<"/|\n"
              << "       /|"<<f7<<""<<f7<<""<<f7<<""<<f7<<""<<f7<<""<<f7<<"|"<<f8<<""<<f8<<""<<f8<<""<<f8<<""<<f8<<""<<f8<<"|"<<f9<<""<<f9<<""<<f9<<""<<f9<<""<<f9<<""<<f9<<"|      " << "     /______/______/______/"<<r8<<""<<r8<<"|/"<<r6<<"|\n"
              << "      /"<<l9<<"|"<<f7<<""<<f7<<""<<f7<<""<<f7<<""<<f7<<""<<f7<<"|"<<f8<<""<<f8<<""<<f8<<""<<f8<<""<<f8<<""<<f8<<"|"<<f9<<""<<f9<<""<<f9<<""<<f9<<""<<f9<<""<<f9<<"|      " << "    /"<<u1<<""<<u1<<""<<u1<<""<<u1<<""<<u1<<""<<u1<<"/"<<u2<<""<<u2<<""<<u2<<""<<u2<<""<<u2<<""<<u2<<"/"<<u3<<""<<u3<<""<<u3<<""<<u3<<""<<u3<<""<<u3<<"/|"<<r8<<""<<r8<<"|"<<r6<<""<<r6<<"|\n"
              << "     /|"<<l9<<"|______|______|______|      " << "   /"<<u1<<""<<u1<<""<<u1<<""<<u1<<""<<u1<<""<<u1<<"/"<<u2<<""<<u2<<""<<u2<<""<<u2<<""<<u2<<""<<u2<<"/"<<u3<<""<<u3<<""<<u3<<""<<u3<<""<<u3<<""<<u3<<"/"<<r7<<"|"<<r8<<"/|"<<r6<<"/|\n"
              << "    /"<<l8<<"|/|"<<f4<<""<<f4<<""<<f4<<""<<f4<<""<<f4<<""<<f4<<"|"<<f5<<""<<f5<<""<<f5<<""<<f5<<""<<f5<<""<<f5<<"|"<<f6<<""<<f6<<""<<f6<<""<<f6<<""<<f6<<""<<f6<<"|      " << "  /______/______/______/"<<r7<<""<<r7<<"|/"<<r5<<"|/"<<r3<<"|\n"
              << "   /|"<<l8<<"/"<<l6<<"|"<<f4<<""<<f4<<""<<f4<<""<<f4<<""<<f4<<""<<f4<<"|"<<f5<<""<<f5<<""<<f5<<""<<f5<<""<<f5<<""<<f5<<"|"<<f6<<""<<f6<<""<<f6<<""<<f6<<""<<f6<<""<<f6<<"|      " << "  |"<<f7<<""<<f7<<""<<f7<<""<<f7<<""<<f7<<""<<f7<<"|"<<f8<<""<<f8<<""<<f8<<""<<f8<<""<<f8<<""<<f8<<"|"<<f9<<""<<f9<<""<<f9<<""<<f9<<""<<f9<<""<<f9<<"|"<<r7<<""<<r7<<"|"<<r5<<r5<<"|"<<r3<<""<<r3<<"|\n"
              << "  /"<<l7<<"|/|"<<l6<<"|______|______|______|      " << "  |"<<f7<<""<<f7<<""<<f7<<""<<f7<<""<<f7<<""<<f7<<"|"<<f8<<""<<f8<<""<<f8<<""<<f8<<""<<f8<<""<<f8<<"|"<<f9<<""<<f9<<""<<f9<<""<<f9<<""<<f9<<""<<f9<<"|"<<r7<<"/|"<<r5<<"/|"<<r3<<"/\n"
              << "  |"<<l7<<"/"<<l5<<"|/|"<<f1<<""<<f1<<""<<f1<<""<<f1<<""<<f1<<""<<f1<<"|"<<f2<<""<<f2<<""<<f2<<""<<f2<<""<<f2<<""<<f2<<"|"<<f3<<""<<f3<<""<<f3<<""<<f3<<""<<f3<<""<<f3<<"|      " << "  |______|______|______|/"<<r4<<"|/"<<r2<<"|/\n"
              << "  |/|"<<l5<<"/"<<l3<<"|"<<f1<<""<<f1<<""<<f1<<""<<f1<<""<<f1<<""<<f1<<"|"<<f2<<""<<f2<<""<<f2<<""<<f2<<""<<f2<<""<<f2<<"|"<<f3<<""<<f3<<""<<f3<<""<<f3<<""<<f3<<""<<f3<<"|      " << "  |"<<f4<<""<<f4<<""<<f4<<""<<f4<<""<<f4<<""<<f4<<"|"<<f5<<f5<<f5<<f5<<f5<<f5<<"|"<<f6<<""<<f6<<""<<f6<<""<<f6<<""<<f6<<""<<f6<<"|"<<r4<<""<<r4<<"|"<<r2<<""<<r2<<"|\n"
              << "  |"<<l4<<"|/|"<<l3<<"|______|______|______|      " << "  |"<<f4<<""<<f4<<""<<f4<<""<<f4<<""<<f4<<""<<f4<<"|"<<f5<<f5<<f5<<f5<<f5<<f5<<"|"<<f6<<""<<f6<<""<<f6<<""<<f6<<""<<f6<<""<<f6<<"|"<<r4<<"/|"<<r2<<"/\n"
              << "  |"<<l4<<"/"<<l2<<"|/"<<d3<<""<<d3<<""<<d3<<""<<d3<<""<<d3<<""<<d3<<"/"<<d2<<""<<d2<<""<<d2<<""<<d2<<""<<d2<<""<<d2<<"/"<<d1<<""<<d1<<""<<d1<<""<<d1<<""<<d1<<""<<d1<<"/       " << "  |______|______|______|/"<<r1<<"|/\n"
              << "  |/|"<<l2<<"/______/______/______/        " << "  |"<<f1<<""<<f1<<""<<f1<<""<<f1<<""<<f1<<""<<f1<<"|"<<f2<<""<<f2<<""<<f2<<""<<f2<<""<<f2<<""<<f2<<"|"<<f3<<""<<f3<<""<<f3<<""<<f3<<""<<f3<<""<<f3<<"|"<<r1<<""<<r1<<"|\n"
              << "  |"<<l1<<"|/"<<d6<<""<<d6<<""<<d6<<""<<d6<<""<<d6<<""<<d6<<"/"<<d5<<""<<d5<<""<<d5<<""<<d5<<""<<d5<<""<<d5<<"/"<<d4<<""<<d4<<""<<d4<<""<<d4<<""<<d4<<""<<d4<<"/         " << "  |"<<f1<<""<<f1<<""<<f1<<""<<f1<<""<<f1<<""<<f1<<"|"<<f2<<""<<f2<<""<<f2<<""<<f2<<""<<f2<<""<<f2<<"|"<<f3<<""<<f3<<""<<f3<<""<<f3<<""<<f3<<""<<f3<<"|"<<r1<<"/\n"
              << "  |"<<l1<<"/------/------/------/          " << "  |______|______|______|/\n"

              << "  |/"<<d9<<""<<d9<<""<<d9<<""<<d9<<""<<d9<<""<<d9<<"/"<<d8<<""<<d8<<""<<d8<<""<<d8<<""<<d8<<""<<d8<<"/"<<d7<<""<<d7<<""<<d7<<""<<d7<<""<<d7<<""<<d7<<"/           \n"
              << "   ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾             \n";

    if (false) {
        std::cout << "      /"<<u7<<"‾/"<<u8<<"‾/"<<u9<<"‾/|\n"
            << "     /__/__/__/"<<r9<<"|\n"
            << "    /"<<u4<<" /"<<u5<<" /"<<u6<<" /| |\n"
            << "   /__/__/__/"<<r8<<"|/|\n"
            << "  /"<<u1<<" /"<<u2<<" /"<<u3<<" /| |"<<r6<<"|\n"
            << " /__/__/__/"<<r7<<"|/|/|\n"
            << " |"<<f7<<" |"<<f8<<" |"<<f9<<" | |"<<r5<<"|"<<r3<<"|\n"
            << " |__|__|__|/|/|/ \n"
            << " |"<<f4<<" |"<<f5<<" |"<<f6<<" |"<<r4<<"|"<<r2<<"|  \n"
            << " |__|__|__|/|/   \n"
            << " |"<<f1<<" |"<<f2<<" |"<<f3<<" |"<<r1<<"|    \n"
            << " |__|__|__|/     \n";
    }
}
