#include <iostream>
#include <vector>
#include <sstream>

//for debugging
void printVec(std::vector<int> vec) {
    for (int element : vec) {
        std::cout << element << ' ';
    }
    std::cout << std::endl;
}

bool checkIncreasingSequence(std::vector<int> seq, bool problemDampner=true) {
    int prev = -1;
    for(int i = 0; i < seq.size(); ++i) {
        int y = seq.at(i);
        if((prev != -1) && !(y > prev && y - prev <= 3) ) {
            if(problemDampner) {

                //the sequence can be valid if either the prev or the current element is removed
                std::vector<int> seq1 = seq;
                std::vector<int> seq2 = seq;
                seq1.erase(seq1.begin()+i);
                seq2.erase(seq2.begin()+i-1);
                return checkIncreasingSequence(seq1, false) || checkIncreasingSequence(seq2, false);
            }
            return false;
        }

        prev = y;
    }
    return true;
}

bool checkDecreasingSequence(std::vector<int> seq, bool problemDampner=true) {
    int prev = -1;
    for(int i = 0; i < seq.size(); ++i) {
        int y = seq.at(i);
        if((prev != -1) && !(y < prev && prev - y <= 3) ) {
            if(problemDampner) {

                //the sequence can be valid if either the prev or the current element is removed
                std::vector<int> seq1 = seq;
                std::vector<int> seq2 = seq;
                seq1.erase(seq1.begin()+i);
                seq2.erase(seq2.begin()+i-1);
                return checkDecreasingSequence(seq1, false) || checkDecreasingSequence(seq2, false);
            }
            return false;
        }

        prev = y;
    }
    return true;
}

bool checkSequence(std::vector<int> seq, bool problemDampner=true) {
    return checkDecreasingSequence(seq, problemDampner) || checkIncreasingSequence(seq, problemDampner);
}

int main() {
    int amountOfSafeReports = 0;

    std::vector<int> seq;
    std::string line;
    while(std::getline(std::cin, line)) {
        std::istringstream iss(line);
        int y;
        while(iss >> y) {
            seq.push_back(y);
        }
        if (checkSequence(seq)) {
            amountOfSafeReports += 1;
        }
        seq.clear();
    }
    
    std::cout << amountOfSafeReports << std::endl;

    return 0;
}
