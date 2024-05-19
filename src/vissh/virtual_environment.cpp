#include "virtual_environment.h"

Tab& VirtualEnvironment::getTab(const std::string& tabID) {
    auto it = _tabs.find(tabID);
    if (it != _tabs.end()) {
        return it->second;
    }
    static Tab dummyTab;  // Maintain a static dummy tab for error cases
    // Optionally log error or handle it as required
    return dummyTab;
}




bool VirtualEnvironment::removeTab(const std::string& tabID) {
    return _tabs.erase(tabID) > 0;
}




void VirtualEnvironment::setTab(std::string tabID, std::vector<FileSystemItem> content){
    _tabs[tabID].setContent(content);
}

VirtualEnvironment::VirtualEnvironment()
{
}

VirtualEnvironment::~VirtualEnvironment()
{
}


void VirtualEnvironment::createNewTab(const std::string& tabID) 
{
    Tab newTab;
    newTab.setPwd("/");
    _tabs[tabID] = newTab;
}

std::string Tab::normalizePath(const std::string& path) const
{
    std::vector<std::string> parts;
    std::stringstream ss(path);
    std::string item;
    while (std::getline(ss, item, '/')) {
        if (item == "..") {
            if (!parts.empty()) {
                parts.pop_back();
            }
        }
        else if (!item.empty() && item != ".") {
            parts.push_back(item);
        }
    }
    std::string normalizedPath;
    for (const auto& part : parts) {
        normalizedPath += "/" + part;
    }
    return normalizedPath.empty() ? "/" : normalizedPath;
}
