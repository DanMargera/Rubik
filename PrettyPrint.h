#ifndef PRETTY_PRINT_H
#define PRETTY_PRINT_H

#include "Constants.h"

#include <list>
#include <string>
#include <vector>

class RubikCube;

namespace pp //xd
{
class PrintView
{
    public:
        PrintView() = default;
        int verticalSize() const { return m_lines.size(); }
        int horizontalSize() const { return m_horizontalSize; }
        const std::string& line(int pos) const { return pos >= m_lines.size() ? m_empty : m_lines.at(pos); }
        void addLine(std::string&& line);
        bool colorize() const { return m_colorized; }
    protected:
        std::vector<std::string> m_lines;
        std::string m_empty{""};
        int m_horizontalSize{0};
        bool m_colorized{true};
};

class TerminalPrinter
{
    public:
        TerminalPrinter() = default;
        TerminalPrinter(bool colorize, int gapSize) : m_colorize(colorize), m_gapSize(gapSize) { }
        void print(std::list<const PrintView*> views);
    private:
        std::string colorize(RubikConstants::Color c, const std::string& viewStr);
        std::string getColorizedViewString(const std::string& viewStr);
        int m_gapSize{4};
        bool m_colorize{true};
};

void printRubikCube(RubikCube& c);

}

#endif