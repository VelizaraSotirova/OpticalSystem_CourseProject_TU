#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include "Supplier.h"

class FileManager {
public:
    enum FileFormat {
        BINARY,
        TEXT
    };

    FileManager();
    FileManager(const std::string& filename);

    bool saveToFile(const std::vector<Supplier>& suppliers, FileFormat format = TEXT);
    bool loadFromFile(std::vector<Supplier>& suppliers, FileFormat format = TEXT);

    static bool fileExists(const std::string& filename);

    static std::string getFileName(const std::string& path);
    static std::string getFileExtension(const std::string& filename);
    static size_t getFileSize(const std::string& filename);

    void setFileName(const std::string& filename);
    std::string getFileName() const;

private:
    std::string currentFile;
};

#endif
