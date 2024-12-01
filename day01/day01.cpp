#include <iostream>
#include <algorithm>
#include <vector>
#include <limits.h>

//part 1
int listDist(std::vector<int> a, std::vector<int> b) {

    //a and b must have the same amount of elements and shall not be empty
    if (a.empty() || b.empty() || a.size() != b.size()) {
        return -1;
    }

    struct ValIndexPair {
        int val;
        int index;
        bool operator< (ValIndexPair &other) {
            return val < other.val;
        }
    };

    std::vector<ValIndexPair> aSorted;
    std::vector<ValIndexPair> bSorted;

    /**
     * Copy the values with indexes into new vectors.
     * Then sort the new vectors according to the values.
     * Now we can grab the nth minimal element from each list with their old indexes.
     * The distance of the nth minimal element of each list can now be obtained by subtracting their old indexes.
     */ 
    for(int i = 0; i < a.size(); ++i) {
        aSorted.push_back( {a.at(i), i } );
        bSorted.push_back( {b.at(i), i } );
    }

    std::sort(aSorted.begin(), aSorted.end());
    std::sort(bSorted.begin(), bSorted.end());

    int diff = 0;
    for( int n = 0; n < a.size(); ++n) {
        diff = diff + std::abs( a.at( aSorted.at(n).index) - b.at( bSorted.at(n).index));
    }

    return diff;
}

//part 2
int similarityScore(std::vector<int> a, std::vector<int> b) {

    //a and b must have the same amount of elements and shall not be empty
    if (a.empty() || b.empty() || a.size() != b.size()) {
        return -1;
    }

    int score = 0;
    for( int i = 0; i < a.size(); ++i) {
        int aVal = a.at(i);
        int count = std::count(b.begin(), b.end(), aVal);
        score = score + aVal * count;
    }

    return score;
}

int main () {
    std::vector<int> a, b;
    int num1, num2;
    while(std::cin >> num1 >> num2) {
        a.push_back(num1);
        b.push_back(num2);
    }

    std::cout << "Distance:         " << listDist(a, b) << std::endl;
    std::cout << "Similarity score: " << similarityScore(a, b) << std::endl;

    return 0;
}