#include "Algorithm.h"
#include "Rubik.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

int main()
{
    RubikCube c;
    c.print();
    std::string opt;
    while (opt != "q") {
        std::cin >> opt;
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
        else if (opt == "downcross") Algorithm::downCross(c);
        else if (opt == "downcorners") Algorithm::downCorners(c);
        else if (opt == "middlelayer") Algorithm::middleLayer(c);
        else if (opt == "topcross") Algorithm::topCross(c);
        else if (opt == "topface") Algorithm::topFace(c);
        else if (opt == "toplayer") Algorithm::topLayer(c);
        else if (opt == "layersolve") Algorithm::layerSolve(c);
        else if (opt == "reset") c = RubikCube();
        else if (opt == "brute") Algorithm::bruteSolve(c, 6);
        else if (opt == "s?") {
            std::cout << "Solved? " << c.isSolved() << std::endl;
            continue;
        }
        c.print();
    }

    return 0;
}
