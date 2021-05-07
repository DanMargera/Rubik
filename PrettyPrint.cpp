#include "PrettyPrint.h"
#include "Rubik.h"

#include <fmt/core.h>
#include <iostream>

static std::string
repeat(const std::string &s, int times)
{
    std::string ret(s);
    for (int i=1; i<times; ++i)
    {
        ret += s;
    }
    return ret;
}

void
pp::PrintView::addLine(std::string&& line)
{
    if (line.length() > m_horizontalSize)
    {
        m_horizontalSize = line.length();
    }
    m_lines.emplace_back(std::move(line));
}

pp::LeftView::LeftView(RubikCube& c, int verticalPadding)
{
    auto& cubids = c.getCubids();
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

    for (int i=0; i<verticalPadding; ++i)
    {
        addLine("");
    }
    // all front and down faces should repeat 6 times
    int fr = 6;
    int dr = 6;
    addLine(fmt::format("       _____________________"));
    addLine(fmt::format("      /|{frt7}|{frt8}|{frt9}|", repeat(f7, fr), repeat(f8, fr), repeat(f9, fr)));
    addLine(fmt::format("     /{}|{frt7}|{frt8}|{frt9}|", l9, repeat(f7, fr), repeat(f8, fr), repeat(f9, fr)));
    addLine(fmt::format("    /|{}|______|______|______|", l9));
    addLine(fmt::format("   /{}|/|{frt4}|{frt5}|{frt6}|", l8, repeat(f4, fr), repeat(f5, fr), repeat(f6, fr)));
    addLine(fmt::format("  /|{}/{}|{frt4}|{frt5}|{frt6}|", l8, l6, repeat(f4, fr), repeat(f5, fr), repeat(f6, fr)));
    addLine(fmt::format(" /{}|/|{}|______|______|______|", l7, l6));
    addLine(fmt::format(" |{}/{}|/|{frt1}|{frt2}|{frt3}|", l7, l5, repeat(f1, fr), repeat(f2, fr), repeat(f3, fr)));
    addLine(fmt::format(" |/|{}/{}|{frt1}|{frt2}|{frt3}|", l5, l3, repeat(f1, fr), repeat(f2, fr), repeat(f3, fr)));
    addLine(fmt::format(" |{}|/|{}|______|______|______|", l4, l3));
    addLine(fmt::format(" |{}/{}|/{dwn3}/{dwn2}/{dwn1}/", l4, l2, repeat(d3, dr), repeat(d2, dr), repeat(d1, dr)));
    addLine(fmt::format(" |/|{}/______/______/______/", l2));
    addLine(fmt::format(" |{}|/{dwn6}/{dwn5}/{dwn4}/", l1, repeat(d6, dr), repeat(d5, dr), repeat(d4, dr)));
    addLine(fmt::format(" |{}/------/------/------/", l1));
    addLine(fmt::format(" |/{dwn9}/{dwn8}/{dwn7}/", repeat(d9, dr), repeat(d8, dr), repeat(d7, dr)));
    addLine(fmt::format("  ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾"));
}

pp::MainView::MainView(RubikCube& c)
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

    int fr = 6;
    int ur = 6;
    addLine(fmt::format("           _____________________"));
    addLine(fmt::format("          /{upr7}/{upr8}/{upr9}/|", repeat(u7, ur), repeat(u8, ur), repeat(u8, ur)));
    addLine(fmt::format("         /{upr7}/{upr8}/{upr9}/{}|", repeat(u8, ur), repeat(u8, ur), repeat(u8, ur), r9));
    addLine(fmt::format("        /______/______/______/{9}|", repeat(r9, 2)));
    addLine(fmt::format("       /{upr4}/{upr5}/{upr6}/|{9}|", repeat(u4, ur), repeat(u5, ur), repeat(u6, ur), repeat(r9, 2)));
    addLine(fmt::format("      /{upr4}/{upr5}/{upr6}/{}|{}/|", repeat(u4, ur), repeat(u5, ur), repeat(u6, ur), r8, r9));
    addLine(fmt::format("     /______/______/______/{8}|/{}|", repeat(r8, 2), r6));
    addLine(fmt::format("    /{upr1}/{upr2}/{upr3}/|{8}|{6}|", repeat(u1, ur), repeat(u2, ur), repeat(u3, ur), repeat(r8, 2), repeat(r6, 2)));
    addLine(fmt::format("   /{upr1}/{upr2}/{upr3}/{}|{}/|{}/|", repeat(u1, ur), repeat(u2, ur), repeat(u2, ur), r7, r8, r6));
    addLine(fmt::format("  /______/______/______/{7}|/{}|/{}|", repeat(r7, 2), r5, r3));
    addLine(fmt::format("  |{frt7}|{frt8}|{frt9}|{7}|{5}|{3}|", repeat(f7, fr), repeat(f8, fr), repeat(f9, fr), repeat(r7, 2), repeat(r5, 2), repeat(r3,2)));
    addLine(fmt::format("  |{frt7}|{frt8}|{frt9}|{}/|{}/|{}/", repeat(f7, fr), repeat(f8, fr), repeat(f9, fr), r7, r5, r3));
    addLine(fmt::format("  |______|______|______|/{}|/{}|/", r4, r2));
    addLine(fmt::format("  |{frt4}|{frt5}|{frt6}|{4}|{2}|", repeat(f4, fr), repeat(f5, fr), repeat(f6, fr), repeat(r4, 2), repeat(r2, 2)));
    addLine(fmt::format("  |{frt4}|{frt5}|{frt6}|{}/|{}/", repeat(f4, fr), repeat(f5, fr), repeat(f6, fr), r4, r2));
    addLine(fmt::format("  |______|______|______|/{}|/", r1));
    addLine(fmt::format("  |{frt1}|{frt2}|{frt3}|{1}|", repeat(f1, fr), repeat(f2, fr), repeat(f3, fr), repeat(r1, 2)));
    addLine(fmt::format("  |{frt1}|{frt2}|{frt3}|{}/", repeat(f1, fr), repeat(f2, fr), repeat(f3, fr), r1));
    addLine(fmt::format("  |______|______|______|/"));
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