#include "imgui_layer.h"

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <chrono>
#include <iomanip> 
#include <iostream>
#include <utilities/loader.h>

#include <vissh/vssh.h>

ImGuiLayer::ImGuiLayer(GLFWwindow* window) :Layer(window, "ImGui") {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    // Initialize ImGui GLFW and OpenGL implementations
    ImGui_ImplOpenGL3_Init("#version 330");

}

ImGuiLayer::~ImGuiLayer() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    //for (auto& entry : _textureMap) {
    //    GLuint textureID = entry.second;
    //    glDeleteTextures(1, &textureID);
    //}

}

void ImGuiLayer::Begin() {
    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::toggleBool(bool& var)
{
    var = !var;
}

void ImGuiLayer::updateTimeString() {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tmNow;
    localtime_s(&tmNow, &now); // Safely obtain local time

    std::stringstream ss;
    ss << std::put_time(&tmNow, "%Y-%m-%d %X"); // Format the time as desired

    _currentTimeString = ss.str(); // Update the member variable
}


void ImGuiLayer::Update() {

    Begin();
    
    updateTimeString();


    renderFileExplorer(_uiWindows.showFileExplorer);


    renderLayerManager(_uiWindows.showLayerManager);
    renderSSHSettings(_uiWindows.showSSHSettings);
    //renderContextMenu(_uiWindows.showContextMenu);

    OnImGuiRender();

    End();


}

void ImGuiLayer::OnImGuiRender() {
    // Example ImGui window
    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("Applications")) {
            if (ImGui::MenuItem("File Explorer")) { toggleBool(_uiWindows.showFileExplorer); }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Dev Tools")) {
            if (ImGui::MenuItem("Layer Manager")) { toggleBool(_uiWindows.showLayerManager); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Connection")) {
            if (ImGui::MenuItem("SSH")) { toggleBool(_uiWindows.showSSHSettings); }
            ImGui::EndMenu();
        }

        ImGui::SameLine(ImGui::GetWindowWidth() - 160);
        ImGui::Separator();
        ImGui::TextUnformatted(_currentTimeString.c_str());

        ImGui::EndMainMenuBar();
    }


}



void ImGuiLayer::renderLayerManager(bool show) {
    if (!show) return;


    ImGui::SetNextWindowSize(ImVec2(250, 300));
    ImGui::Begin("Layer Manager", &_uiWindows.showLayerManager);
    ImGui::BeginChild("DrawOrder", ImVec2(0, 0), true);

    for (int i = 0; i < _layerManager->getLayers().size(); ++i) {
        auto& layer = _layerManager->getLayers()[i];
        std::string layerName = std::to_string(i + 1) + ": " + layer->getName(); // Add ordering number

        ImGui::PushID(i);

        ImVec4 textColor = layer->isEnabled() ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        ImVec4 bgColor = ImVec4(0.20f, 0.20f, 0.20f, 0.65f); // A dark background color for all layers
        ImGui::PushStyleColor(ImGuiCol_Text, textColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, bgColor); // Color for when the selectable is hovered
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, bgColor);  // Color for when the selectable is held clicked
        ImGui::PushStyleColor(ImGuiCol_Header, bgColor);        // Color for the normal state

        if (ImGui::Selectable(layerName.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowItemOverlap)) {
            // Selection logic
        }

        ImGui::PopStyleColor(4);

        // Drag and drop source
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
            ImGui::SetDragDropPayload("LAYER_DRAG", &i, sizeof(int));
            ImGui::Text("%s", layerName.c_str());
            ImGui::EndDragDropSource();
        }

        // Drag and drop target
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LAYER_DRAG")) {
                int draggedLayerID = *(const int*)payload->Data;
                _layerManager->ReorderLayer(draggedLayerID, i);
            }
            ImGui::EndDragDropTarget();
        }
        if (layer->getName() != "ImGui") {
            // Context menu
            if (ImGui::BeginPopupContextItem("LayerContextMenu")) {

                    if (layer->isEnabled()) {
                        if (ImGui::MenuItem("Disable")) {
                            layer->setEnabled(false);
                        }
                    }
                    else {
                        if (ImGui::MenuItem("Enable")) {
                            layer->setEnabled(true);
                        }
                    }
            

            
                ImGui::EndPopup();
            }
        }
        ImGui::PopID();
    }

    ImGui::EndChild();
    ImGui::End();
}

std::vector<SSHProfile> profiles = {
    {"Profile 1", "10.197.1.101", "root", "milkv"},
    {"Profile 2", "10.0.0.2", "root", "pass2"},

};



void ImGuiLayer::renderSSHSettings(bool show)
{
    if (!show) return;

    ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_FirstUseEver);
    ImGui::Begin("SSH Settings", &_uiWindows.showSSHSettings);


    
    // Start a child window to group SSH settings
    ImGui::BeginChild("SSH", ImVec2(500, 300), true); // Adjusted width to leave space for profiles

    // Configuration for input fields
    float desiredInputWidth = 200.0f;
    float cursorPos = 85;

    // Display connection status
    ImGui::Text("Handles");
    ImGui::SameLine();
    ImGui::SetCursorPosX(cursorPos);
    ImGui::PushItemWidth(desiredInputWidth);

    ImGui::BeginChild("SSHConnections", ImVec2(500, 150), true); // Adjusted width to leave space for profiles

    for (auto& connection : _sshManager->listConnections()) {
        if (connection == nullptr) return;

        std::string handleText = "Handle: " + connection->getHandle();
        std::string statusText = "Status: " + connection->getStatusStr();
        std::string lifetime = connection->getConnectionLifeTimeStr();
        if (ImGui::Selectable(handleText.c_str())) {
            // Handle selection logic here if needed
        }

        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Status: %s\nLifetime: %s", statusText.c_str(), lifetime.c_str());
        }
    }

    ImGui::EndChild();

    ImGui::PopItemWidth();

    // IP address input
    ImGui::Text("Hostname");
    ImGui::SameLine(cursorPos);
    ImGui::PushItemWidth(desiredInputWidth);
    ImGui::InputText("##ipaddress", &_settings.sshHostname);
    ImGui::PopItemWidth();

    // Username input
    ImGui::Text("Username");
    ImGui::SameLine(cursorPos);
    ImGui::PushItemWidth(desiredInputWidth);
    ImGui::InputText("##username", &_settings.sshUsername);
    ImGui::PopItemWidth();

    // Password input
    ImGui::Text("Password");
    ImGui::SameLine(cursorPos);
    ImGui::PushItemWidth(desiredInputWidth);
    ImGui::InputText("##password", &_settings.sshPassword, ImGuiInputTextFlags_Password);
    ImGui::PopItemWidth();

    // Connect or disconnect button
    if (ImGui::Button(_sshManager->getFree()->isConnected() ? "Disconnect SSH" : "Connect SSH")) {
        _sshManager->getFree()->isConnected() ? _sshManager->getFree()->Disconnect() : _sshManager->getFree()->Connect();
    }

    // End the child window and the main window
    ImGui::EndChild();
    ImGui::SameLine();
    // Render profiles on the right side
    ImGui::BeginChild("Profiles", ImVec2(0, 0), true);
    for (auto& profile : profiles) {
        if (ImGui::Selectable(profile.profileID.c_str())) {
            _settings.sshHostname = profile.sshHostname;
            _settings.sshUsername = profile.sshUsername;
            _settings.sshPassword = profile.sshPassword;
        }
    }
    ImGui::EndChild();

    ImGui::End();
}



void ImGuiLayer::displayFilesInTab(const std::string& directory, bool shouldUpdate = false) {
    if (ImGui::BeginTabItem(directory.c_str())) {
        if (shouldUpdate) {

        }
        std::vector<std::string> files = std::vector<std::string>{ ".", "..", "test" };    //listFiles(directory);
        for (const auto& file : files) 
        {
            ImGui::Selectable(file.c_str());
        }

        ImGui::EndTabItem();
    }
}

std::vector<FileSystemItem> ImGuiLayer::updateFiles(const std::string& path, bool shouldUpdate)
{
    if (!shouldUpdate) {
        return { FileSystemItem("NO CONNECTION...", "N/A") };
    }

    // Fetch the list of files
    std::string command = "ls -1 " + path;
    std::string result = _sshManager->getFree()->executeCommand(command);
    std::vector<FileSystemItem> items;
    items.push_back(FileSystemItem("..", "Directory"));  // Parent directory

    std::istringstream iss(result);
    std::string fileName;

    while (std::getline(iss, fileName)) {
        if (fileName.empty()) {
            continue;  // Skip empty lines
        }

        // Use the file command to determine the file type
        std::string fileCommand = "file -b \"" + path + "/" + fileName + "\"";
        std::string fileType = _sshManager->getFree()->executeCommand(fileCommand);

        // Trim any trailing whitespace or newline characters from the fileType
        fileType.erase(std::remove(fileType.begin(), fileType.end(), '\n'), fileType.end());
        fileType.erase(std::remove(fileType.begin(), fileType.end(), '\r'), fileType.end());

        items.push_back(FileSystemItem(fileName, fileType));
    }

    return items;
}


void ImGuiLayer::renderFileContextMenu(const FileSystemItem& item) {

    ImGui::Text(item.name.c_str());
    ImGui::Separator();
    ImGui::Text(item.fileType.c_str());
    ImGui::Separator();
    if (ImGui::MenuItem("Refresh"))
    {
        //fileSystem.update(true); // Force the update with true value
    }

    if (ImGui::MenuItem("Select"))
    {
        //fileSystem.selectedItemPath = fileSystem.currentPath;
    }
    if (ImGui::MenuItem("Edit with Eedit")) // Embedded Editor
    {
        //fileSystem.selectedItemPath = fileSystem.currentPath;
    }

    if (ImGui::BeginMenu("Programs"))
    {
        if (ImGui::BeginMenu("Compression and Archiving"))
        {

            if (ImGui::MenuItem("zip"))
            {
                //std::string command = "zip archive.zip " + fileSystem.currentPath;
                //executeCommand(command);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("unzip"))
            {
                //std::string command = "unzip archive.zip -d " + fileSystem.currentPath;
                //executeCommand(command);
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("tar"))
            {
                //std::string command = "tar -cvf archive.tar " + fileSystem.currentPath;
                //executeCommand(command);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("gzip"))
            {
                //std::string command = "gzip " + fileSystem.currentPath;
                //executeCommand(command);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("gunzip"))
            {
                //std::string command = "gunzip " + fileSystem.currentPath;
                //executeCommand(command);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("bzip2"))
            {
                //std::string command = "bzip2 " + fileSystem.currentPath;
                //executeCommand(command);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("bunzip2"))
            {
                //std::string command = "bunzip2 " + fileSystem.currentPath;
                //executeCommand(command);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndMenu();
        }

        
        ImGui::EndMenu();



    }
    ImGui::Separator();
    if (ImGui::MenuItem("Copy path"))
    {
        //fileSystem.selectedItemPath = fileSystem.currentPath;
    }
    ImGui::Separator();
    if (ImGui::MenuItem("Copy"))
    {
        //fileSystem.selectedItemPath = fileSystem.currentPath;
    }
    if (ImGui::MenuItem("Cut"))
    {
        //fileSystem.selectedItemPath = fileSystem.currentPath;
    }
    ImGui::Separator();
    if (ImGui::MenuItem("Rename"))
    {
        //fileSystem.selectedItemPath = fileSystem.currentPath;
    }


    if (ImGui::BeginMenu("Delete"))
    {
        if (ImGui::MenuItem("Yes"))
        {

            //newDirToPath(fileSystem.currentPath);

            ImGui::CloseCurrentPopup();
        }
        if (ImGui::MenuItem("No"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndMenu();
    }
    ImGui::Separator();
    if (ImGui::MenuItem("Properties"))
    {
        //fileSystem.selectedItemPath = fileSystem.currentPath;
    }
    
}

void ImGuiLayer::renderDirectoryContextMenu(const FileSystemItem& item) {

    ImGui::Text(item.name.c_str());
    ImGui::Separator();
    ImGui::Text(item.fileType.c_str());
    ImGui::Separator();
    if (ImGui::MenuItem("Refresh"))
    {
        // fileSystem.update(true); // Force the update with true value
    }

    if (ImGui::MenuItem("Open in new Tab"))
    {
        // fileSystem.openDirectory(item); // Logic to open the directory
    }


    if (ImGui::BeginMenu("Programs"))
    {
        if (ImGui::BeginMenu("Compression and Archiving"))
        {
            if (ImGui::MenuItem("zip"))
            {
                // std::string command = "zip archive.zip " + fileSystem.currentPath;
                // executeCommand(command);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("unzip"))
            {
                // std::string command = "unzip archive.zip -d " + fileSystem.currentPath;
                // executeCommand(command);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("tar"))
            {
                // std::string command = "tar -cvf archive.tar " + fileSystem.currentPath;
                // executeCommand(command);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("gzip"))
            {
                // std::string command = "gzip " + fileSystem.currentPath;
                // executeCommand(command);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("gunzip"))
            {
                // std::string command = "gunzip " + fileSystem.currentPath;
                // executeCommand(command);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("bzip2"))
            {
                // std::string command = "bzip2 " + fileSystem.currentPath;
                // executeCommand(command);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("bunzip2"))
            {
                // std::string command = "bunzip2 " + fileSystem.currentPath;
                // executeCommand(command);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }

    ImGui::Separator();
    if (ImGui::MenuItem("Copy path"))
    {
        // fileSystem.selectedItemPath = fileSystem.currentPath;
    }
    ImGui::Separator();
    if (ImGui::MenuItem("Copy"))
    {
        // fileSystem.selectedItemPath = fileSystem.currentPath;
    }
    if (ImGui::MenuItem("Cut"))
    {
        // fileSystem.selectedItemPath = fileSystem.currentPath;
    }
    ImGui::Separator();
    if (ImGui::MenuItem("Rename"))
    {
        // fileSystem.selectedItemPath = fileSystem.currentPath;
    }

    if (ImGui::BeginMenu("Delete"))
    {
        if (ImGui::MenuItem("Yes"))
        {
            // fileSystem.deleteDirectory(fileSystem.currentPath);

            ImGui::CloseCurrentPopup();
        }
        if (ImGui::MenuItem("No"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndMenu();
    }
    ImGui::Separator();
    if (ImGui::MenuItem("Properties"))
    {
        // fileSystem.selectedItemPath = fileSystem.currentPath;
    }
}

void ImGuiLayer::renderFileExplorer(bool show) {
    if (!show) return;

    static int tabCounter = 0;  // Counter to generate unique tab IDs
    static std::string currentTabID = "Tab0";
    static bool filesFetched = false;  // Static variable to keep track if files have been fetched

    // Ensure the initial tab exists
    if (_virtualEnvironment->getTabs().empty()) {
        _virtualEnvironment->createNewTab(currentTabID);
        std::vector<FileSystemItem> files = updateFiles("/", true);
        _virtualEnvironment->setTab(currentTabID, files);
    }

    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
    ImGui::Begin("File Explorer", &_uiWindows.showFileExplorer);
    if (ImGui::BeginTabBar("Tabs")) {

        // Add new tab button on the right
        if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing)) {
            // Create a new tab with a unique ID
            std::string newTabID = "Tab" + std::to_string(++tabCounter);
            _virtualEnvironment->createNewTab(newTabID);

            // Fetch the content for the new tab
            std::vector<FileSystemItem> files = updateFiles("/", true);
            _virtualEnvironment->setTab(newTabID, files);

            // Set the current tab to the new tab
            currentTabID = newTabID;
            filesFetched = false;  // Ensure files are fetched for the new tab
        }

        // Render existing tabs
        for (const auto& tabPair : _virtualEnvironment->getTabs()) {
            const std::string& tabID = tabPair.first;
            Tab& tab = _virtualEnvironment->getTab(tabID);

            std::string tabLabel = tab.getPwd() + "###" + tabID;

            if (ImGui::BeginTabItem(tabLabel.c_str())) {
                currentTabID = tabID;

                // Fetch files if not already fetched
                if (!filesFetched || tab.getContent().empty()) {
                    std::vector<FileSystemItem> files = updateFiles(tab.getPwd(), true);
                    _virtualEnvironment->setTab(tabID, files);
                    filesFetched = true;
                }

                const std::vector<FileSystemItem>& content = tab.getContent();
                for (size_t i = 0; i < content.size(); ++i) {
                    const FileSystemItem& item = content[i];
                    const std::string& entry = item.name;
                    const std::string& fileType = item.fileType;

                    // Left-click selection
                    ImGui::PushID(static_cast<int>(i)); // Ensure unique ID for context menus
                    if (ImGui::Selectable(entry.c_str())) {
                        std::cout << "Selected: " << entry << std::endl;
                    }

                    // Left-doubleclick for navigation
                    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                        std::cout << "Double-clicked: " << entry << std::endl;

                        if (entry == "..") {
                            // Navigate up one directory level
                            std::string currentPath = tab.getPwd();
                            std::string newPath = tab.normalizePath(currentPath + "/..");
                            tab.setPwd(newPath);
                            std::vector<FileSystemItem> files = updateFiles(newPath, true);
                            _virtualEnvironment->setTab(tabID, files);
                        }
                        else if (fileType.find("directory") != std::string::npos) {
                            // Open directory logic
                            std::string newPath = tab.getPwd() + "/" + entry;
                            newPath = tab.normalizePath(newPath);
                            tab.setPwd(newPath);
                            std::vector<FileSystemItem> files = updateFiles(newPath, true);
                            _virtualEnvironment->setTab(tabID, files);
                        }
                        else {
                            // Handle file double-click logic
                            // openFile(entry);
                        }
                    }

                    // Right-click context menu
                    if (ImGui::BeginPopupContextItem("Context Menu", ImGuiMouseButton_Right)) {
                        ImGui::Text("Type: %s", fileType.c_str());
                        ImGui::Separator();

                        if (fileType.find("directory") != std::string::npos) {
                            renderDirectoryContextMenu(item);
                        }
                        else {
                            renderFileContextMenu(item);
                        }

                        ImGui::EndPopup();
                    }

                    ImGui::PopID();  // Match PushID call above
                }

                ImGui::EndTabItem();
            }
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
}






void ImGuiLayer::End() {
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void ImGuiLayer::setLayerManager(LayerManager* layerManager)
{
    this->_layerManager = layerManager;
}

void ImGuiLayer::setSSHManager(SSHManager* sshManager)
{
    this->_sshManager = sshManager;
}

void ImGuiLayer::setSettings(EDISESettings settings)
{
    this->_settings = settings;
}

void ImGuiLayer::setVirtualEnvironment(VirtualEnvironment* virtualEnvironment)
{
    this->_virtualEnvironment = virtualEnvironment;
}
