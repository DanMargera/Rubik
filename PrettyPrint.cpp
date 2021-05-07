#include "PrettyPrint.h"
#include "Rubik.h"

#include <iostream>

pp::LeftView::LeftView(RubikCube& c)
{
}

pp::MainView::MainView(RubikCube& c)
{
}

void pp::printRubikCube(RubikCube& c)
{
    auto& cubids = c.getCubids();
    std::string u1 = toString(cubids[0][0][0].getColor(Position::up));
    std::string u2 = toString(cubids[1][0][0].getColor(Position::up));
    std::string u3 = toString(cubids[2][0][0].getColor(Position::up));
    std::string u4 = toString(cubids[0][0][1].getColor(Position::up));
    std::string u5 = toString(cubids[1][0][1].getColor(Position::up));
    std::string u6 = toString(cubids[2][0][1].getColor(Position::up));
    std::string u7 = toString(cubids[0][0][2].getColor(Position::up));
    std::string u8 = toString(cubids[1][0][2].getColor(Position::up));
    std::string u9 = toString(cubids[2][0][2].getColor(Position::up));

    std::string r1 = toString(cubids[2][2][0].getColor(Position::right));
    std::string r2 = toString(cubids[2][2][1].getColor(Position::right));
    std::string r3 = toString(cubids[2][2][2].getColor(Position::right));
    std::string r4 = toString(cubids[2][1][0].getColor(Position::right));
    std::string r5 = toString(cubids[2][1][1].getColor(Position::right));
    std::string r6 = toString(cubids[2][1][2].getColor(Position::right));
    std::string r7 = toString(cubids[2][0][0].getColor(Position::right));
    std::string r8 = toString(cubids[2][0][1].getColor(Position::right));
    std::string r9 = toString(cubids[2][0][2].getColor(Position::right));

    std::string f1 = toString(cubids[0][2][0].getColor(Position::front));
    std::string f2 = toString(cubids[1][2][0].getColor(Position::front));
    std::string f3 = toString(cubids[2][2][0].getColor(Position::front));
    std::string f4 = toString(cubids[0][1][0].getColor(Position::front));
    std::string f5 = toString(cubids[1][1][0].getColor(Position::front));
    std::string f6 = toString(cubids[2][1][0].getColor(Position::front));
    std::string f7 = toString(cubids[0][0][0].getColor(Position::front));
    std::string f8 = toString(cubids[1][0][0].getColor(Position::front));
    std::string f9 = toString(cubids[2][0][0].getColor(Position::front));

    std::string l1 = toString(cubids[0][2][2].getColor(Position::left));
    std::string l2 = toString(cubids[0][2][1].getColor(Position::left));
    std::string l3 = toString(cubids[0][2][0].getColor(Position::left));
    std::string l4 = toString(cubids[0][1][2].getColor(Position::left));
    std::string l5 = toString(cubids[0][1][1].getColor(Position::left));
    std::string l6 = toString(cubids[0][1][0].getColor(Position::left));
    std::string l7 = toString(cubids[0][0][2].getColor(Position::left));
    std::string l8 = toString(cubids[0][0][1].getColor(Position::left));
    std::string l9 = toString(cubids[0][0][0].getColor(Position::left));

    std::string d1 = toString(cubids[2][2][0].getColor(Position::down));
    std::string d2 = toString(cubids[1][2][0].getColor(Position::down));
    std::string d3 = toString(cubids[0][2][0].getColor(Position::down));
    std::string d4 = toString(cubids[2][2][1].getColor(Position::down));
    std::string d5 = toString(cubids[1][2][1].getColor(Position::down));
    std::string d6 = toString(cubids[0][2][1].getColor(Position::down));
    std::string d7 = toString(cubids[2][2][2].getColor(Position::down));
    std::string d8 = toString(cubids[1][2][2].getColor(Position::down));
    std::string d9 = toString(cubids[0][2][2].getColor(Position::down));

    std::string b1 = toString(cubids[2][2][2].getColor(Position::back));
    std::string b2 = toString(cubids[1][2][2].getColor(Position::back));
    std::string b3 = toString(cubids[0][2][2].getColor(Position::back));
    std::string b4 = toString(cubids[2][1][2].getColor(Position::back));
    std::string b5 = toString(cubids[1][1][2].getColor(Position::back));
    std::string b6 = toString(cubids[0][1][2].getColor(Position::back));
    std::string b7 = toString(cubids[2][0][2].getColor(Position::back));
    std::string b8 = toString(cubids[1][0][2].getColor(Position::back));
    std::string b9 = toString(cubids[0][0][2].getColor(Position::back));

    std::string y = toString(Color::yellow);
    std::string r = toString(Color::red);
    std::string g = toString(Color::green);
    std::string b = toString(Color::blue);
    std::string w = toString(Color::white);

    if (c.zoom()) {
        std::cout << "                                               _____________________ " << "          Rotation Commands:\n"
                << "                                              /"<<u7<<""<<u7<<""<<u7<<""<<u7<<""<<u7<<""<<u7<<"/"<<u8<<""<<u8<<""<<u8<<""<<u8<<""<<u8<<""<<u8<<"/"<<u9<<""<<u9<<""<<u9<<""<<u9<<""<<u9<<""<<u9<<"/|\n"
                << "                                             /"<<u7<<""<<u7<<""<<u7<<""<<u7<<""<<u7<<""<<u7<<"/"<<u8<<""<<u8<<""<<u8<<""<<u8<<""<<u8<<""<<u8<<"/"<<u9<<""<<u9<<""<<u9<<""<<u9<<""<<u9<<""<<u9<<"/"<<r9<<"|" << "          [R] [-R] [L] [-L]\n"
                << "                                            /______/______/______/"<<r9<<""<<r9<<"|\n"
                << "                                           /"<<u4<<""<<u4<<""<<u4<<""<<u4<<""<<u4<<""<<u4<<"/"<<u5<<u5<<u5<<u5<<u5<<u5<<"/"<<u6<<""<<u6<<""<<u6<<""<<u6<<""<<u6<<""<<u6<<"/|"<<r9<<""<<r9<<"|" << "          [F] [-F] [B] [-B]\n"


                << "        _____________________       " << "      /"<<u4<<""<<u4<<""<<u4<<""<<u4<<""<<u4<<""<<u4<<"/"<<u5<<u5<<u5<<u5<<u5<<u5<<"/"<<u6<<""<<u6<<""<<u6<<""<<u6<<""<<u6<<""<<u6<<"/"<<r8<<"|"<<r9<<"/|\n"
                << "       /|"<<f7<<""<<f7<<""<<f7<<""<<f7<<""<<f7<<""<<f7<<"|"<<f8<<""<<f8<<""<<f8<<""<<f8<<""<<f8<<""<<f8<<"|"<<f9<<""<<f9<<""<<f9<<""<<f9<<""<<f9<<""<<f9<<"|      " << "     /______/______/______/"<<r8<<""<<r8<<"|/"<<r6<<"|" << "          [U] [-U] [D] [-D]\n"
                << "      /"<<l9<<"|"<<f7<<""<<f7<<""<<f7<<""<<f7<<""<<f7<<""<<f7<<"|"<<f8<<""<<f8<<""<<f8<<""<<f8<<""<<f8<<""<<f8<<"|"<<f9<<""<<f9<<""<<f9<<""<<f9<<""<<f9<<""<<f9<<"|      " << "    /"<<u1<<""<<u1<<""<<u1<<""<<u1<<""<<u1<<""<<u1<<"/"<<u2<<""<<u2<<""<<u2<<""<<u2<<""<<u2<<""<<u2<<"/"<<u3<<""<<u3<<""<<u3<<""<<u3<<""<<u3<<""<<u3<<"/|"<<r8<<""<<r8<<"|"<<r6<<""<<r6<<"|\n"
                << "     /|"<<l9<<"|______|______|______|      " << "   /"<<u1<<""<<u1<<""<<u1<<""<<u1<<""<<u1<<""<<u1<<"/"<<u2<<""<<u2<<""<<u2<<""<<u2<<""<<u2<<""<<u2<<"/"<<u3<<""<<u3<<""<<u3<<""<<u3<<""<<u3<<""<<u3<<"/"<<r7<<"|"<<r8<<"/|"<<r6<<"/|\n"
                << "    /"<<l8<<"|/|"<<f4<<""<<f4<<""<<f4<<""<<f4<<""<<f4<<""<<f4<<"|"<<f5<<""<<f5<<""<<f5<<""<<f5<<""<<f5<<""<<f5<<"|"<<f6<<""<<f6<<""<<f6<<""<<f6<<""<<f6<<""<<f6<<"|      " << "  /______/______/______/"<<r7<<""<<r7<<"|/"<<r5<<"|/"<<r3<<"|" << "            View Commands:\n"
                << "   /|"<<l8<<"/"<<l6<<"|"<<f4<<""<<f4<<""<<f4<<""<<f4<<""<<f4<<""<<f4<<"|"<<f5<<""<<f5<<""<<f5<<""<<f5<<""<<f5<<""<<f5<<"|"<<f6<<""<<f6<<""<<f6<<""<<f6<<""<<f6<<""<<f6<<"|      " << "  |"<<f7<<""<<f7<<""<<f7<<""<<f7<<""<<f7<<""<<f7<<"|"<<f8<<""<<f8<<""<<f8<<""<<f8<<""<<f8<<""<<f8<<"|"<<f9<<""<<f9<<""<<f9<<""<<f9<<""<<f9<<""<<f9<<"|"<<r7<<""<<r7<<"|"<<r5<<r5<<"|"<<r3<<""<<r3<<"|\n"
                << "  /"<<l7<<"|/|"<<l6<<"|______|______|______|      " << "  |"<<f7<<""<<f7<<""<<f7<<""<<f7<<""<<f7<<""<<f7<<"|"<<f8<<""<<f8<<""<<f8<<""<<f8<<""<<f8<<""<<f8<<"|"<<f9<<""<<f9<<""<<f9<<""<<f9<<""<<f9<<""<<f9<<"|"<<r7<<"/|"<<r5<<"/|"<<r3<<"/" << "               [H] [-H]\n"
                << "  |"<<l7<<"/"<<l5<<"|/|"<<f1<<""<<f1<<""<<f1<<""<<f1<<""<<f1<<""<<f1<<"|"<<f2<<""<<f2<<""<<f2<<""<<f2<<""<<f2<<""<<f2<<"|"<<f3<<""<<f3<<""<<f3<<""<<f3<<""<<f3<<""<<f3<<"|      " << "  |______|______|______|/"<<r4<<"|/"<<r2<<"|/\n"
                << "  |/|"<<l5<<"/"<<l3<<"|"<<f1<<""<<f1<<""<<f1<<""<<f1<<""<<f1<<""<<f1<<"|"<<f2<<""<<f2<<""<<f2<<""<<f2<<""<<f2<<""<<f2<<"|"<<f3<<""<<f3<<""<<f3<<""<<f3<<""<<f3<<""<<f3<<"|      " << "  |"<<f4<<""<<f4<<""<<f4<<""<<f4<<""<<f4<<""<<f4<<"|"<<f5<<f5<<f5<<f5<<f5<<f5<<"|"<<f6<<""<<f6<<""<<f6<<""<<f6<<""<<f6<<""<<f6<<"|"<<r4<<""<<r4<<"|"<<r2<<""<<r2<<"|" << "                 [V] [-V]\n"
                << "  |"<<l4<<"|/|"<<l3<<"|______|______|______|      " << "  |"<<f4<<""<<f4<<""<<f4<<""<<f4<<""<<f4<<""<<f4<<"|"<<f5<<f5<<f5<<f5<<f5<<f5<<"|"<<f6<<""<<f6<<""<<f6<<""<<f6<<""<<f6<<""<<f6<<"|"<<r4<<"/|"<<r2<<"/\n"
                << "  |"<<l4<<"/"<<l2<<"|/"<<d3<<""<<d3<<""<<d3<<""<<d3<<""<<d3<<""<<d3<<"/"<<d2<<""<<d2<<""<<d2<<""<<d2<<""<<d2<<""<<d2<<"/"<<d1<<""<<d1<<""<<d1<<""<<d1<<""<<d1<<""<<d1<<"/       " << "  |______|______|______|/"<<r1<<"|/\n"
                << "  |/|"<<l2<<"/______/______/______/        " << "  |"<<f1<<""<<f1<<""<<f1<<""<<f1<<""<<f1<<""<<f1<<"|"<<f2<<""<<f2<<""<<f2<<""<<f2<<""<<f2<<""<<f2<<"|"<<f3<<""<<f3<<""<<f3<<""<<f3<<""<<f3<<""<<f3<<"|"<<r1<<""<<r1<<"|\n"
                << "  |"<<l1<<"|/"<<d6<<""<<d6<<""<<d6<<""<<d6<<""<<d6<<""<<d6<<"/"<<d5<<""<<d5<<""<<d5<<""<<d5<<""<<d5<<""<<d5<<"/"<<d4<<""<<d4<<""<<d4<<""<<d4<<""<<d4<<""<<d4<<"/         " << "  |"<<f1<<""<<f1<<""<<f1<<""<<f1<<""<<f1<<""<<f1<<"|"<<f2<<""<<f2<<""<<f2<<""<<f2<<""<<f2<<""<<f2<<"|"<<f3<<""<<f3<<""<<f3<<""<<f3<<""<<f3<<""<<f3<<"|"<<r1<<"/\n"
                << "  |"<<l1<<"/------/------/------/          " << "  |______|______|______|/\n"

                << "  |/"<<d9<<""<<d9<<""<<d9<<""<<d9<<""<<d9<<""<<d9<<"/"<<d8<<""<<d8<<""<<d8<<""<<d8<<""<<d8<<""<<d8<<"/"<<d7<<""<<d7<<""<<d7<<""<<d7<<""<<d7<<""<<d7<<"/           \n"
                << "   ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾             \n";
    } else {
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