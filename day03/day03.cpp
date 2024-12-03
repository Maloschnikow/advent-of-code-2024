#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

int executeMultiplyInstruction(std::string instructionStr) {
    //verify that the instruction is valid
    if( instructionStr.substr(0,4) == "mul(" ) {
        try {
            int commaPos = instructionStr.find(",");
            int closeBrace = instructionStr.find(")");

            std::string num1Str = instructionStr.substr(4, commaPos - 4);
            std::string num2Str = instructionStr.substr(commaPos + 1, closeBrace - (commaPos + 1) );

            //verify that the number strings don't contain invalid characters
            if ( !(std::all_of(num1Str.begin(), num1Str.end(), ::isdigit) && std::all_of(num2Str.begin(), num2Str.end(), ::isdigit)) ) {
                return -1;
            }

            //multiply
            int num1 = std::stoi(num1Str);
            int num2 = std::stoi(num2Str);

            return num1 * num2;
        }
        catch (std::out_of_range e) {
            std::cout << instructionStr << " is not a valid mul instruction" << std::endl;
            return -1;
        }
        catch (std::exception e) {
            std::cout << instructionStr << " is not a valid mul instruction" << std::endl;
            return -1;
        }
    }

    return -1;
} 

int main() {

    int mulSum = 0;
    bool mulEnabled = true;

    std::vector<std::string> seq;
    std::string line;

    //read line by line
    while(std::getline(std::cin, line)) {
        std::istringstream iss(line);

        //seq contains instruction streams (strings), because >> will stop reading at a space
        std::string instructionStream;
        while(iss >> instructionStream) {
            seq.push_back(instructionStream);
        }

        for(std::string insStream : seq) {

            size_t lastDoInstructionBegin = 0;

            size_t lastDontInstructionBegin = 0;

            size_t lastMulInstructionBegin = 0;
            size_t lastMulInstructionEnd = 0;
            while( lastMulInstructionBegin != insStream.npos) {

                lastMulInstructionBegin = insStream.find("mul(", lastMulInstructionEnd);
                lastMulInstructionEnd = insStream.find(")", lastMulInstructionBegin);


                //try to find do/dont instructions before the read mul instruction, to determine if it should be executed (only the most recent do/dont counts)
                lastDoInstructionBegin = insStream.rfind("do()", lastMulInstructionBegin);
                if( lastDoInstructionBegin == insStream.npos ) {
                    lastDoInstructionBegin = -1;
                }
                
                lastDontInstructionBegin = insStream.rfind("don't()", lastMulInstructionBegin);
                if( lastDontInstructionBegin == insStream.npos ) {
                    lastDontInstructionBegin = -1;
                }

                if( ((lastDoInstructionBegin == insStream.npos) || (lastDoInstructionBegin < lastDontInstructionBegin)) && (lastDontInstructionBegin < lastMulInstructionBegin)) {
                    mulEnabled = false;
                }
                else if ( ((lastDontInstructionBegin == insStream.npos) || (lastDontInstructionBegin < lastDoInstructionBegin)) && (lastDoInstructionBegin < lastMulInstructionBegin) ) {
                    mulEnabled = true;
                }
                /**
                 * break after reading do/dont, because if a do/dont instructions is at end of line, it will be read,
                 * because lastMulInstructionBegin is npos (large number) and rfind tries to find the substring before the given pos
                 */
                if(lastMulInstructionBegin == insStream.npos) { break; }
                if(lastMulInstructionEnd == insStream.npos) { break; }

                std::string instruction = insStream.substr(lastMulInstructionBegin, lastMulInstructionEnd + 1 - lastMulInstructionBegin);

                int mul = executeMultiplyInstruction(instruction);

                //if the instruction was not a valid one we need to search again from lastMulInstructionBegin+1
                if( mul == -1 ) { 
                    lastMulInstructionEnd = lastMulInstructionBegin + 1;
                    
                    continue;
                } else {
                    if(!mulEnabled) {
                        std::cout << "\e[31m" << instruction << "\e[0m";
                    }
                    else {
                        std::cout << "\e[32m" << instruction << "\e[0m";
                    }
                }

                if(mulEnabled) {
                    mulSum = mulSum + mul;
                }
            }
        }
        std::cout << std::endl;
        seq.clear();
    }
    
    std::cout << mulSum << std::endl;
    return 0;
}