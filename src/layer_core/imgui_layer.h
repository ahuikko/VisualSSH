#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <sstream>
#include <string>
#include "layer.h"
#include "layer_manager.h"
#include <utilities/file_browser.h>
#include <imgui/imgui.h>
#include <vissh/ssh_manager.h>
#include <vissh/datatypes.h>
#include <vissh/virtual_environment.h>


struct UIWindows {
    bool showLayerManager;
    bool showSSHSettings;
    bool showFileExplorer;
    bool showContextMenu;
};


class ImGuiLayer : public Layer {

public:
    ImGuiLayer(GLFWwindow* window);
    ~ImGuiLayer();

    void Begin();
    void Update() override;
    void End();

    void setLayerManager(LayerManager* layerManager);
    void setSSHManager(SSHManager* sshManager);
    void setSettings(EDISESettings settings);
    void setVirtualEnvironment(VirtualEnvironment* virtualEnvironment);

private:
    int selected_client_id = -1;
    int selected_asset_id = -1;


    ImGuiTextFilter _filter;

    UIWindows _uiWindows;
    // Performance tracking
    float _time = 0.0f;
    int _frameCount = 0;
    float _FPS = 0.0f;
    float _frameTime = 0.0f; // In milliseconds

    // System Time
    std::string _currentTimeString;

    LayerManager* _layerManager;

    SSHManager* _sshManager;

    VirtualEnvironment* _virtualEnvironment;

    EDISESettings _settings;


protected:

    void toggleBool(bool& var);

    void OnImGuiRender();

    void renderLayerManager(bool show);
    void renderSSHSettings(bool show);
    void displayFilesInTab(const std::string& directory, bool shouldUpdate);
    std::vector<FileSystemItem> updateFiles(const std::string& path, bool shouldUpdate);

    bool isFile(std::string itemPath);

    void renderFileExplorer(bool show);
    void updateTimeString();

    void renderFileContextMenu(const FileSystemItem& item);
    void renderDirectoryContextMenu(const FileSystemItem& item);
    // File/Image manip helpers need to be added somewhere else

};

#endif // IMGUI_LAYER_H