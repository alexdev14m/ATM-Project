#include <iostream>
#include <string>
#include "Transactions.hpp"
#include "picosha2.hpp"

int main(){
    std::cout << "Hello world!" << std::endl;
    std::cout << std::endl;
    const std::string filename = "data";
    bool whileState = false;

    static int numofacc = 0;

    FileManipulation::makeFile(filename);

    int choice;
    while(!whileState){
        std::cout << "Select the operation you want to do:\n1. Create an account\n2. Deposit\n3. Withdraw\n4. Check your balance\n5. Exit" << std::endl << "> ";
        if (!(std::cin >> choice)){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Error: Input isn't an integer!" << std::endl;
            std::cout << std::endl;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch(choice){
            case 1: {
                if (FileManipulation::checkLineHasText(filename, 2)){
                    FileManipulation::rewriteWordInLine(2, 1, std::to_string(numofacc), filename);
                } else {
                    FileManipulation::appendText(std::to_string(numofacc), filename);
                }

                Account acc;
                std::string name;
                int pin;
                int ramBalance = 0;

                std::cout << "Enter your name: " << std::endl << "> ";
                std::getline(std::cin, name);

                std::cout << "Enter your desired pin (1111-9999)" << std::endl << "> ";
                std::cin >> pin;

                std::string hashedpin = picosha2::hash256_hex_string(std::to_string(pin));

                if (pin < 1111 || pin > 9999) {
                    std::cerr << "The pin is out range!." << std::endl << std::endl;
                    break;
                }

                acc.setBalance(ramBalance);

                std::string finalString = name + " " + hashedpin + " " + std::to_string(acc.getBalance());

                FileManipulation::appendText(finalString, filename);

                std::cout << std::endl;

                numofacc++;

                break;
            }
            case 2: {
                Transactions::deposit(filename);
                break;
            }

            case 3: {
                Transactions::withdraw(filename);
                break;
            }

            case 4: {
                std::cout << Transactions::checkBalance(filename) << std::endl;
                break;
            }

            case 5: {
                whileState = true;
                break;
            }
            default: {
                std::cerr << "Error: Inputed choice is invalid. Please select a valid choice (1-5)!" << std::endl;
                std::cout << std::endl;
                break;
            }
        }
    }

    return 0;
}
