#include <iostream>
#include <string>
#include <ostream>
#include <string>
#include <limits>
#include "FileManipulation.hpp"
#include "Account.hpp"

class Transactions {
    public:
        static bool deposit(std::string filename) {
            Account acc;

            std::string name;
            std::cout << "Enter your name: " << std::endl;
            std::getline(std::cin, name);

            std::string pin;
            std::cout << "Enter your pin: " << std::endl;
            std::cin >> pin;

            std::string tempBalance;
            if (!FileManipulation::matchTextWithSpecific(filename, name, pin, tempBalance)) {
                std::cerr << "Error: The data you provided either doesn't exist or it's invalid!" << std::endl;
                return false;
            }

            int currentBalance = std::stoi(tempBalance);
            acc.setBalance(currentBalance);

            int amount;
            std::cout << "Enter the amount you want to deposit: " << std::endl;
            std::cin >> amount;

            int newBalance = currentBalance + amount;
            acc.setBalance(newBalance);

            int lineNumber = 3;

            FileManipulation::rewriteWordInLine(lineNumber, 4, std::to_string(acc.getBalance()), filename);

            return true;
        }

        static bool withdraw(const std::string filename) {
            Account acc;

            std::string name;
            std::cout << "Enter your name: " << std::endl;
            std::getline(std::cin, name);

            std::string pin;
            std::cout << "Enter your pin: " << std::endl;
            std::cin >> pin;

            std::string tempBalance;
            if (!FileManipulation::matchTextWithSpecific(filename, name, pin, tempBalance)) {
                std::cerr << "Error: The data you provided either doesn't exist or it's invalid!" << std::endl;
                return false;
            }

            int currentBalance = std::stoi(tempBalance);
            acc.setBalance(currentBalance);

            int amount;
            std::cout << "Enter the amount you want to withdraw: " << std::endl;
            std::cin >> amount;

            if((currentBalance - amount) < 0 || currentBalance == 0){
                std::cerr << "Error: Cannot withdraw amount, your balance is 0 or the amount gets in the negative!" << std::endl;
                return false;
            }

            int newBalance = currentBalance - amount;
            acc.setBalance(newBalance);

            FileManipulation::rewriteWordInLine(3, 4, std::to_string(acc.getBalance()), filename);

            return true;
        }

        static int checkBalance(const std::string& filename) {
            Account acc;

            int currentBalance = FileManipulation::findIntByLine(filename, 3, 2);
            acc.setBalance(currentBalance);

            return acc.getBalance();
        }
};
