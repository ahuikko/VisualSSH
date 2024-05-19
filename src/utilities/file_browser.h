#pragma once

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

class FileBrowser {
public:
    FileBrowser(const std::string& startPath = fs::current_path().string());

    // Lists the current directory's contents
    std::vector<std::string> listContents();

    // Navigate to a subdirectory
    bool navigateDown(const std::string& directoryName);
    bool navigate(const std::string& directoryName);
    // Navigate to the parent directory
    bool navigateUp();

    bool isDir(const std::string& itemName);
    bool isFile(const std::string& itemName);
    std::string getCurrentPath();

private:
    fs::path currentPath;
    std::vector<std::string> directories_;
    std::vector<std::string> files_;
    std::vector<std::string> items_;

    // Refreshes the lists of files and directories in the current directory
    void refreshContents();
};
