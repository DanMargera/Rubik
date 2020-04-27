#include "Algorithm.h"
#include "Rubik.h"
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <time.h>

static void help()
{
    std::string optColor = "\e[38;5;246m\e[1m";
    std::cout << "\e[1m- \e[38;5;208mASCII \e[38;5;196mR\e[38;5;165mu\e[38;5;27mb\e[38;5;118mi\e[38;5;226mk\e[0m \e[1mCommands -\n"
              << optColor << "  shuffle     \e[0m" << " Shuffles cube. Applies 15~25 random rotations.\n"
              << optColor << "  layersolve  \e[0m" << " Applies an inneficient layer solving algorithm. 150+ moves.\n"
              << optColor << "  brute       \e[0m" << " Explores all possible combinations of 6 moves from the current state.\n"
              << optColor << "  reset       \e[0m" << " Resets the initial state of the cube.\n"
              << optColor << "  help        \e[0m" << " Prints this help text.\n"
              << optColor << "  q           \e[0m" << " Quit.\n\n"

              << "- Apply rotations using face initials:  [R]ight [L]eft [U]p\n"
              << "                                        [F]ront [B]ack [D]own\n"
              << optColor << "  R \e[0m\t"    << " Rotates right face clockwise.\n"
              << optColor << "  -R \e[0m\t"    << " Rotates right face counter-clockwise.\n\n"
              << "- Change angle of view with: [V]ertical [H]orizontal\n"
              << std::endl;
}

static void toLower(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c){ return std::tolower(c); });
}

static void clear()
{
    printf("\033[H""\033[J");
    std::cout << std::flush;
}

int main()
{
    RubikCube c;
    clear();
    c.print();
    help();
    std::string opt;
    while (opt != "q") {
        std::cin >> opt;
        clear();
        toLower(opt);
        if (opt == "r") c.rotateSide(Position::right, false);
        else if (opt == "-r") c.rotateSide(Position::right, true);
        else if (opt == "l") c.rotateSide(Position::left, false);
        else if (opt == "-l") c.rotateSide(Position::left, true);
        else if (opt == "u") c.rotateSide(Position::up, false);
        else if (opt == "-u") c.rotateSide(Position::up, true);
        else if (opt == "d") c.rotateSide(Position::down, false);
        else if (opt == "-d") c.rotateSide(Position::down, true);
        else if (opt == "f") c.rotateSide(Position::front, false);
        else if (opt == "-f") c.rotateSide(Position::front, true);
        else if (opt == "b") c.rotateSide(Position::back, false);
        else if (opt == "-b") c.rotateSide(Position::back, true);
        else if (opt == "h") c.turn(/*horizontal*/true, false);
        else if (opt == "-h") c.turn(/*horizontal*/true, true);
        else if (opt == "v") c.turn(/*horizontal*/false, false);
        else if (opt == "-v") c.turn(/*horizontal*/false, true);
        else if (opt == "shuffle") c.shuffle();
        else if (opt == "layersolve") {
            c.print();
            c.resetMoveCount();
            c.setVerboseMoves(true);
            std::cout << "\n{ ";
            Algorithm::layerSolve(c);
            std::cout << " }\n" << std::endl;
            std::cout << "Solved cube in " << c.getMoveCount() << " moves.\n" << std::endl;
            c.setVerboseMoves(false);
        }
        else if (opt == "reset") c = RubikCube();
        else if (opt == "brute") Algorithm::bruteSolve(c, 6);
        else if (opt == "s?") {
            std::cout << "Solved? " << c.isSolved() << std::endl;
            continue;
        } else if (opt != "q") {
            c.print();
            help();
            continue;
        }
        c.print();
    }

    return 0;
}
