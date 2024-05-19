#include <iostream>
#include <stb/stb_image.h>
#include "GLFW/glfw3.h"
#include <tuple>

class Loader {
private:
    static int width, height, channels;
    static unsigned char* data;
    static bool isDataFreed;
    static bool LoadItem(const std::string& path);
public:
    Loader();
    ~Loader();



    static GLFWimage LoadGLFWImage(const std::string& path);


    static std::string LoadItemAsString(const std::string& path);

    static unsigned char* LoadItemAsBytes(const std::string& path);

    static std::tuple<int, int, int> getData();

   
};
