#include <iostream>
#include <vector>

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

struct PageOrder {
    int before;
    int after;

    bool check(int bef, int aft) {
        if ( (bef == after) && (aft == before) ) {
            return false;
        }
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const PageOrder& pageorder) {
        return os << pageorder.before << "|" << pageorder.after;
    };
};

PageOrder getPageOrderOfString(std::string str) {
    int seperatorPos = str.find("|");
    std::string num1Str = str.substr(0, seperatorPos+1);
    std::string num2Str = str.substr(seperatorPos+1, str.size() - (seperatorPos+1));

    int num1 = std::stoi(num1Str);
    int num2 = std::stoi(num2Str);

    return {num1, num2};
}

std::vector<int> getUpdatesOfString(std::string str) {
    std::vector<int> pages;

    size_t lastPageStart = 0;
    size_t lastPageEnd = str.find(",", lastPageStart);
    std::string pageStr;

    while( lastPageEnd != str.npos) {
        pageStr = str.substr(lastPageStart, lastPageEnd - lastPageStart);
        int page = std::stoi(pageStr);
        pages.push_back(page);

        lastPageStart = lastPageEnd+1;
        lastPageEnd = str.find(",", lastPageEnd+1);
    }

    pageStr = str.substr(lastPageStart, str.size() - lastPageStart);
    int lastPage = std::stoi(pageStr);
    pages.push_back(lastPage);

    return pages;
}

bool checkPairOfPagesAgainstAllRules(int beforePage, int afterPage, std::vector<PageOrder> orderingRules) {
    for(PageOrder rule : orderingRules) {
        if( !rule.check(beforePage, afterPage) ) {
            return false;
        }
    }
    return true;
}

bool checkUpdate(std::vector<int> update, std::vector<PageOrder> orderingRules) {

    if(orderingRules.empty()) { return true; }
    if(update.size() <= 1)    { return true; }

    if(update.size() == 2) {
        return checkPairOfPagesAgainstAllRules(update.at(0), update.at(1), orderingRules);
    }

    std::vector<int> updateHalf1(update.begin()                        , update.begin() + (update.size()/2) );
    std::vector<int> updateHalf2(update.begin() + (update.size()/2), update.end()                           );

    if( !(checkUpdate(updateHalf1, orderingRules) && checkUpdate(updateHalf2, orderingRules)) ) {
        return false;
    }

    for(int beforePage : updateHalf1) {
        for(int afterPage : updateHalf2) {
            if ( !checkPairOfPagesAgainstAllRules(beforePage, afterPage, orderingRules) ) {
                return false;
            }
        }
    }

    return true;
}

std::vector<int> sortUpdate(std::vector<int> update, std::vector<PageOrder> orderingRules) {

    if(orderingRules.empty()) { return update; }
    if(update.size() <= 1)    { return update; }

    if(update.size() == 2) {
        if(checkPairOfPagesAgainstAllRules(update.at(0), update.at(1), orderingRules)) {
            return update;
        }
        int temp = update.at(0);
        update.at(0) = update.at(1);
        update.at(1) = temp;
        return update;
    }

    std::vector<int> updateHalf1(update.begin()                        , update.begin() + (update.size()/2) );
    std::vector<int> updateHalf2(update.begin() + (update.size()/2), update.end()                           );

    updateHalf1 = sortUpdate(updateHalf1, orderingRules);
    updateHalf2 = sortUpdate(updateHalf2, orderingRules);

    std::vector<int> mergedUpdate;
    int i = 0;
    int j = 0;

    while(i < updateHalf1.size() && j < updateHalf2.size()) {
        if ( !checkPairOfPagesAgainstAllRules(updateHalf1.at(i), updateHalf2.at(j), orderingRules) ) {
            mergedUpdate.push_back( updateHalf2.at(j) );
            ++j;
        } else {
            mergedUpdate.push_back( updateHalf1.at(i) );
            ++i;
        }
    }

    while(i < updateHalf1.size()) {
        mergedUpdate.push_back( updateHalf1.at(i) );
        ++i;
    }

    while(j < updateHalf2.size()) {
        mergedUpdate.push_back( updateHalf2.at(j) );
        ++j;
    }

    return mergedUpdate;
}

std::vector<int> getCorrectUpdates(std::vector<std::vector<int>> updateList, std::vector<PageOrder> orderingRules, bool invert=false) { //invert return incorrent updates
    std::vector<int> indexesOfCorrectUpdates;

    for(int i = 0; i < updateList.size(); ++i) {
        if ( checkUpdate(updateList.at(i), orderingRules) != invert ) {
            indexesOfCorrectUpdates.push_back(i);
        }
    }
    return indexesOfCorrectUpdates;
}

int sumMiddlePageNumbersOfCorrectUpdates(std::vector<std::vector<int>> updateList, std::vector<PageOrder> orderingRules) {
    int sum = 0;
    std::vector<int> correctUpdates = getCorrectUpdates(updateList, orderingRules);
    for(int updateIndex : correctUpdates) {
        std::vector<int> update = updateList.at(updateIndex);
        sum += update.at( update.size()/2 );
    }
    return sum;
}

int sumMiddlePageNumbersOfOrderedIncorrectUpdates(std::vector<std::vector<int>> updateList, std::vector<PageOrder> orderingRules) {
    int sum = 0;
    std::vector<int> incorrectUpdates = getCorrectUpdates(updateList, orderingRules, true);

    for(int updateIndex : incorrectUpdates) {
        std::vector<int> update = sortUpdate( updateList.at(updateIndex), orderingRules );
        sum += update.at( update.size()/2 );
    }
    return sum;
}

int main() {

    //read ordering rules until empty line
    std::vector<PageOrder> orderingRules;
    std::string inputLine;

    while(std::getline(std::cin, inputLine)) {
        if(inputLine.empty()) { break; }
        orderingRules.push_back( getPageOrderOfString(inputLine) );
    }
    printVec(orderingRules);

    //start reading updates
    std::vector<std::vector<int>> updateList;

    while(std::getline(std::cin, inputLine)) {
        updateList.push_back( getUpdatesOfString(inputLine) );
    }
    printMatrix(updateList);

    std::cout << "Correct sum          : " << sumMiddlePageNumbersOfCorrectUpdates(updateList, orderingRules)          << std::endl;
    std::cout << "Incorrect ordered sum: " << sumMiddlePageNumbersOfOrderedIncorrectUpdates(updateList, orderingRules) << std::endl; 


    return 0;
}