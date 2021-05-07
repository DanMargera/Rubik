#ifndef PRETTY_PRINT_H
#define PRETTY_PRINT_H

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
    protected:
        void addLine(std::string&& line);
        std::vector<std::string> m_lines;
        std::string m_empty{""};
        int m_horizontalSize{0};
};

class LeftView : public PrintView
{
    public:
        LeftView(RubikCube& c, int verticalPadding = 5);
};

class MainView : public PrintView
{
    public:
        MainView(RubikCube& c);
};

void printRubikCube(RubikCube& c);

}

#endif