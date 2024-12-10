#include <iostream>
#include <vector>
#include <set>

enum ColorfulColor {
    Red = 31,
    Green = 32,
    Yellow = 33,
    Blue = 34,
    Magenta = 35,
    Cyan = 36,
    LightRed = 91,
    LightGreen = 92,
    LightYellow = 93,
    LightBlue = 94,
    LightMagenta = 95,
    LightCyan = 96
};

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

    bool inBoundaries(int xHighBoundary, int yHighBoundary, int xLowBoundary=0, int yLowBoundary=0) {
        return !(x < xLowBoundary || x > xHighBoundary || y < yLowBoundary || y > yHighBoundary);
    }

    friend std::ostream& operator<<(std::ostream &os, const Coord& coord) {
        return os << "(" << coord.x << "," << coord.y << ")";
    }

    bool operator<(const Coord& other) const {
        if(x < other.x) { return true; }
        else if ( x == other.x && y < other.y ) { return true; }
        return false;
    }
};

//returns vector of disttint 9-height positions reachable from trail
std::set<Coord> searchTrail(std::vector<std::vector<short>> map, Coord startSearchPoint, int* rating=nullptr, std::vector<std::vector<std::string>> *coloredMap = nullptr, ColorfulColor color=Green, int targetHeight=0) {
    std::set<Coord> trailEnds;

    if( !startSearchPoint.inBoundaries(map.size()-1, map.at(0).size()-1)) {
        return trailEnds;
    }

    short currentHeight = map.at(startSearchPoint.x).at(startSearchPoint.y);

    if(currentHeight != targetHeight) {
        return trailEnds;
    }

    if(currentHeight == 9) {
        trailEnds.insert(startSearchPoint);

        if(coloredMap != nullptr) {
            coloredMap->at(startSearchPoint.x).at(startSearchPoint.y) = "\e[" + std::to_string(color) + "m" + coloredMap->at(startSearchPoint.x).at(startSearchPoint.y) + "\e[0m";
        }

        if(rating != nullptr) {
            *rating += 1;
        }

        return trailEnds;
    }

    Coord up   (startSearchPoint.x-1, startSearchPoint.y  );
    Coord right(startSearchPoint.x  , startSearchPoint.y+1);
    Coord down (startSearchPoint.x+1, startSearchPoint.y  );
    Coord left (startSearchPoint.x  , startSearchPoint.y-1);

    std::set<Coord> upTrailEnds    = searchTrail(map, up   , rating, coloredMap, color, currentHeight+1);
    std::set<Coord> rightTrailEnds = searchTrail(map, right, rating, coloredMap, color, currentHeight+1);
    std::set<Coord> downTrailEnds  = searchTrail(map, down , rating, coloredMap, color, currentHeight+1);
    std::set<Coord> leftTrailEnds  = searchTrail(map, left , rating, coloredMap, color, currentHeight+1);

    trailEnds.insert( upTrailEnds.begin()   , upTrailEnds.end()    );
    trailEnds.insert( rightTrailEnds.begin(), rightTrailEnds.end() );
    trailEnds.insert( downTrailEnds.begin() , downTrailEnds.end()  );
    trailEnds.insert( leftTrailEnds.begin() , leftTrailEnds.end()  );

    if(!trailEnds.empty() && coloredMap != nullptr) {
        coloredMap->at(startSearchPoint.x).at(startSearchPoint.y) = "\e[" + std::to_string(color) + "m" + coloredMap->at(startSearchPoint.x).at(startSearchPoint.y) + "\e[0m";
    }

    return trailEnds;
}

std::vector<Coord> searchHeightOnMap(std::vector<std::vector<short>> map, short height) {
    std::vector<Coord> positionsWithHeight;

    for(int x = 0; x < map.size(); ++x) {
        for(int y = 0; y < map.at(x).size(); ++y) {
            if(map.at(x).at(y) == height) {
                positionsWithHeight.push_back(Coord(x,y));
            }
        }
    }

    return positionsWithHeight;
}

int main() {

    std::string inputLine;
    std::vector<std::vector<char>> matrix;

    while(std::getline(std::cin, inputLine)) {
        std::vector<char> l(inputLine.begin(), inputLine.end());
        matrix.push_back(l);
    }

    //conevert char matrix to short matrix
    std::vector<std::vector<short>> map;
    for(int x = 0; x < matrix.size(); ++x) {
        std::vector<short> column;
        for(int y = 0; y < matrix.at(x).size(); ++y) {
            column.push_back(matrix.at(x).at(y) - '0');
        }
        map.push_back(column);
    }
    printMatrix(map);

    //convert char matrix to string matrix to color all trails 
    std::vector<std::vector<std::string>> coloredMap;
    for(int x = 0; x < matrix.size(); ++x) {
        std::vector<std::string> column;
        for(int y = 0; y < matrix.at(x).size(); ++y) {
            column.push_back(std::string(1, matrix.at(x).at(y)));
        }
        coloredMap.push_back(column);
    }

    //Part 1 and 2 combined
    std::vector<Coord> possibleTrailStartPoints = searchHeightOnMap(map, 0);
    int rating = 0;
    std::vector<Coord> trailEnds;
    for(size_t i = 0; i < possibleTrailStartPoints.size(); ++i) {
        std::set<Coord> currentTrailEnds = searchTrail(map, possibleTrailStartPoints.at(i), &rating, &coloredMap, ColorfulColor((i+1) % 7 + 30 ));
        trailEnds.insert( trailEnds.end(), currentTrailEnds.begin(), currentTrailEnds.end() );
    }

    printMatrix(coloredMap);

    std::cout << "Score: " << trailEnds.size() << std::endl;
    std::cout << "Rating: " << rating << std::endl;

    return 0;
}