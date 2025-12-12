#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#else
#endif

using namespace std;

FileManager::FileManager() : currentFile("suppliers.dat") {}

FileManager::FileManager(const std::string& filename) : currentFile(filename) {}

bool FileManager::saveToFile(const vector<Supplier>& suppliers, FileFormat format) {
    ofstream out;

    if (format == BINARY) {
        out.open(currentFile, ios::binary);
    }
    else {
        out.open(currentFile);
    }

    if (!out) {
        cerr << "Error: Cannot open file for writing: " << currentFile << endl;
        return false;
    }

    try {
        size_t count = suppliers.size();

        if (format == BINARY) {
            out.write(reinterpret_cast<const char*>(&count), sizeof(count));
        }
        else {
            out << count << "\n";
        }

        for (const auto& supplier : suppliers) {
            if (format == BINARY) {
                if (!supplier.saveToBinary(out)) {
                    cerr << "Error writing supplier to binary file." << endl;
                    return false;
                }
            }
            else {
                if (!supplier.saveToText(out)) {
                    cerr << "Error writing supplier to text file." << endl;
                    return false;
                }
            }
        }

        out.close();
        return true;

    }
    catch (const exception& e) {
        cerr << "Error saving file: " << e.what() << endl;
        if (out.is_open()) {
            out.close();
        }
        return false;
    }
}

bool FileManager::loadFromFile(vector<Supplier>& suppliers, FileFormat format) {
    if (!fileExists(currentFile)) {
        cerr << "Error: File does not exist: " << currentFile << endl;
        return false;
    }

    ifstream in;

    if (format == BINARY) {
        in.open(currentFile, ios::binary);
    }
    else {
        in.open(currentFile);
    }

    if (!in) {
        cerr << "Error: Cannot open file for reading: " << currentFile << endl;
        return false;
    }

    try {
        size_t count;

        if (format == BINARY) {
            if (!in.read(reinterpret_cast<char*>(&count), sizeof(count))) {
                cerr << "Error reading supplier count from binary file." << endl;
                return false;
            }
        }
        else {
            in >> count;
            in.ignore();
        }

        suppliers.clear();

        for (size_t i = 0; i < count; i++) {
            Supplier supplier;

            if (format == BINARY) {
                if (!supplier.loadFromBinary(in)) {
                    cerr << "Error reading supplier from binary file." << endl;
                    suppliers.clear();
                    return false;
                }
            }
            else {
                if (!supplier.loadFromText(in)) {
                    cerr << "Error reading supplier from text file." << endl;
                    suppliers.clear();
                    return false;
                }
            }

            suppliers.push_back(supplier);
        }

        in.close();
        return true;

    }
    catch (const exception& e) {
        cerr << "Error loading file: " << e.what() << endl;
        suppliers.clear();
        if (in.is_open()) {
            in.close();
        }
        return false;
    }
}

bool FileManager::fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}


string FileManager::getFileName(const string& path) {
    size_t lastSlash = path.find_last_of("/\\");
    if (lastSlash != string::npos) {
        return path.substr(lastSlash + 1);
    }
    return path;
}

string FileManager::getFileExtension(const string& filename) {
    size_t lastDot = filename.find_last_of(".");
    if (lastDot != string::npos) {
        return filename.substr(lastDot);
    }
    return "";
}

size_t FileManager::getFileSize(const string& filename) {
    ifstream file(filename, ios::binary | ios::ate);
    if (!file) {
        return 0;
    }
    return file.tellg();
}

void FileManager::setFileName(const string& filename) {
    currentFile = filename;
}

string FileManager::getFileName() const {
    return currentFile;
}