#include <iostream>
#include <filesystem>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>

class FileManipulation {
    public:
        static bool makeFile(const std::string filename) {
            std::string filenameNew;

            if (filename.ends_with(".log")) {
                std::cout << "The string ends with '.log'\n";
                filenameNew = filename;
            } else {
                filenameNew = filename + ".log";
            }

            if (std::filesystem::exists(filenameNew)) {
                std::cerr << "File exists, not making it!" << std::endl;
                return false;
            }

            std::cout << "Making file " << filenameNew << std::endl;
            std::ofstream logFile(filenameNew, std::ios::app);

            if (logFile){
                logFile << "FULL NAME  ||  PIN  ||  BALANCE\n";
                logFile.close();
            } else {
                std::cerr << "File doesn't exist or it cannot be opened." << std::endl;
                return false;
            }
            return true;
        }

        static bool appendText(const std::string text, const std::string filename){
            std::string filenameNew;

            if (!filename.ends_with(".log")){
                filenameNew = filename + ".log";
            } else {
                filenameNew = filename;
            }

            if (std::filesystem::exists(filenameNew)) {
                std::ofstream logFile(filenameNew, std::ios::app);
                if(!logFile) return false;

                logFile << text << "\n";
                logFile.close();

                return true;
            }
            else return false;
        }

        static bool rewriteWordInLine(int lineNum, int wordIndexToReplace, const std::string& newWord, const std::string& filename) {
            std::string filenameNew = filename.ends_with(".log") ? filename : filename + ".log";

            std::ifstream logFile(filenameNew);
            if (!logFile) {
                std::cerr << "Cannot open the file!" << std::endl;
                return false;
            }

            std::string tempFilename = filenameNew + ".tmp";
            std::ofstream tempFile(tempFilename);
            if (!tempFile) {
                std::cerr << "Cannot create/open the temp file!" << std::endl;
                return false;
            }

            std::string line;
            int currentLine = 1;
            bool replaced = false;

            while (std::getline(logFile, line)) {
                if (currentLine == lineNum) {
                    std::istringstream stream(line);
                    std::ostringstream newLine;
                    std::string word;
                    int wordIndex = 1;

                    while (stream >> word) {
                        if (wordIndex == wordIndexToReplace)
                            newLine << newWord << " ";
                        else
                            newLine << word << " ";
                        wordIndex++;
                    }

                    std::string finalLine = newLine.str();
                    if (!finalLine.empty() && finalLine.back() == ' ')
                        finalLine.pop_back();

                    tempFile << finalLine << "\n";
                    replaced = true;
                } else {
                    tempFile << line << "\n";
                }
                currentLine++;
            }

            logFile.close();
            tempFile.close();

            if (!replaced) {
                std::cerr << "Line number " << lineNum << " not found." << std::endl;
                std::filesystem::remove(tempFilename);
                return false;
            }

            std::filesystem::rename(tempFilename, filenameNew);
            return true;
        }


        static bool lineHasAnyText(const std::string& line) {
            return std::find_if(line.begin(), line.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }) != line.end();
        }

        static bool checkLineHasText(const std::string& filename, int lineNumber) {
            std::string filenameNew;
            if(!filename.ends_with(".log")) filenameNew = filename + ".log";
            else filenameNew = filename;

            std::ifstream file(filenameNew);
            if (!file) {
                std::cerr << "Cannot open file.\n";
                return false;
            }

            std::string line;
            int currentLine = 1;

            while (std::getline(file, line)) {
                if (currentLine == lineNumber) {
                    return lineHasAnyText(line);
                }
                currentLine++;
            }

            std::cerr << "File has fewer than " << lineNumber << " lines.\n";
            return false;
        }

        static std::string findNameByLine(const std::string& filename, int targetLine) {
            std::string filenameNew;
            if(!filename.ends_with(".log")) filenameNew = filename + ".log";
            else filenameNew = filename;

            std::ifstream file(filenameNew);
            if(!file) {
                std::cerr << "Error: Cannot open the file" << std::endl << std::endl;
                return "";
            }

            int currentLine = 1;

            std::string line;
            while (std::getline(file, line)) {
                if(currentLine == targetLine){
                    std::istringstream iss(line);
                    std::string firstname;
                    std::string lastname;
                    std::string fullname;

                    if (iss >> firstname >> lastname) {
                        fullname = firstname + " " + lastname;

                        return fullname;
                    } else {
                        std::cerr << "Not the needed patern!" << std::endl << std::endl;
                        return "";
                    }
                }
                currentLine++;
            }

            return "";
        }

        static int findIntByLine(const std::string& filename, int targetLine, int type) {
            std::string filenameNew;
            if(!filename.ends_with(".log")) filenameNew = filename + ".log";
            else filenameNew = filename;

            std::ifstream file(filenameNew);
            if(!file) {
                std::cerr << "Error: Failed to open the file!" << std::endl;
                return 1;
            }

            int currentLine = 1;

            std::string line;

            while(std::getline(file, line)) {
                if (currentLine == targetLine) {
                    std::istringstream iss(line);
                    std::string skip1;
                    std::string skip2;
                    std::string pin;
                    std::string balance;

                    if (iss >> skip1 >> skip2 >> pin >> balance) {
                        switch(type) {
                            case 1: {
                                return std::stoi(pin);
                                break;
                            }
                            case 2: {
                                return std::stoi(balance);
                                break;
                            }
                            default: {
                                std::cerr << "Invalid pattern!" << std::endl;
                                return 0;
                                break;
                            }
                        }
                    }
                }

                currentLine++;
            }

            return 1;
        }

        static bool matchTextWithSpecific(const std::string& filename, const std::string& fullnameInput, const std::string& pinInput, std::string& balanceOutput) {
            std::string filenameNew = filename;
            if (!filename.ends_with(".log"))
                filenameNew += ".log";

            std::ifstream file(filenameNew);
            if (!file) {
                std::cerr << "Error: Failed to open the file!" << std::endl;
                return false;
            }

            std::string line;
            int lineNumber = 0;

            // Skip first 2 lines (header maybe)
            while (lineNumber < 2 && std::getline(file, line)) {
                ++lineNumber;
            }

            // Now read data lines
            while (std::getline(file, line)) {
                std::istringstream lineStream(line);
                std::string firstName, lastName, pin, balance;

                if (lineStream >> firstName >> lastName >> pin >> balance) {
                    std::string fullname = firstName + " " + lastName;
                    if (fullname == fullnameInput && pin == pinInput) {
                        balanceOutput = balance;
                        file.close();
                        return true;
                    }
                }
            }

            file.close();
            return false;
        }
};
