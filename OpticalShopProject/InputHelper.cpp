#include "InputHelper.h"
#include <iostream>
#include <limits>

void InputHelper::clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int InputHelper::getValidatedInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInputBuffer();
            return value;
        }
        std::cout << "Invalid input! Please enter an integer.\n";
        clearInputBuffer();
    }
}

double InputHelper::getValidatedDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInputBuffer();
            return value;
        }
        std::cout << "Invalid input! Please enter a number.\n";
        clearInputBuffer();
    }
}

std::string InputHelper::getStringInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}