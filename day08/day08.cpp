#include <iostream>
#include <vector>
#include <set>
#include <chrono>

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

struct Coord {
    int x;
    int y;

    Coord() { x, y = -1, -1; }
    Coord(int x, int y) : x(x), y(y) {}

    void invert() {
        x = -x;
        y = -y;
    }

    bool inBoundaries(int xHighBoundary, int yHighBoundary, int xLowBoundary=0, int yLowBoundary=0) {
        return !(x < xLowBoundary || x > xHighBoundary || y < yLowBoundary || y > yHighBoundary);
    }

    friend std::ostream& operator<<(std::ostream &os, const Coord& coord) {
        return os << "(" << coord.x << "," << coord.y << ")";
    }

    Coord operator+(const Coord &other) const {
        return Coord(x+other.x, y+other.y);
    }

    Coord operator-(const Coord &other) const {
        return Coord(x-other.x, y-other.y);
    }

    bool operator<(const Coord& other) const {
        if(x < other.x) { return true; }
        else if ( x == other.x && y < other.y ) { return true; }
        return false;
    }
};

std::vector<Coord> getCoordinatesOfFrequencyInMatrix(std::vector<std::vector<char>> matrix, char freqChar) {
    std::vector<Coord> coordinates;
    for(int x = 0; x < matrix.size(); ++x) {
        for(int y = 0; y < matrix.at(x).size(); ++y) {
            if(matrix.at(x).at(y) == freqChar) {
                coordinates.push_back(Coord(x,y));
            }
        }
    }
    return coordinates;
}

std::vector<Coord> determineAntinodesOfFrequencyCoordinates(std::vector<Coord> frequencyCoordinates) {
    std::vector<Coord> antinodeCoordinates;
    for(int i = 0; i < frequencyCoordinates.size(); ++i) {
        for(int j = i+1; j < frequencyCoordinates.size(); ++j) {
            Coord coordA = frequencyCoordinates.at(i);
            Coord coordB = frequencyCoordinates.at(j);

            Coord diffVectorBA = coordA - coordB;       //       B --> A
            Coord antinodeA    = coordA + diffVectorBA; //       B --> A --> #
            
            Coord diffVectorAB = coordB - coordA;       //       B <-- A
            Coord antinodeB = coordB + diffVectorAB;    // # <-- B <-- A

            antinodeCoordinates.push_back(antinodeA);
            antinodeCoordinates.push_back(antinodeB);
        }
    }

    return antinodeCoordinates;
}

std::vector<Coord> determineAntinodesOfFrequencyCoordinates2(std::vector<Coord> frequencyCoordinates, int xBoundary, int yBoundary) {
    std::vector<Coord> antinodeCoordinates;
    for(int i = 0; i < frequencyCoordinates.size(); ++i) {
        for(int j = i+1; j < frequencyCoordinates.size(); ++j) {
            Coord coordA = frequencyCoordinates.at(i);
            Coord coordB = frequencyCoordinates.at(j);

            Coord diffVectorBA = coordA - coordB;       //       B --> A
            Coord diffVectorAB = coordB - coordA;       //       B <-- A

            Coord startingPoint = coordA;
            

            while(startingPoint.inBoundaries(xBoundary, yBoundary)) {
                startingPoint = startingPoint + diffVectorAB;
            }
            startingPoint = startingPoint + diffVectorBA;

            Coord antinodePoint = startingPoint;
            while(antinodePoint.inBoundaries(xBoundary, yBoundary)) {
                antinodeCoordinates.push_back(antinodePoint);
                antinodePoint = antinodePoint + diffVectorBA;
            }
        }
    }

    return antinodeCoordinates;
}

std::vector<Coord> filterCoordinatesWithBoundaries(std::vector<Coord> coordinates, int xBoundary, int yBoundary) {
    for(int i = 0; i < coordinates.size(); ++i) {
        if( !coordinates.at(i).inBoundaries(xBoundary, yBoundary) ) {
            coordinates.erase(coordinates.begin() + i);
            --i;
        }
    }
    return coordinates;
}

std::vector<Coord> determineAntinodesOfAllFrequencies(std::vector<std::vector<char>> matrix, std::string frequencies="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") {
    std::vector<Coord> antinodeCoordinates;
    for(char frequencyChar : frequencies) {
        std::vector<Coord> frequencyCoordinates = getCoordinatesOfFrequencyInMatrix(matrix, frequencyChar);
        std::vector<Coord> frequencyAntinodeCoordinates =  filterCoordinatesWithBoundaries( determineAntinodesOfFrequencyCoordinates(frequencyCoordinates), matrix.size()-1, matrix.at(0).size()-1);
        antinodeCoordinates.insert(antinodeCoordinates.end(), frequencyAntinodeCoordinates.begin(), frequencyAntinodeCoordinates.end());
    }
    return antinodeCoordinates;
}

std::vector<Coord> determineAntinodesOfAllFrequencies2(std::vector<std::vector<char>> matrix, std::string frequencies="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") {
    std::vector<Coord> antinodeCoordinates;
    for(char frequencyChar : frequencies) {
        std::vector<Coord> frequencyCoordinates = getCoordinatesOfFrequencyInMatrix(matrix, frequencyChar);
        std::vector<Coord> frequencyAntinodeCoordinates = filterCoordinatesWithBoundaries( 
                                                            determineAntinodesOfFrequencyCoordinates2(frequencyCoordinates, matrix.size()-1, matrix.at(0).size()-1),
                                                            matrix.size()-1, matrix.at(0).size()-1
                                                          );
        antinodeCoordinates.insert(antinodeCoordinates.end(), frequencyAntinodeCoordinates.begin(), frequencyAntinodeCoordinates.end());
    }
    return antinodeCoordinates;
}

std::vector<std::vector<char>> printCoordinatesToMatrix(std::vector<std::vector<char>> matrix, std::vector<Coord> coordinates, char c, bool overrideExistingChars=false) {
    for(int i = 0; i < coordinates.size(); ++i) {
        char existingChar = matrix.at(coordinates.at(i).x) .at(coordinates.at(i).y);
        if (existingChar == '.' || overrideExistingChars) {
            matrix.at(coordinates.at(i).x) .at(coordinates.at(i).y) = c;
        }
    }
    return matrix;
}

int main() {

    // TIME START
    auto start = std::chrono::system_clock::now();

    std::vector<std::vector<char>> matrix;
    std::string inputLine;

    while(std::getline(std::cin, inputLine)) {
        std::vector<char> l(inputLine.begin(), inputLine.end());
        matrix.push_back(l);
    }
    printMatrix(matrix);

    // PART 1 -------------------------------------------------------------------------------------------------------
    std::vector<Coord> antinodeCoordinates = determineAntinodesOfAllFrequencies(matrix);
    std::vector<std::vector<char>> antinodeMatrix = printCoordinatesToMatrix(matrix, antinodeCoordinates, '#');

    //convert vector antinodeCoordinates to set antinodeCoordinatesSet to get rid of duplicates
    std::set<Coord> antinodeCoordinatesSet;
    size_t size1 = antinodeCoordinates.size();
    for( unsigned i = 0; i < size1; ++i ) antinodeCoordinatesSet.insert( antinodeCoordinates.at(i) );
    antinodeCoordinates.assign( antinodeCoordinatesSet.begin(), antinodeCoordinatesSet.end() );

    printMatrix(antinodeMatrix);
    std::cout << "Unique antinode locations 1: " << antinodeCoordinatesSet.size() << std::endl;

    // PART 2 -------------------------------------------------------------------------------------------------------
    std::vector<Coord> antinodeCoordinates2 = determineAntinodesOfAllFrequencies2(matrix);
    std::vector<std::vector<char>> antinodeMatrix2 = printCoordinatesToMatrix(matrix, antinodeCoordinates2, '#');

    //convert vector antinodeCoordinates2 to set antinodeCoordinatesSet2 to get rid of duplicates
    std::set<Coord> antinodeCoordinatesSet2;
    size_t size2 = antinodeCoordinates2.size();
    for( unsigned i = 0; i < size2; ++i ) antinodeCoordinatesSet2.insert( antinodeCoordinates2.at(i) );
    antinodeCoordinates2.assign( antinodeCoordinatesSet2.begin(), antinodeCoordinatesSet2.end() );

    printMatrix(antinodeMatrix2);
    std::cout << "Unique antinode locations 2: " << antinodeCoordinatesSet2.size() << std::endl;

    // TIME STOP
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cout << "Elapsed time: \e[33m" << elapsed_seconds.count() << "s \e[0m" << std::endl;
    
    return 0;
}