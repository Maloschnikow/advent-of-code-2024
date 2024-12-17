#include <iostream>
#include <regex>

#include "../helper.hpp"

struct ClawMachine {
    helper::Coord a;
    helper::Coord b;
    helper::Coord prizeLocation;
};

/**
 * ab
 * cd
 */
long determinantOf2x2(long a, long b, long c, long d) {
    return a*d - c*b;
}

/**
 * linear equation
 * s * a.x + t * b.x = prizeLocation.x
 * s * a.y + t * b.y = prizeLocation.y
 * 
 * solve for s and t (cramers rule)
 */

void clawMachineCorrection(ClawMachine *clawMachine) {
    clawMachine->prizeLocation.x += 10000000000000;
    clawMachine->prizeLocation.y += 10000000000000;
}

bool solveWithCramersRule(ClawMachine clawMachine, long *s, long *t) {
    long detA  = determinantOf2x2(clawMachine.a.x            , clawMachine.a.y            , clawMachine.b.x            , clawMachine.b.y            );
    
    if(detA == 0) {
        return false;
    }

    long detAs = determinantOf2x2(clawMachine.prizeLocation.x, clawMachine.b.x            , clawMachine.prizeLocation.y, clawMachine.b.y            );
    long detAt = determinantOf2x2(clawMachine.a.x            , clawMachine.prizeLocation.x, clawMachine.a.y            , clawMachine.prizeLocation.y);

    *s = detAs / detA;
    *t = detAt / detA;

    return true;
}

bool checkSolution(ClawMachine clawMachine, long &s, long &t) {
    return s * clawMachine.a + t * clawMachine.b == clawMachine.prizeLocation;
}

int main() {
    std::vector<ClawMachine> clawMachines;
    std::string line;

    do {
        ClawMachine currentMachine;
        for(int i = 0; i < 3; ++i) {

            std::getline(std::cin, line);

            std::regex pattern(R"(.*?X[\+\=](\d+), Y[\+\=](\d+))");
            std::smatch matches;

            if (std::regex_match(line, matches, pattern)) {

                int x = std::stoi(matches[1].str());
                int y = std::stoi(matches[2].str());

                switch (i)
                {
                case 0:
                    currentMachine.a = helper::Coord(x, y);
                    break;
                case 1:
                    currentMachine.b = helper::Coord(x, y);
                case 2:
                    currentMachine.prizeLocation = helper::Coord(x,y);
                }

            } else {
                std::cerr << "No matches found!" << std::endl;
            }
        }
        clawMachines.push_back(currentMachine);

    } while(std::getline(std::cin, line));

    const int aTokenMultiplier = 3;
    const int bTokenMultiplier = 1;
    const int tokenLimit = 100;
    const bool activeLimit = false; //part1: true | part2: false
    long tokens = 0;


    for(ClawMachine &machine : clawMachines) {

        clawMachineCorrection(&machine);

        long s, t; //s equals A button presses and t equals B button presses

        bool solutionFound = solveWithCramersRule(machine, &s, &t);
        bool solutionCheck = checkSolution(machine, s, t);
        if(solutionFound) {
            if( solutionCheck && (!activeLimit || (s <= tokenLimit && t <= tokenLimit)) ) {
                std::cout << "\e[33m";
                tokens += s * aTokenMultiplier + t * bTokenMultiplier;
            }
            std::cout << "Solution check: " << solutionCheck << " Solution: s=" << s << " t=" << t << "\e[0m" << std::endl;
        }
        else {
            std::cout << "No solution found!" << std::endl;
        }

    }

    if(activeLimit) {
        std::cout << "Tokens limit (<=): \e[31m" << tokenLimit << "\e[0m" << std::endl;
    }
    std::cout << "Tokens needed: " << tokens << std::endl;

}