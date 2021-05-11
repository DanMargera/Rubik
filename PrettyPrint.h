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
        TerminalPrinter(std::ostream& ostr, std::list<const PrintView*>&& views) : m_outStream(ostr), m_views(std::move(views)) { }
        void print();
    private:
        void print(const std::list<const PrintView*>& views);
        std::string colorize(RubikConstants::Color c, const std::string& viewStr);
        std::string getColorizedViewString(const std::string& viewStr);

        int m_gapSize{4};
        bool m_colorize{true};
        std::ostream& m_outStream;
        std::list<const PrintView*> m_views;
};

void printRubikCube(RubikCube& c);

}

#endif