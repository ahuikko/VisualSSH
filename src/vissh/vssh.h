#ifndef VSSH_H // include guard
#define VSSH_H

#include <string>

class FileSystemItem {
public:
    std::string name;
    std::string fileType;  // Detailed file type information

    FileSystemItem(const std::string& name, const std::string& fileType)
        : name(name), fileType(fileType) {}
};

#endif