#ifndef HELPER_H
#define HELPER_H

#include <set>
#include <vector>
#include <iostream>

namespace helper {

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
