#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime> 
#define yeet throw

//for debugging
template <typename T>
void printVec(std::vector<T> vec) {
    for (T element : vec) {
        std::cout << element << ' ';
    }
    std::cout << std::endl;
}

//for debugging
template <typename T>
void printMatrix(std::vector<std::vector<T>> mat, int standardColor=0, int highlightX=-1, int highlightY=-1, int highlightCode=0) {
    std::cout << "\e[" << standardColor << "m";
    for (int x = 0; x < mat.size(); ++x) {
        for(int y = 0; y < mat.at(x).size(); ++y) {
            if(x == highlightX && y == highlightY) {
                std::cout << "\e[" << highlightCode << "m";
            }
            std::cout << mat.at(x).at(y) << " "; 
            std::cout << "\e[" << standardColor << "m";
        }
        std::cout << std::endl;
    }
    std::cout << "\e[0m" << std::endl;
}

struct Coord {
    int x;
    int y;
};

//returns the first hit of the searched char
Coord findCharInMatrix(std::vector<std::vector<char>> matrix, char searchChar) {
    for(int x = 0; x < matrix.size(); ++x) {
        for(int y = 0; y < matrix.at(x).size(); ++y) {
            if(matrix.at(x).at(y) == searchChar) {
                return {x,y};
            }
        }
    }
    return {-1,-1};
}

Coord findCharAtCoordInDirection(std::vector<std::vector<char>> matrix, char searchChar, int x, int y, int dir) {

    bool found = false;
    Coord retCoord;
    retCoord.x = -1;
    retCoord.y = -1;
    int i = 0;
    while(!found) {

        switch (dir)
        {
        case 0: //right
            retCoord = { x, y+i };
            break;
        case 1: //down right
            retCoord = { x+i, y+i };
            break;
        case 2: //down
            retCoord = { x+i, y };
            break;
        case 3: //down left
            retCoord = { x+i, y-i };
            break;
        case 4: //left
            retCoord = { x, y-i };
            break;
        case 5: //up left
            retCoord = { x-i, y-i };
            break;
        case 6: //up
            retCoord = { x-i, y };
            break;
        case 7: //up right
            retCoord = { x-i, y+i };
            break;
        default:
            yeet std::invalid_argument(std::to_string(dir).append(" is not a valid direction identifier"));
            break;
        }

        try {
            found = (searchChar == matrix.at(retCoord.x).at(retCoord.y));
        }
        catch (std::out_of_range e) {
            return {-1,-1};
        }

        ++i;
    }
    return retCoord;
}

int turn90DegRight(int dir) {
    return (dir + 2) % 8;
}

std::vector<std::vector<char>> editMatrixVerticalLine(std::vector<std::vector<char>> matrix, char editChar, int iStart, int iEnd, int column) {

    int _iStart = std::min(iStart, iEnd);
    int _iEnd = std::max(iStart, iEnd);

    for(int i = _iStart; i <= _iEnd; ++i) {
        matrix.at(i).at(column) = editChar;
    }
    return matrix;
}

std::vector<std::vector<char>> editMatrixHorizontalLine(std::vector<std::vector<char>> matrix, char editChar, int iStart, int iEnd, int row) {
    
    int _iStart = std::min(iStart, iEnd);
    int _iEnd = std::max(iStart, iEnd);

    for(int i = _iStart; i <= _iEnd; ++i) {
        matrix.at(row).at(i) = editChar;
    }
    return matrix;
}

std::vector<std::vector<char>> markGuardPath(std::vector<std::vector<char>> matrix, bool *trapped=nullptr) {
    if (trapped != nullptr) *trapped = false;
    int countSameCoord = 0;

    int currentDirection = 6; //up
    Coord currentCoord = findCharInMatrix(matrix, '^');

    bool end = false;
    Coord nextCoord;
    while(!end) {
        nextCoord = findCharAtCoordInDirection(matrix, '#', currentCoord.x, currentCoord.y, currentDirection);
        if (nextCoord.x < 0 && nextCoord.y < 0 ) {

            switch (currentDirection)
            {
            case 0: //right
                nextCoord.x = currentCoord.x;
                nextCoord.y = matrix.at( currentCoord.x ).size();
                break;

            case 2: //down
                nextCoord.x = matrix.size();
                nextCoord.y = currentCoord.y;
                break;

            case 4: //left
                nextCoord.x = currentCoord.x;
                nextCoord.y = -1;
                break;

            case 6: //up
                nextCoord.x = -1;
                nextCoord.y = currentCoord.y;
                break;
            
            default:
                yeet std::invalid_argument(std::to_string(currentDirection).append(" is not a valid direction identifier in this context"));
                break;
            }
            end = true;
            if (trapped != nullptr) *trapped = false;
        }

        switch (currentDirection)
        {
        case 0: //right
            nextCoord.x = nextCoord.x;
            nextCoord.y = nextCoord.y-1;
            break;

        case 2: //down
            nextCoord.x = nextCoord.x-1;
            nextCoord.y = nextCoord.y;
            break;

        case 4: //left
            nextCoord.x = nextCoord.x;
            nextCoord.y = nextCoord.y+1;
            break;

        case 6: //up
            nextCoord.x = nextCoord.x+1;
            nextCoord.y = nextCoord.y;
            break;
        
        default:
            yeet std::invalid_argument(std::to_string(currentDirection).append(" is not a valid direction identifier in this context"));
            break;
        }

        //check if guard is trapped
        char nextCoordChar = matrix.at(nextCoord.x).at(nextCoord.y);
        if( 
            (nextCoordChar == '+' || nextCoordChar == '-' || nextCoordChar == '|') && !end
        ) {
            ++countSameCoord;
        } else {
            countSameCoord = 0;
        }

        if( countSameCoord > 3 ) { //if guards next coordinates are 3 times coordinates they already visited, they are trapped
            if (trapped != nullptr) *trapped = true;
            end = true;
        }



        if ( currentDirection == 6 || currentDirection == 2) {//vertical
            matrix = editMatrixVerticalLine(matrix, '|', currentCoord.x, nextCoord.x, currentCoord.y);
        }
        if ( currentDirection == 4 || currentDirection == 0) {//horizontal
            matrix = editMatrixHorizontalLine(matrix, '-', currentCoord.y, nextCoord.y, currentCoord.x);
        }
        matrix.at(currentCoord.x).at(currentCoord.y) = '+';
        matrix.at(nextCoord.x).at(nextCoord.y) = '+';

        currentCoord = nextCoord;
        currentDirection = turn90DegRight(currentDirection);
    }
    
    return matrix;
}

int countCharInMatrix(std::vector<std::vector<char>> matrix, char searchChar) {

    int count = 0;

    for(int x = 0; x < matrix.size(); ++x) {

        for(int y = 0; y < matrix.at(x).size(); ++y) {
            count = (matrix.at(x).at(y) == searchChar) ? count+1 : count;            
        }
    }
    return count;
}

//brute force method (kinda)
int countPossibleObstructionPlacmentOptions(std::vector<std::vector<char>> matrix, bool print=false) {

    Coord initGuardCoord = findCharInMatrix(matrix, '^');
    std::vector<std::vector<char>> markedPathMatrix = markGuardPath(matrix);

    int count = 0;

    for(int x = 0; x < matrix.size(); ++x) {

        for(int y = 0; y < matrix.at(x).size(); ++y) {

            char matrixChar = markedPathMatrix.at(x).at(y);
            if(x == initGuardCoord.x && y == initGuardCoord.y) { continue; } //can't place at guzard initi position
            if(matrixChar == '#') { continue; }
            if( !(matrixChar == '+' || matrixChar == '-' || matrixChar == '|') ) { continue; }

            std::vector<std::vector<char>> modMatrix = matrix;
            modMatrix.at(x).at(y) = '#';

            bool trapped = false;
            modMatrix = markGuardPath(modMatrix, &trapped); //will test if guard is trapped
            
            
            if(trapped) {
                ++count;
                if(print) {
                    printMatrix(modMatrix, 31 + (count) % 7, x, y, 31 + (count+1) % 7);
                    std::cout << "\e[0m";
                }
            }

        }
    }
    return count;
}

int main() {

    auto start = std::chrono::system_clock::now();

    std::vector<std::vector<char>> matrix;
    std::string inputLine;

    //read line by line
    while(std::getline(std::cin, inputLine)) {
        std::vector<char> l(inputLine.begin(), inputLine.end());
        matrix.push_back(l);
    }
    
    printMatrix(matrix);


    std::vector<std::vector<char>> markedPathMatrix = markGuardPath(matrix);
    printMatrix( markedPathMatrix );

    int countDistinctPostitions = countCharInMatrix(markedPathMatrix, '+') + countCharInMatrix(markedPathMatrix, '|') + countCharInMatrix(markedPathMatrix, '-');
    std::cout << "Distinct positions                 : " << countDistinctPostitions << std::endl;
    
    int countObstructionPositions = countPossibleObstructionPlacmentOptions(matrix);
    std::cout << "Different positions for obstruction: " << countObstructionPositions << std::endl;


    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cout << "Elapsed time: \e[33m" << elapsed_seconds.count() << "s \e[0m" << std::endl;
    
    return 0;
}