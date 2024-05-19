
// Embedded Development Server or Visual Secure Shell 

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vissh/datatypes.h>
#include <layer_core/imgui_layer.h>
#include <layer_core/layer_manager.h>
#include <vissh/ssh_manager.h>
#include <vissh/virtual_environment.h>


int main() {

    // Set with ini file
    EDISESettings globalSettings;

    globalSettings.sshHostname = "10.197.1.101";
    globalSettings.sshUsername = "root";
    globalSettings.sshPassword = "milkv";


    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "Visual Secure Shell" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "System Information" << std::endl;
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(1200, 800, "Visual Secure Shell - EDISE", NULL, NULL);

    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);





    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Print Graphics Card Information
    std::cout << "Graphics Card: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // Check if the desired OpenGL version is supported
    if (!GLAD_GL_VERSION_3_3) {
        std::cerr << "OpenGL 3.3 not supported" << std::endl;
        return -1;
    }
    std::cout << "----------------------------------------------------------" << std::endl;


    ImGuiLayer* imguiLayer = new ImGuiLayer(window);

    LayerManager* layerManager = new LayerManager();
    VirtualEnvironment* virtualEnvironment = new VirtualEnvironment();
    SSHManager* sshManager = new SSHManager();

    
    imguiLayer->setSettings(globalSettings);

    imguiLayer->setLayerManager(layerManager);

    imguiLayer->setSSHManager(sshManager);

    imguiLayer->setVirtualEnvironment(virtualEnvironment);

    layerManager->addLayer(imguiLayer);

    sshManager->addConnection("192.168.42.1", "root", "milkv");
    

    auto connections = sshManager->listConnections();

    for (auto& connection : connections) {
        std::cout << connection->getHandle() << std::endl;
        connection->Connect();
    }
    





    


    while (!glfwWindowShouldClose(window)) {
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        layerManager->updateLayers();



        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }


    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
