#include "file_browser.h"

FileBrowser::FileBrowser(const std::string& startPath) : currentPath(startPath) {
    refreshContents();
}

std::vector<std::string> FileBrowser::listContents() {
    refreshContents(); // Ensure the contents are up to date
    return items_;

}

bool FileBrowser::navigate(const std::string& directoryName) {

    if (directoryName == "..")
    {
        if (currentPath.has_parent_path()) {
            currentPath = currentPath.parent_path();
            refreshContents();
            return true;
        }
        return false;
    }
    auto newPath = currentPath / directoryName;
    if (fs::exists(newPath) && fs::is_directory(newPath)) {
        currentPath = newPath;
        refreshContents();
        return true;
    }


    return false;
}

bool FileBrowser::navigateDown(const std::string& directoryName) {
    auto newPath = currentPath / directoryName;
    if (fs::exists(newPath) && fs::is_directory(newPath)) {
        currentPath = newPath;
        refreshContents();
        return true;
    }
    return false;
}

bool FileBrowser::navigateUp() {
    if (currentPath.has_parent_path()) {
        currentPath = currentPath.parent_path();
        refreshContents();
        return true;
    }
    return false;
}

bool FileBrowser::isDir(const std::string& itemName)
{
    auto itemPath = currentPath / itemName;

    auto item = std::filesystem::directory_entry(itemPath);


    return item.is_directory();

}

bool FileBrowser::isFile(const std::string& itemName)
{
    auto itemPath = currentPath / itemName;

    auto item = std::filesystem::directory_entry(itemPath);
    return item.is_regular_file();


}

std::string FileBrowser::getCurrentPath()
{
    return currentPath.string();
}

void FileBrowser::refreshContents() {
    directories_.clear();
    files_.clear();
    items_.clear();
    items_.push_back("..");
    for (const auto& entry : fs::directory_iterator(currentPath)) {
        if (entry.is_directory()) {
            directories_.push_back(entry.path().filename().string());
        }
        else if (entry.is_regular_file()) {
            files_.push_back(entry.path().filename().string());
        }

        items_.push_back(entry.path().filename().string());


    }
}