#ifndef INPUTHELPER_H
#define INPUTHELPER_H

#include <string>

class InputHelper {
public:
    static void clearInputBuffer();
    static int getValidatedInt(const std::string& prompt);
    static double getValidatedDouble(const std::string& prompt);
    static std::string getStringInput(const std::string& prompt);
};

#endif
