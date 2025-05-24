#include <iostream>

class Account {
    public:
        std::string getName() {
            return name;
        }

        void setName(const std::string& newName) {
            name = newName;
        }

        int getPin() {
            return pin;
        }

        void setPin(int newPin) {
            pin = newPin;
        }

        int getBalance() {
            return balance;
        }

        void setBalance(int newBalance) {
            balance = newBalance;
        }

        int getCreditscore() {
            return creditscore;
        }

        void setCreditscore(int newCreditscore) {
            creditscore = newCreditscore;
        }

    private:
        std::string name;
        int pin;
        int balance;
        int creditscore;
};