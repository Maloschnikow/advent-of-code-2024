#include <iostream> 
#include <vector>
#include <chrono>
#include <sstream>

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

long concatenate(long i, long j) {
    std::string iStr = std::to_string(i);
    std::string jStr = std::to_string(j);
    return std::stol( iStr.append(jStr) );
}

bool testEquation(std::vector<long> equation, bool _enableCon=false) {
    if(equation.size() == 1) {
        return true;
    }

    long testValue = equation.front();

    if(equation.size() == 2) {
        return testValue == equation.at(1);
    }

    long num1 = equation.at(1);
    long num2 = equation.at(2);

    long mul = num1 * num2;
    long add  = num1 + num2;
    long con = concatenate(num1, num2);

    bool enableMul = mul <= testValue;
    bool enableAdd = add <= testValue;
    bool enableCon = (con <= testValue) && _enableCon;

    std::vector<long> mulEquation;
    std::vector<long> addEquation;
    std::vector<long> conEquation;

    if( enableMul ) {
        mulEquation.push_back(testValue);
        mulEquation.push_back(mul);
    }
    if( enableAdd ) {
        addEquation.push_back(testValue);
        addEquation.push_back(add);
    }
    if( enableCon ) {
        conEquation.push_back(testValue);
        conEquation.push_back(con);
    }

    for(int i = 3; i < equation.size(); ++i) {
        if (enableMul) mulEquation.push_back( equation.at(i) );
        if (enableAdd) addEquation.push_back( equation.at(i) );
        if (enableCon) conEquation.push_back( equation.at(i) );
    }

    return ( (enableMul && testEquation(mulEquation, _enableCon)) || (enableAdd && testEquation(addEquation, _enableCon)) || (enableCon && testEquation(conEquation, _enableCon)) );
}

int main() {

    auto start = std::chrono::system_clock::now();

    std::vector<std::vector<long>> matrix;
    std::string inputLine;

    long checkSum;

    //read line by line
    while(std::getline(std::cin, inputLine)) {
        std::vector<long> equation;

        //first element in equation vector is the checksum
        size_t checkSumEnd = inputLine.find(':');
        std::string checkSumStr;
        checkSum = std::stol( inputLine.substr(0, checkSumEnd) );
        equation.push_back(checkSum);

        //append the remaining numbers of the line to the equation vector
        inputLine = inputLine.substr(checkSumEnd+2);
        std::stringstream ss(inputLine);
        long num;
        while(ss >> num) {
            equation.push_back(num);
        }
        matrix.push_back(equation);
    }

    printMatrix(matrix);

    long totalCalibrationResult = 0;
    for(std::vector<long> equation : matrix) {
        if (testEquation(equation)) {
            totalCalibrationResult += equation.front();
        }
    }

    std::cout << "Total calibration result (part 1): " << totalCalibrationResult << std::endl;
    
    totalCalibrationResult = 0;
    for(std::vector<long> equation : matrix) {
        if (testEquation(equation, true)) {
            totalCalibrationResult += equation.front();
        }
    }

    std::cout << "Total calibration result (part 2): " << totalCalibrationResult << std::endl;
    
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cout << "Elapsed time: \e[33m" << elapsed_seconds.count() << "s \e[0m" << std::endl;
    
    return 0;
}