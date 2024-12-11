#include <iostream>
#include <vector>
#include <map>

//for debugging
template <typename T>
void printVec(std::vector<T> vec) {
    for (T element : vec) {
        std::cout << element << ' ';
    }
    std::cout << std::endl;
}

size_t countDigits(unsigned long num) {
    return std::to_string(num).length();
}

void splitNum(unsigned long num, unsigned long *firstHalf, unsigned long *lastHalf) {
    std::string numStr = std::to_string(num);
    std::string firstHalfStr = numStr.substr(0, numStr.size()/2);
    std::string lastHalfStr = numStr.substr(numStr.size()/2);

    *firstHalf = std::stoul(firstHalfStr);
    *lastHalf = std::stoul(lastHalfStr);
}

unsigned long calculatedStoneCountAfterNBlinks(unsigned long stone, size_t nBlinks, std::vector<std::map<unsigned long, unsigned long>>* depthMaps, unsigned long *result = nullptr) {
    unsigned long _result;
    std::map<unsigned long, unsigned long>* currentMap = &depthMaps->at(nBlinks);

    if(nBlinks == 0) {
        _result = 1;
    }
    else {

        try{
            _result = currentMap->at(stone); //look up if value is already in map
        }
        catch(std::out_of_range e) {

            if (stone == 0) {
                _result = calculatedStoneCountAfterNBlinks(1, nBlinks-1, depthMaps);
            }
            else if (countDigits(stone) % 2 == 0) {
                unsigned long newFirstStone;
                unsigned long newLastStone;
                splitNum(stone, &newFirstStone, &newLastStone);

                _result = calculatedStoneCountAfterNBlinks(newFirstStone, nBlinks-1, depthMaps) + calculatedStoneCountAfterNBlinks(newLastStone, nBlinks-1, depthMaps);
            }
            else {
                _result = calculatedStoneCountAfterNBlinks(stone * 2024, nBlinks-1, depthMaps);
            }
            currentMap->insert({stone, _result});
        }
    }

    if(result != nullptr) *result = _result;
    return _result;
}

unsigned long sumStoneCountOfStones(std::vector<unsigned long> lineOfStones, size_t nBlinks) {
    std::vector<std::map<unsigned long, unsigned long>> depthMaps;
    for(size_t i = 0; i <= nBlinks; ++i) {
        depthMaps.push_back( std::map<unsigned long, unsigned long>() );
    }
    std::cout << depthMaps.size() << std::endl;
    unsigned long sum = 0;
    for(unsigned long stone : lineOfStones) {
        sum += calculatedStoneCountAfterNBlinks(stone, nBlinks, &depthMaps);
    }

    return sum;
}

int main() {

    unsigned long num;
    std::vector<unsigned long> lineOfStones;

    while(std::cin >> num) {
        lineOfStones.push_back(num);
    }
    printVec(lineOfStones);

    std::cout << "Stones n=6 : " << sumStoneCountOfStones(lineOfStones, 6) << std::endl;
    std::cout << "Stones n=25: " << sumStoneCountOfStones(lineOfStones, 25) << std::endl;
    std::cout << "Stones n=42: " << sumStoneCountOfStones(lineOfStones, 42) << std::endl;
    std::cout << "Stones n=75: " << sumStoneCountOfStones(lineOfStones, 75) << std::endl;

    return 0;
}