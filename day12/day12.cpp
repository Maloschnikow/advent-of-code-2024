#include "../helper.hpp"
#include <iostream>
#include <vector>
#include <set>
#define yeet throw

struct Plant {
    char type; //plant type from puzzle input
    int regionID; // maps the plant to a region

    Plant(char _type='\0', int _regionID=-1)
    :   type(_type), 
        regionID(_regionID)
    {}

    friend std::ostream& operator<<(std::ostream &os, const Plant& plant) {
        return os << plant.type;
    }
};

struct Coord {
    int x;
    int y;

    Coord() { x, y = -1, -1; }
    Coord(int x, int y) : x(x), y(y) {}

    Coord getUp(int steps=1) const {
        return Coord(x-1,y  );
    }
    Coord getRight(int steps=1) const {
        return Coord(x  ,y+1);
    }
    Coord getDown(int steps=1) const {
        return Coord(x+1,y);
    }
    Coord getLeft(int steps=1) const {
        return Coord(x  ,y-1);
    }

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

    Coord operator+(const Coord &other) const {
        return Coord(x+other.x, y+other.y);
    }

    Coord operator-(const Coord &other) const {
        return Coord(x-other.x, y-other.y);
    }

    bool operator==(const Coord &other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Coord &other) const {
        return !(*this == other);
    }
};

/**
 * @param matrix Matrix in which the neighbors will be looked for
 * @param coord  Coordinates of the "home" plant
 * 
 * @returns Set of Coordinates (which are neighbors), where the plant type is the same as in home and does not yet belong to a region
 */
std::set<Coord> getEqualNeighbors(std::vector<std::vector<Plant>> &matrix, Coord coord) {
    Coord up = coord.getUp();
    Coord right = coord.getRight();
    Coord down = coord.getDown();
    Coord left = coord.getLeft();
    std::set<Coord> neighbors;
    char plantType = matrix.at(coord.x).at(coord.y).type;

    int matrixMaxX = matrix.size()-1;
    int matrixMaxY = matrix.at(0).size()-1;

    if(up.inBoundaries(matrixMaxX, matrixMaxY)    && matrix.at(up.x).at(up.y).type       == plantType && matrix.at(up.x).at(up.y).regionID       == -1) {
        neighbors.insert(up);
    }
    if(right.inBoundaries(matrixMaxX, matrixMaxY) && matrix.at(right.x).at(right.y).type == plantType && matrix.at(right.x).at(right.y).regionID == -1) {
        neighbors.insert(right);
    }
    if(down.inBoundaries(matrixMaxX, matrixMaxY)  && matrix.at(down.x).at(down.y).type   == plantType && matrix.at(down.x).at(down.y).regionID   == -1) {
        neighbors.insert(down);
    }
    if(left.inBoundaries(matrixMaxX, matrixMaxY)  && matrix.at(left.x).at(left.y).type   == plantType && matrix.at(left.x).at(left.y).regionID   == -1) {
        neighbors.insert(left);
    }
    return neighbors;
}

/**
 * @param matrix Matrix that contains the region
 * @param region Region of which the perimeter should be calculated
 * 
 * @returns Perimeter of the given region
 */
int calculateRegionPerimeter(std::vector<std::vector<Plant>> &matrix, std::set<Coord> &region) {
    int perimeter = 0;
    for(Coord coord : region) {
        perimeter += 4 - getEqualNeighbors(matrix, coord).size();
    }
    return perimeter;
}

/**
 * @param matrix Matrix that contains the region
 * @param region Region of which the region's plant type should be returned
 * 
 * @returns Plant type of the given region
 */
char getRegionPlantType(std::vector<std::vector<Plant>> &matrix, std::set<Coord> &region) {
    std::set<Coord>::iterator c = region.begin();
    return matrix.at(c->x).at(c->y).type;
}

//Is the Dijkstra? Mhmmmmm....
/**
 * @param matrix           Pointer to a matrix in which a region should be fully explored
 * @param startSearchCoord Coordinates to start exploring from
 * @param regionID         Explored plants will receive this regionID (-1 means not explored, so the algorithm won't work is this is set to -1)
 * @param dontModify       If set to true the given matrix will be copied before the region in it will be explored to not change the data in the given matrix.
 * 
 * @returns Vector of coordinates of all the plants in the explored region
 */
std::set<Coord> exploreRegion(std::vector<std::vector<Plant>> *matrix, Coord startSearchCoord, int regionID=0, bool dontModify=true) { //regionID should not be -1
    std::vector<std::vector<Plant>> *searchMatrix;
    std::vector<std::vector<Plant>> copyMatrix;
    std::set<Coord> neighbors;
    std::set<Coord> region;

    // If the matrix should not be modified copy it
    if(dontModify) {
        copyMatrix   = *matrix;
        searchMatrix = &copyMatrix;
    }
    else {
        searchMatrix = matrix;
    }

    if (searchMatrix->at(startSearchCoord.x).at(startSearchCoord.y).regionID != -1) {
        return region; //aborts if the specified coordinate already belongs to a idendified region (would not be inserted anyway, because now a set is used)
    } 
    region.insert(startSearchCoord);

    neighbors = getEqualNeighbors(*searchMatrix, startSearchCoord);
    searchMatrix->at(startSearchCoord.x).at(startSearchCoord.y).regionID = regionID; //mark as detected

    for(Coord neighborCoord : neighbors) {
        std::set<Coord> neighborRegion = exploreRegion(searchMatrix, neighborCoord, regionID, false);
        region.insert(neighborRegion.begin(), neighborRegion.end());
    }
    return region;
}

/**
 * @param matrix Matrix in which regions should be searched
 * 
 * @returns Vector of regions that can be found in the given matrix
 */
std::vector<std::set<Coord>> findRegions(std::vector<std::vector<Plant>> matrix) {
    std::vector<std::set<Coord>> regions;
    int matrixMaxX = matrix.size()-1;
    int matrixMaxY = matrix.at(0).size()-1;
    Coord searchCoord(0,0);

    int regionID = 0; //region ids start at 0 and increment with every region found
    while(searchCoord.inBoundaries(matrixMaxX, matrixMaxY)) {
        if(matrix.at(searchCoord.x).at(searchCoord.y).regionID == -1) {

            std::set<Coord> region = exploreRegion(&matrix, searchCoord, regionID, false);
            regions.push_back(region);

            ++regionID;
        }
        int oldY = searchCoord.y;
        searchCoord.y = (searchCoord.y + 1) % (matrixMaxY+1);
        searchCoord.x = searchCoord.x + ((oldY+1) / (matrixMaxY+1));
    }

    return regions;
}

/**
 * @param coord Coordinates from which a step is taken in direction
 * @param dir   Direction in which the step will be taken
 * 
 * @returns Coordinate in a specific direction
 */
Coord getDirectionCoordinate(Coord coord, int dir) {
    switch (dir)
    {
    case 0: //right
        return coord.getRight();
        break;
    case 2: //down
        return coord.getDown();
        break;
    case 4: //left
        return coord.getLeft();
        break;
    case 6: //up
        return coord.getUp();
        break;
    default:
        yeet std::invalid_argument(std::to_string(dir).append(" is not a valid direction identifier"));
        break;
    }
}

/**
 * @param matrix    Matrix in which the check takes place
 * @param plantType Plant type that is checked for
 * @param coord     Coordinates from which the check will be taken in a direction
 * @param dir       Direction in which the check will be taken
 * 
 * @returns False if the testes coordinate is out of range of the matrix or the plant type at the tested coordinate doesnt match
 */
bool checkPlantTypeInDirection(std::vector<std::vector<Plant>> matrix, char plantType, Coord coord, int dir) {

    Coord testCoord = getDirectionCoordinate(coord, dir);

    try {
        return (plantType == matrix.at(testCoord.x).at(testCoord.y).type);
    }
    catch (std::out_of_range e) {
        return false;
    }
}

/**
 * @param matrix Matrix in which the "search" takes place
 * @param region The region of which the border will be returned
 * @param dir    Direction the border should point in
 * 
 * @returns Set of coordinates which mark the border of the region which points in the given direction
 */
std::set<Coord> getCoordinatesWithBorderInDirection(std::vector<std::vector<Plant>> &matrix, std::set<Coord> &region, int dir) {
    int matrixMaxX = matrix.size()-1;
    int matrixMaxY = matrix.at(0).size()-1;
    char plantType = getRegionPlantType(matrix, region);

    std::set<Coord> coordinatesWithBorderInDir;

    for(Coord coord : region) {
        if ( !checkPlantTypeInDirection(matrix, plantType, coord, dir)) {
            coordinatesWithBorderInDir.insert(coord);
        };
    }
    return coordinatesWithBorderInDir;
}

/**
 * @param matrix                           Matrix in which the counting takes place
 * @param coordinatesWithBorderInDirection Set of border coordinates that all point in the same direction
 * 
 * @returns Amount of actual sides that point in the same direction as the border coordinates given
 */
int countSidesOfCoordinatesWithBorderInDirection(std::vector<std::vector<Plant>> &matrix, std::set<Coord> &coordinatesWithBorderInDirection) {
    int matrixMaxX = matrix.size()-1;
    int matrixMaxY = matrix.at(0).size()-1;

    std::vector<std::vector<Plant>> borderMatrix;

    for(int x = 0; x <= matrixMaxX; ++x) {
        std::vector<Plant> column;
        for(int y = 0; y <= matrixMaxX; ++y) {
            column.push_back(Plant('.'));
        }
        borderMatrix.push_back(column);
    }

    for(Coord coord : coordinatesWithBorderInDirection) {
        borderMatrix.at(coord.x).at(coord.y) = matrix.at(coord.x).at(coord.y);
    }

    int sideCount = 0;
    int sideID = 0;
    for(Coord coord : coordinatesWithBorderInDirection) {
        if ( !exploreRegion(&borderMatrix, coord, sideID, false).empty()) {
            ++sideCount;
        }
        ++sideID;
    }

    return sideCount;
}

/**
 * @param matrix Matrix in which contains the given region
 * @param region Region of which the actual sides should be counted
 * 
 * @returns Amount of all actual sides of the region
 */
int countSidesOfRegion(std::vector<std::vector<Plant>> &matrix, std::set<Coord> &region) {
    int sideCount = 0;
    int dir = 0;
    for (int d = dir; d <= 6; d+=2) {
        std::set<Coord> borderCoordsInDir = getCoordinatesWithBorderInDirection(matrix, region, d);
        sideCount += countSidesOfCoordinatesWithBorderInDirection(matrix, borderCoordsInDir);
    }
    return sideCount;
}

int main() {

    //Read the input
    std::string inputLine;
    std::vector<std::vector<Plant>> matrix;
    while(std::getline(std::cin, inputLine)) {
        std::vector<Plant> l;
        for(char c : inputLine) {
            l.push_back(Plant(c));
        }
        matrix.push_back(l);
    }
    helper::printMatrix(matrix);

    //Part 1

    std::vector<std::set<Coord>> regions = findRegions(matrix);

    int sumPrice1 = 0;
    int sumPrice2 = 0;
    for(std::set<Coord>& region : regions) {
        char regionChar = getRegionPlantType(matrix, region);
        int perimeter   = calculateRegionPerimeter(matrix, region);
        int area        = region.size();
        int fenceCost1  = perimeter * area;
        int sides       = countSidesOfRegion(matrix, region);
        int fenceCost2  = sides * area;
        sumPrice1 += fenceCost1;
        sumPrice2 += fenceCost2;
        /* std::cout << "Part 1            | " << regionChar << ": " << fenceCost1 << " = " << area << " * " << perimeter << std::endl;
        std::cout << "Part 2 (discount) | " << regionChar << ": " << fenceCost2 << " = " << area << " * " << sides << std::endl;
        std::cout << "Corners: " << sides << std::endl; */
    }

    std::cout << "Fence price part 1: " << sumPrice1 << std::endl;
    std::cout << "Fence price part 2: " << sumPrice2 << std::endl;

    return 0;
}