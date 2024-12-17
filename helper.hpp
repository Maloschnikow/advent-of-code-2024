#ifndef HELPER_H
#define HELPER_H

#include <set>
#include <vector>
#include <iostream>

namespace helper {

    struct Coord {
        long x;
        long y;

        Coord() { x, y = -1, -1; }
        Coord(long x, long y) : x(x), y(y) {}

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

        bool inBoundaries(long xHighBoundary, long yHighBoundary, long xLowBoundary=0, long yLowBoundary=0) {
            return !(x < xLowBoundary || x > xHighBoundary || y < yLowBoundary || y > yHighBoundary);
        }

        friend std::ostream& operator<<(std::ostream &os, const Coord &coord) {
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

        Coord operator*(const long &s) const {
            return Coord(x*s, y*s);
        }

        friend Coord operator*(const long &s, const Coord &coord) {
            return Coord(coord.x*s, coord.y*s);
        }

        bool operator==(const Coord &other) const {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Coord &other) const {
            return !(*this == other);
        }
    };

    //for debugging
    template <typename T>
    void printSet(std::set<T> s) {
        for (T element : s) {
            std::cout << element << ' ';
        }
        std::cout << std::endl;
    }

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

    //for debugging
    template <typename T>
    std::vector<std::vector<T>> createSubMatrix(std::vector<std::vector<T>> &mat, size_t xMin, size_t yMin, size_t xMax, size_t yMax) {
        std::vector<std::vector<T>> subMat;
        for (int x = xMin; x <= xMax; ++x) {
            std::vector<T> newColumn;
            for(int y = yMin; y <= yMax; ++y) {
                newColumn.push_back(mat.at(x).at(y));
            }
            subMat.push_back(newColumn);
        }
        return subMat;
    }

}

#endif
