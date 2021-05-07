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
        std::vector<std::string> m_lines;
};

class LeftView : PrintView
{
    public:
        LeftView(RubikCube& c);
};

class MainView : PrintView
{
    public:
        MainView(RubikCube& c);
};

void printRubikCube(RubikCube& c);

}

#endif