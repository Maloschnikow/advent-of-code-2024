#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

//for debugging
void printBlock(std::vector<unsigned long> vec) {
    for (unsigned long element : vec) {
        if(element == -1) {
            std::cout << ".|";
        }
        else {
            std::cout << element << '|';
        }
    }
    std::cout << std::endl;
}

std::vector<unsigned long> pushBackUnsignedLongNTimes(std::vector<unsigned long> vec, unsigned long x, unsigned long n) {
    for(unsigned long i = 0; i < n; ++i) {
        vec.push_back(x);
    }
    return vec;
}

std::vector<unsigned long> getFileSizes(std::string input) {
    std::vector<unsigned long> fileSizes;
    for(size_t i = 0; i < input.size(); ++i) {
        if(i % 2 == 0) {
            fileSizes.push_back(input.at(i) - '0');
        }
    }
    return fileSizes;
}

std::vector<unsigned long> getFreeSpaceSizes(std::string input) {
    std::vector<unsigned long> freeSpaceSizes;
    for(size_t i = 0; i < input.size(); ++i) {
        if(i % 2 != 0) {
            freeSpaceSizes.push_back(input.at(i) - '0');
        }
    }
    return freeSpaceSizes;
}

std::vector<unsigned long> getBlockOfFilesAndFreeSpaces(std::vector<unsigned long> fileSizes, std::vector<unsigned long> freeSpaces) {
    std::vector<unsigned long> block;

    size_t i, j = 0; //i is also file id


    while(i < fileSizes.size() && j < freeSpaces.size()) {
        block = pushBackUnsignedLongNTimes(block, i, fileSizes.at(i));
        block = pushBackUnsignedLongNTimes(block, -1, freeSpaces.at(j)); //id of -1 is free space (-1 in unsigned is biggest possible number)
        ++i;
        ++j;
    }
    // technically only one can be left if the other is empty, but while loops look pretty
    while( i < fileSizes.size()) {
        block = pushBackUnsignedLongNTimes(block, i, fileSizes.at(i));
        ++i;
    }
    while( j < freeSpaces.size()) {
        block = pushBackUnsignedLongNTimes(block, -1, freeSpaces.at(j));
        ++j;
    }

    return block;
}

std::vector<unsigned long> compactBlock(std::vector<unsigned long> block, bool output=false) {

    std::vector<unsigned long>::iterator freeSpaceItr = std::find(block.begin(), block.end(), -1);
    std::vector<unsigned long>::reverse_iterator lastCharItr = std::find_if(block.rbegin(), block.rend(), [&](unsigned long val) { return val != -1; });

    while( std::distance(lastCharItr, block.rend()) > std::distance(block.begin(), freeSpaceItr)) {

        *freeSpaceItr = *lastCharItr;
        *lastCharItr = -1;
        freeSpaceItr = std::find(freeSpaceItr, block.end(), -1);
        lastCharItr = std::find_if(lastCharItr, block.rend(), [&](unsigned long val) { return val != -1; });
        if (output) printBlock(block);
    }

    return block;
}

std::vector<unsigned long> compactBlockWholeFiles(std::vector<unsigned long> block, bool output=false) {

    std::vector<unsigned long>::iterator freeSpaceBeginItr = std::find(block.begin(), block.end(), -1);
    std::vector<unsigned long>::iterator freeSpaceEndItr = std::find_if(freeSpaceBeginItr, block.end(), [&](unsigned long val) { return val != -1; }) - 1;
    size_t freeSpaceSize = std::distance(freeSpaceBeginItr, freeSpaceEndItr) + 1;

    std::vector<unsigned long>::reverse_iterator lastCharEndItr = std::find_if(block.rbegin(), block.rend(), [&](unsigned long val) { return val != -1; });
    unsigned long fileID = *lastCharEndItr;

    std::vector<unsigned long>::reverse_iterator lastCharBeginItr = std::find_if(lastCharEndItr, block.rend(), [&](unsigned long val) { return val != fileID; }) - 1;

    size_t fileSize = std::distance(lastCharEndItr, lastCharBeginItr) + 1;

    while(lastCharEndItr != block.rend()) { //go through each file from behin
        while( std::distance(lastCharBeginItr, block.rend()) > std::distance(block.begin(), freeSpaceEndItr)) { //go through each free space from front

            if(freeSpaceSize >= fileSize) {
                for(size_t i = 0; i < fileSize; ++i) {
                    *(freeSpaceBeginItr + i) = fileID;
                    *(lastCharEndItr + i) = -1;
                }
                //std::cout << fileID << " file moved into free space at " << std::distance(block.begin(), freeSpaceBeginItr) << std::endl;
                if (output) printBlock(block);
                break;

            }
            //std::cout << fileID << " file NOT moved into free space at " << std::distance(block.begin(), freeSpaceBeginItr) << std::endl;
            freeSpaceBeginItr = freeSpaceEndItr+1;

            freeSpaceBeginItr = std::find(freeSpaceBeginItr, block.end(), -1);
            freeSpaceEndItr = std::find_if(freeSpaceBeginItr, block.end(), [&](unsigned long val) { return val != -1; }) - 1;
            freeSpaceSize = std::distance(freeSpaceBeginItr, freeSpaceEndItr) + 1;
        }

        freeSpaceBeginItr = std::find(block.begin(), block.end(), -1);
        freeSpaceEndItr = std::find_if(freeSpaceBeginItr, block.end(), [&](unsigned long val) { return val != -1; }) - 1;
        freeSpaceSize = std::distance(freeSpaceBeginItr, freeSpaceEndItr) + 1;

        lastCharEndItr = std::find_if(lastCharBeginItr+1, block.rend(), [&](unsigned long val) { return val != -1; });
        fileID = *lastCharEndItr;
        lastCharBeginItr = std::find_if(lastCharEndItr, block.rend(), [&](unsigned long val) { return val != fileID; }) - 1;
        fileSize = std::distance(lastCharEndItr, lastCharBeginItr) + 1;
    }

    return block;
}

unsigned long calculateChecksumOfCompactBlock(std::vector<unsigned long> block) {
    unsigned long checkSum = 0;
    for(size_t i = 0; i < block.size(); ++i) {
        if(block.at(i) == -1) continue;
        checkSum = checkSum + (block.at(i) * i);
    }
    return checkSum;
}

int main() {

    std::string inputLine;
    std::getline(std::cin, inputLine);
    //std::cout << inputLine << std::endl;

    // PART 1 TIME START
    auto part1Start = std::chrono::system_clock::now();

    // PART 1 -------------------------------------------------------------------------------------------------------
    //std::cout << "PART 1" << std::endl;

    std::vector<unsigned long> fileSizes = getFileSizes(inputLine);
    std::vector<unsigned long> freeSpaceSizes = getFreeSpaceSizes(inputLine);

    //std::cout << "Block ------------------" << std::endl;
    std::vector<unsigned long> block = getBlockOfFilesAndFreeSpaces(fileSizes, freeSpaceSizes);
    //printBlock(block);

    //std::cout << "Compact block ----------" << std::endl;
    std::vector<unsigned long> compactedBlock = compactBlock(block, false);
    //printBlock(compactedBlock);
    unsigned long checkSum = calculateChecksumOfCompactBlock(compactedBlock);
    std::cout << "Checksum  : " << checkSum << std::endl;
    //std::cout << std::endl;
    
    auto part2Start = std::chrono::system_clock::now();
    // PART 2 -------------------------------------------------------------------------------------------------------
    //std::cout << "PART 2" << std::endl;
    //std::cout << "Compact block 2 --------" << std::endl;
    std::vector<unsigned long> compactedBlock2 = compactBlockWholeFiles(block, false);
    //printBlock(compactedBlock2);
    unsigned long checkSum2 = calculateChecksumOfCompactBlock(compactedBlock2);
    std::cout << "Checksum 2: " << checkSum2 << std::endl;

    auto part2End = std::chrono::system_clock::now();

    // TIME STOP
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds_part1 = part2Start - part1Start;
    std::chrono::duration<double> elapsed_seconds_part2 = part2End - part2Start;

    std::cout << "Part 1 took: \e[33m" << elapsed_seconds_part1.count() << "s \e[0m" << std::endl;
    std::cout << "Part 2 took: \e[33m" << elapsed_seconds_part2.count() << "s \e[0m" << std::endl;
    
    return 0;
}