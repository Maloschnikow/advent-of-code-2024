#include <iostream>
#include <vector>
#include <algorithm>
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
void printMatrix(std::vector<std::vector<T>> mat) {
    for (std::vector<T> column : mat) {
        for(T element : column) {
            std::cout << element << " "; 

        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool checkStringAtCoordInDirection(std::vector<std::vector<char>> matrix, std::string searchStr, int x, int y, int dir) {

    bool ret = true;

    for(int i = 0; i < searchStr.size(); ++i) {

        int dirCount = 0;

        switch (dir)
        {
        case 0:
            ret = (y+i < matrix.at(x).size())                        ? ( ret && (searchStr.at(i) == matrix.at(x  ).at(y+i)) ) : false;
            break;
        case 1:
            ret = (x+i < matrix.size() && y+i < matrix.at(x).size()) ? ( ret && (searchStr.at(i) == matrix.at(x+i).at(y+i)) ) : false;
            break;
        case 2:
            ret = (x+i < matrix.size())                              ? ( ret && (searchStr.at(i) == matrix.at(x+i).at(y  )) ) : false;
            break;
        case 3:
            ret = (x+i < matrix.size() && y-i >= 0)                  ? ( ret && (searchStr.at(i) == matrix.at(x+i).at(y-i)) ) : false;
            break;
        case 4:
            ret = (y-i >= 0)                                         ? ( ret && (searchStr.at(i) == matrix.at(x  ).at(y-i)) ) : false;
            break;
        case 5:
            ret = (x-i >= 0 && y-i >= 0)                             ? ( ret && (searchStr.at(i) == matrix.at(x-i).at(y-i)) ) : false;
            break;
        case 6:
            ret = (x-i >= 0)                                         ? ( ret && (searchStr.at(i) == matrix.at(x-i).at(y  )) ) : false;
            break;
        case 7:
            ret = (x-i >= 0 && y+i < matrix.at(x).size())            ? ( ret && (searchStr.at(i) == matrix.at(x-i).at(y+i)) ) : false;
            break;
        default:
            yeet std::invalid_argument(std::to_string(dir).append(" is not a valid direction identifier"));
            break;
        }

        if( !ret ) {
            return false;
        }
    }
    return ret;
}

int countStringAtStartingCoordsInMatrix(std::vector<std::vector<char>> matrix, std::string searchStr, int x, int y, bool onlyDiagonal=false) {
    int count = 0;

    bool dir[8];

    for(int j = 0; j < 8; ++j) {
        if(onlyDiagonal && j % 2 != 0) { //even indexes are not diagonal
            dir[j] = checkStringAtCoordInDirection(matrix, searchStr, x, y, j);

        } else if (onlyDiagonal) {
            dir[j] = false;
        } else {
            dir[j] = checkStringAtCoordInDirection(matrix, searchStr, x, y, j);
        }
        count += dir[j];
    }
    return count;
}

int countStringInMatrix(std::vector<std::vector<char>> matrix, std::string searchStr, bool onlyDiagonal=false) {

    int count = 0;

    for(int x = 0; x < matrix.size(); ++x) {

        for(int y = 0; y < matrix.at(x).size(); ++y) {
            count += countStringAtStartingCoordsInMatrix(matrix, searchStr, x, y, onlyDiagonal);            
        }
    }
    return count;
}


int countDiagonalStringCrossesInMatrix(std::vector<std::vector<char>> matrix, std::string searchStr) {
    
    int count = 0;

    for(int x = 0; x < matrix.size() - searchStr.size() + 1; ++x) {

        for(int y = 0; y < matrix.at(x).size() - searchStr.size() + 1; ++y) {

            //TODO make a sub matrix with the dimensions of searchStr size and count diagonals in this sub matrix
            std::vector<std::vector<char>> subMatrix;
            for(int i = 0; i < searchStr.size(); ++i) {
                std::vector<char> l(matrix.at(x+i).begin()+y, matrix.at(x+i).begin()+y + searchStr.size());
                subMatrix.push_back(l);
            }

            if(countStringInMatrix(subMatrix, searchStr, true) == 2) { //two diagonal matches in a sub matrix mean there is one cross of the word
                count +=1;
            }
        }
    }
    return count;
}

int main() {

    std::vector<std::vector<char>> matrix;
    std::string inputLine;

    //read line by line
    while(std::getline(std::cin, inputLine)) {
        std::vector<char> l(inputLine.begin(), inputLine.end());
        matrix.push_back(l);
    }
    
    printMatrix(matrix);

    std::cout << "XMAS:  " << countStringInMatrix(matrix, "XMAS") << std::endl;
    std::cout << "MAS:   " << countStringInMatrix(matrix, "MAS") << std::endl;
    std::cout << "X-MAS: " << countDiagonalStringCrossesInMatrix(matrix, "MAS") << std::endl;

    return 0;
}