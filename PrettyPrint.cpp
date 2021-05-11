#include "PrettyPrint.h"
#include "Rubik.h"

#include <algorithm>
#include <fmt/core.h>
#include <iostream>
#include <regex>
#include <sstream>
#include <stack>
#include <sys/ioctl.h>
#include <unistd.h>

static std::string
repeat(const std::string& s, int times)
{
    std::string ret(s);
    for (int i=1; i<times; ++i)
    {
        ret += s;
    }
    return ret;
}

static std::string
repeat(char c, int times)
{
    std::string ret;
    for (int i=0; i<times; ++i)
    {
        ret += c;
    }
    return ret;
}

void
pp::PrintView::addLine(std::string&& line)
{
    if (line.size() > m_horizontalSize && line.find("‾") == std::string::npos)
    {
        m_horizontalSize = line.size();
    }
    m_lines.emplace_back(std::move(line));
}

namespace
{

class LeftView : public pp::PrintView
{
    public:
        LeftView(RubikCube& c, int verticalPadding = 5)
        {
            auto& cubids = c.getCubids();
            char f1 = RubikConstants::value(cubids[0][2][0].getColor(Position::front));
            char f2 = RubikConstants::value(cubids[1][2][0].getColor(Position::front));
            char f3 = RubikConstants::value(cubids[2][2][0].getColor(Position::front));
            char f4 = RubikConstants::value(cubids[0][1][0].getColor(Position::front));
            char f5 = RubikConstants::value(cubids[1][1][0].getColor(Position::front));
            char f6 = RubikConstants::value(cubids[2][1][0].getColor(Position::front));
            char f7 = RubikConstants::value(cubids[0][0][0].getColor(Position::front));
            char f8 = RubikConstants::value(cubids[1][0][0].getColor(Position::front));
            char f9 = RubikConstants::value(cubids[2][0][0].getColor(Position::front));

            char l1 = RubikConstants::value(cubids[0][2][2].getColor(Position::left));
            char l2 = RubikConstants::value(cubids[0][2][1].getColor(Position::left));
            char l3 = RubikConstants::value(cubids[0][2][0].getColor(Position::left));
            char l4 = RubikConstants::value(cubids[0][1][2].getColor(Position::left));
            char l5 = RubikConstants::value(cubids[0][1][1].getColor(Position::left));
            char l6 = RubikConstants::value(cubids[0][1][0].getColor(Position::left));
            char l7 = RubikConstants::value(cubids[0][0][2].getColor(Position::left));
            char l8 = RubikConstants::value(cubids[0][0][1].getColor(Position::left));
            char l9 = RubikConstants::value(cubids[0][0][0].getColor(Position::left));

            char d1 = RubikConstants::value(cubids[2][2][0].getColor(Position::down));
            char d2 = RubikConstants::value(cubids[1][2][0].getColor(Position::down));
            char d3 = RubikConstants::value(cubids[0][2][0].getColor(Position::down));
            char d4 = RubikConstants::value(cubids[2][2][1].getColor(Position::down));
            char d5 = RubikConstants::value(cubids[1][2][1].getColor(Position::down));
            char d6 = RubikConstants::value(cubids[0][2][1].getColor(Position::down));
            char d7 = RubikConstants::value(cubids[2][2][2].getColor(Position::down));
            char d8 = RubikConstants::value(cubids[1][2][2].getColor(Position::down));
            char d9 = RubikConstants::value(cubids[0][2][2].getColor(Position::down));

            for (int i=0; i<verticalPadding; ++i)
            {
                addLine("");
            }
            // all front and down faces should repeat 6 times
            int fr = 6;
            int dr = 6;
            addLine(fmt::format("      _____________________"));
            addLine(fmt::format("     /|{}|{}|{}|", repeat(f7, fr), repeat(f8, fr), repeat(f9, fr)));
            addLine(fmt::format("    /{}|{}|{}|{}|", l9, repeat(f7, fr), repeat(f8, fr), repeat(f9, fr)));
            addLine(fmt::format("   /|{}|______|______|______|", l9));
            addLine(fmt::format("  /{}|/|{}|{}|{}|", l8, repeat(f4, fr), repeat(f5, fr), repeat(f6, fr)));
            addLine(fmt::format(" /|{}/{}|{}|{}|{}|", l8, l6, repeat(f4, fr), repeat(f5, fr), repeat(f6, fr)));
            addLine(fmt::format("/{}|/|{}|______|______|______|", l7, l6));
            addLine(fmt::format("|{}/{}|/|{}|{}|{}|", l7, l5, repeat(f1, fr), repeat(f2, fr), repeat(f3, fr)));
            addLine(fmt::format("|/|{}/{}|{}|{}|{}|", l5, l3, repeat(f1, fr), repeat(f2, fr), repeat(f3, fr)));
            addLine(fmt::format("|{}|/|{}|______|______|______|", l4, l3));
            addLine(fmt::format("|{}/{}|/{}/{}/{}/", l4, l2, repeat(d3, dr), repeat(d2, dr), repeat(d1, dr)));
            addLine(fmt::format("|/|{}/______/______/______/", l2));
            addLine(fmt::format("|{}|/{}/{}/{}/", l1, repeat(d6, dr), repeat(d5, dr), repeat(d4, dr)));
            addLine(fmt::format("|{}/------/------/------/", l1));
            addLine(fmt::format("|/{}/{}/{}/", repeat(d9, dr), repeat(d8, dr), repeat(d7, dr)));
            addLine(fmt::format(" ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾"));
        }
};

class MainView : public pp::PrintView
{
    public:
        MainView(RubikCube& c)
        {
            auto& cubids = c.getCubids();
            char u1 = RubikConstants::value(cubids[0][0][0].getColor(Position::up));
            char u2 = RubikConstants::value(cubids[1][0][0].getColor(Position::up));
            char u3 = RubikConstants::value(cubids[2][0][0].getColor(Position::up));
            char u4 = RubikConstants::value(cubids[0][0][1].getColor(Position::up));
            char u5 = RubikConstants::value(cubids[1][0][1].getColor(Position::up));
            char u6 = RubikConstants::value(cubids[2][0][1].getColor(Position::up));
            char u7 = RubikConstants::value(cubids[0][0][2].getColor(Position::up));
            char u8 = RubikConstants::value(cubids[1][0][2].getColor(Position::up));
            char u9 = RubikConstants::value(cubids[2][0][2].getColor(Position::up));

            char r1 = RubikConstants::value(cubids[2][2][0].getColor(Position::right));
            char r2 = RubikConstants::value(cubids[2][2][1].getColor(Position::right));
            char r3 = RubikConstants::value(cubids[2][2][2].getColor(Position::right));
            char r4 = RubikConstants::value(cubids[2][1][0].getColor(Position::right));
            char r5 = RubikConstants::value(cubids[2][1][1].getColor(Position::right));
            char r6 = RubikConstants::value(cubids[2][1][2].getColor(Position::right));
            char r7 = RubikConstants::value(cubids[2][0][0].getColor(Position::right));
            char r8 = RubikConstants::value(cubids[2][0][1].getColor(Position::right));
            char r9 = RubikConstants::value(cubids[2][0][2].getColor(Position::right));

            char f1 = RubikConstants::value(cubids[0][2][0].getColor(Position::front));
            char f2 = RubikConstants::value(cubids[1][2][0].getColor(Position::front));
            char f3 = RubikConstants::value(cubids[2][2][0].getColor(Position::front));
            char f4 = RubikConstants::value(cubids[0][1][0].getColor(Position::front));
            char f5 = RubikConstants::value(cubids[1][1][0].getColor(Position::front));
            char f6 = RubikConstants::value(cubids[2][1][0].getColor(Position::front));
            char f7 = RubikConstants::value(cubids[0][0][0].getColor(Position::front));
            char f8 = RubikConstants::value(cubids[1][0][0].getColor(Position::front));
            char f9 = RubikConstants::value(cubids[2][0][0].getColor(Position::front));

            int fr = 6;
            int ur = 6;
            addLine(fmt::format("         _____________________"));
            addLine(fmt::format("        /{}/{}/{}/|", repeat(u7, ur), repeat(u8, ur), repeat(u9, ur)));
            addLine(fmt::format("       /{}/{}/{}/{}|", repeat(u7, ur), repeat(u8, ur), repeat(u9, ur), r9));
            addLine(fmt::format("      /______/______/______/{}|", repeat(r9, 2)));
            addLine(fmt::format("     /{}/{}/{}/|{}|", repeat(u4, ur), repeat(u5, ur), repeat(u6, ur), repeat(r9, 2)));
            addLine(fmt::format("    /{}/{}/{}/{}|{}/|", repeat(u4, ur), repeat(u5, ur), repeat(u6, ur), r8, r9));
            addLine(fmt::format("   /______/______/______/{}|/{}|", repeat(r8, 2), r6));
            addLine(fmt::format("  /{}/{}/{}/|{}|{}|", repeat(u1, ur), repeat(u2, ur), repeat(u3, ur), repeat(r8, 2), repeat(r6, 2)));
            addLine(fmt::format(" /{}/{}/{}/{}|{}/|{}/|", repeat(u1, ur), repeat(u2, ur), repeat(u3, ur), r7, r8, r6));
            addLine(fmt::format("/______/______/______/{}|/{}|/{}|", repeat(r7, 2), r5, r3));
            addLine(fmt::format("|{}|{}|{}|{}|{}|{}|", repeat(f7, fr), repeat(f8, fr), repeat(f9, fr), repeat(r7, 2), repeat(r5, 2), repeat(r3,2)));
            addLine(fmt::format("|{}|{}|{}|{}/|{}/|{}/", repeat(f7, fr), repeat(f8, fr), repeat(f9, fr), r7, r5, r3));
            addLine(fmt::format("|______|______|______|/{}|/{}|/", r4, r2));
            addLine(fmt::format("|{}|{}|{}|{}|{}|", repeat(f4, fr), repeat(f5, fr), repeat(f6, fr), repeat(r4, 2), repeat(r2, 2)));
            addLine(fmt::format("|{}|{}|{}|{}/|{}/", repeat(f4, fr), repeat(f5, fr), repeat(f6, fr), r4, r2));
            addLine(fmt::format("|______|______|______|/{}|/", r1));
            addLine(fmt::format("|{}|{}|{}|{}|", repeat(f1, fr), repeat(f2, fr), repeat(f3, fr), repeat(r1, 2)));
            addLine(fmt::format("|{}|{}|{}|{}/", repeat(f1, fr), repeat(f2, fr), repeat(f3, fr), r1));
            addLine(fmt::format("|______|______|______|/"));
        }
};

class CmdHelpView : public pp::PrintView
{
    public:
        CmdHelpView()
        {
            m_colorized = false;
            addLine("Rotation Commands:");
            addLine("");
            addLine("[R] [-R] [L] [-L]");
            addLine("");
            addLine("[F] [-F] [B] [-B]");
            addLine("");
            addLine("[U] [-U] [D] [-D]");
            addLine("");
            addLine("");
            addLine("  View Commands:");
            addLine("");
            addLine("    [H] [-H]");
            addLine("");
            addLine("    [V] [-V]");
        }
};

} // namespace

std::string
pp::TerminalPrinter::colorize(const Color c, const std::string& str)
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
    return retVal + str + "\e[0m";
}

std::string
pp::TerminalPrinter::getColorizedViewString(const std::string& viewStr)
{
    std::string colorized;

    std::stack<std::pair<std::regex, std::string>> regexStack;
    const std::string fullMatch = "$&";
    regexStack.push({std::regex(std::string(1,value(Color::red))+"+"), colorize(Color::red, fullMatch)});
    regexStack.push({std::regex(std::string(1,value(Color::blue))+"+"), colorize(Color::blue, fullMatch)});
    regexStack.push({std::regex(std::string(1,value(Color::yellow))+"+"), colorize(Color::yellow, fullMatch)});
    regexStack.push({std::regex(std::string(1,value(Color::green))+"+"), colorize(Color::green, fullMatch)});
    regexStack.push({std::regex(std::string(1,value(Color::orange))+"+"), colorize(Color::orange, fullMatch)});
    regexStack.push({std::regex(std::string(1,value(Color::white))+"+"), colorize(Color::white, fullMatch)});

    while (!regexStack.empty())
    {
        auto pair = regexStack.top();
        regexStack.pop();
        const std::string& previous = colorized.empty() ? viewStr : colorized;
        colorized = std::regex_replace(previous, pair.first, pair.second);
    }

    return colorized;
}

void
pp::TerminalPrinter::print(const std::list<const PrintView*>& viewList)
{
    if (viewList.empty())
    {
        return;
    }

    std::stringstream stream;
    auto gapSize = [&] (const PrintView& view, int lineNum)
    {
        return view.horizontalSize() - view.line(lineNum).size() + m_gapSize;
    };
    int maxLines = (*std::max_element(viewList.begin(),
                                      viewList.end(),
                                      [](const PrintView *v1, const PrintView *v2) {
                                          return v1->verticalSize() < v2->verticalSize();
                                      }))
                       ->verticalSize();
    for (int i=0; i<maxLines; ++i)
    {
        auto viewNumber = 0;
        for (const PrintView* v : viewList)
        {
            stream << (v->colorize() ? getColorizedViewString(v->line(i)) : v->line(i))
                   << (++viewNumber < viewList.size() ? repeat(" ", gapSize(*v, i)) : "");
        }
        stream << "\n";
    }
    m_outStream << stream.str() << std::flush;
}

void
pp::TerminalPrinter::print()
{
    winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    int terminalHzSize = size.ws_col;
    std::list<const PrintView*> all = m_views;
    while (!all.empty())
    {
        std::list<const PrintView*> row;
        int currentSize = 0;
        while (!all.empty() && currentSize < terminalHzSize)
        {
            if (currentSize + all.front()->horizontalSize() < terminalHzSize)
            {
                row.emplace_back(all.front());
                all.pop_front();
            }
            currentSize += row.back()->horizontalSize();
        }
        print(row);
    }
}

void
pp::printRubikCube(RubikCube& c)
{
    LeftView lv(c);
    MainView mv(c);
    CmdHelpView cmds;
    TerminalPrinter tp(std::cout, {&lv, &mv, &cmds});
    tp.print();
}

/*
void pp::printRubikCube(RubikCube& c)
{
    auto& cubids = c.getCubids();
    std::string u1(1,value(cubids[0][0][0].getColor(Position::up)));
    std::string u2(1,value(cubids[1][0][0].getColor(Position::up)));
    std::string u3(1,value(cubids[2][0][0].getColor(Position::up)));
    std::string u4(1,value(cubids[0][0][1].getColor(Position::up)));
    std::string u5(1,value(cubids[1][0][1].getColor(Position::up)));
    std::string u6(1,value(cubids[2][0][1].getColor(Position::up)));
    std::string u7(1,value(cubids[0][0][2].getColor(Position::up)));
    std::string u8(1,value(cubids[1][0][2].getColor(Position::up)));
    std::string u9(1,value(cubids[2][0][2].getColor(Position::up)));

    std::string r1(1,value(cubids[2][2][0].getColor(Position::right)));
    std::string r2(1,value(cubids[2][2][1].getColor(Position::right)));
    std::string r3(1,value(cubids[2][2][2].getColor(Position::right)));
    std::string r4(1,value(cubids[2][1][0].getColor(Position::right)));
    std::string r5(1,value(cubids[2][1][1].getColor(Position::right)));
    std::string r6(1,value(cubids[2][1][2].getColor(Position::right)));
    std::string r7(1,value(cubids[2][0][0].getColor(Position::right)));
    std::string r8(1,value(cubids[2][0][1].getColor(Position::right)));
    std::string r9(1,value(cubids[2][0][2].getColor(Position::right)));

    std::string f1(1,value(cubids[0][2][0].getColor(Position::front)));
    std::string f2(1,value(cubids[1][2][0].getColor(Position::front)));
    std::string f3(1,value(cubids[2][2][0].getColor(Position::front)));
    std::string f4(1,value(cubids[0][1][0].getColor(Position::front)));
    std::string f5(1,value(cubids[1][1][0].getColor(Position::front)));
    std::string f6(1,value(cubids[2][1][0].getColor(Position::front)));
    std::string f7(1,value(cubids[0][0][0].getColor(Position::front)));
    std::string f8(1,value(cubids[1][0][0].getColor(Position::front)));
    std::string f9(1,value(cubids[2][0][0].getColor(Position::front)));

    std::string l1(1,value(cubids[0][2][2].getColor(Position::left)));
    std::string l2(1,value(cubids[0][2][1].getColor(Position::left)));
    std::string l3(1,value(cubids[0][2][0].getColor(Position::left)));
    std::string l4(1,value(cubids[0][1][2].getColor(Position::left)));
    std::string l5(1,value(cubids[0][1][1].getColor(Position::left)));
    std::string l6(1,value(cubids[0][1][0].getColor(Position::left)));
    std::string l7(1,value(cubids[0][0][2].getColor(Position::left)));
    std::string l8(1,value(cubids[0][0][1].getColor(Position::left)));
    std::string l9(1,value(cubids[0][0][0].getColor(Position::left)));

    std::string d1(1,value(cubids[2][2][0].getColor(Position::down)));
    std::string d2(1,value(cubids[1][2][0].getColor(Position::down)));
    std::string d3(1,value(cubids[0][2][0].getColor(Position::down)));
    std::string d4(1,value(cubids[2][2][1].getColor(Position::down)));
    std::string d5(1,value(cubids[1][2][1].getColor(Position::down)));
    std::string d6(1,value(cubids[0][2][1].getColor(Position::down)));
    std::string d7(1,value(cubids[2][2][2].getColor(Position::down)));
    std::string d8(1,value(cubids[1][2][2].getColor(Position::down)));
    std::string d9(1,value(cubids[0][2][2].getColor(Position::down)));

    std::string b1(1,value(cubids[2][2][2].getColor(Position::back)));
    std::string b2(1,value(cubids[1][2][2].getColor(Position::back)));
    std::string b3(1,value(cubids[0][2][2].getColor(Position::back)));
    std::string b4(1,value(cubids[2][1][2].getColor(Position::back)));
    std::string b5(1,value(cubids[1][1][2].getColor(Position::back)));
    std::string b6(1,value(cubids[0][1][2].getColor(Position::back)));
    std::string b7(1,value(cubids[2][0][2].getColor(Position::back)));
    std::string b8(1,value(cubids[1][0][2].getColor(Position::back)));
    std::string b9(1,value(cubids[0][0][2].getColor(Position::back)));

    // std::string y = toString(Color::yellow);
    // std::string r = toString(Color::red);
    // std::string g = toString(Color::green);
    // std::string b = toString(Color::blue);
    // std::string w = toString(Color::white);

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
*/