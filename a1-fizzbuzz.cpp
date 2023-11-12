#include <vector>
#include <string>
#include <thread>
#include <iostream>

constexpr static uint8_t TOTAL_ROUNDS = 10; 

enum class result {
    divByThree,
    divByFive, 
    divByThreeAndFive, 
    divByNone
};

result checkDivisibility(uint8_t number) {
    if (number % 3 == 0 && number % 5 == 0) {
        return result::divByThreeAndFive;
    } else if (number % 3 == 0) {
        return result::divByThree;
    } else if (number % 5 == 0) {
        return result::divByFive;
    } else {
        return result::divByNone;
    }   
}

int main() {
    std::vector<std::string> players{"Abdul", "Bart", "Claudia", "Divya"};

    std::thread th([&players]() {
        int currRound = 1;
        while(currRound <= TOTAL_ROUNDS) {

            for (auto& name : players) {
                if (currRound == (TOTAL_ROUNDS + 1)) break;

                auto output = checkDivisibility(currRound);

                if (output == result::divByNone) {
                    std::cout << name << " says " << currRound << std::endl;
                }
                else if (output == result::divByThree) {
                    std::cout << name << " says " << "fizz!" << std::endl;
                }
                else if (output == result::divByFive) {
                    std::cout << name << " says " << "buzz!" << std::endl;
                }
                else if (output == result::divByThreeAndFive) {
                    std::cout << name << " says " << "fizzbuzz!" << std::endl;
                }
                currRound++;
            }
        }
    });

    th.join();

    return 0;
}

