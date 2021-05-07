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
    protected:
        void addLine(std::string&& line);
        std::vector<std::string> m_lines;
        int m_horizontalSize{0};
};

class LeftView : PrintView
{
    public:
        LeftView(RubikCube& c, int verticalPadding = 5);
};

class MainView : PrintView
{
    public:
        MainView(RubikCube& c);
};

void printRubikCube(RubikCube& c);

}

#endif