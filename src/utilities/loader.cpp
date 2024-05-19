#include "loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <iostream>
#include <sstream>
#include<fstream>

// Initialize static members
int Loader::width = 0;
int Loader::height = 0;
int Loader::channels = 0;
unsigned char* Loader::data = nullptr;
bool Loader::isDataFreed = true;  // Add this line

Loader::Loader() {
    // Constructor logic if any (you can keep it empty if there's no additional logic)
}

Loader::~Loader() {
    if (data && !isDataFreed) {  // Check the isDataFreed flag
        stbi_image_free(data);
        data = nullptr;          // Clear the data pointer after freeing memory
        isDataFreed = true;      // Mark data as freed
    }
}

bool Loader::LoadItem(const std::string& path) {
    if (data) { // If data already exists, free it before loading a new image
        stbi_image_free(data);
        data = nullptr;
        isDataFreed = true;
    }
    data = stbi_load(path.c_str(), &width, &height, &channels, 4); // 4 for RGBA
    std::cout << "loaded: " << path << " w: " << width << " h: " << height << " c: " << channels << std::endl;

    if (!data) {
        std::cerr << "Error loading image: " << path << std::endl;
        return false;
    }
    isDataFreed = false;  // Mark data as not freed
    return true;
}

unsigned char* Loader::LoadItemAsBytes(const std::string& path)
{

    if (LoadItem(path))
    {
        // Flips the image so it appears right side up
        stbi_set_flip_vertically_on_load(true);

        // Reads the image from a file and stores it in bytes
        unsigned char* bytes = data;

        if (bytes == nullptr)
        {

            // Handle error, could not load image
            return nullptr;
        }

        return bytes;
    }

}




std::tuple<int,int,int> Loader::getData()
{
    return std::make_tuple(width,height,channels);
}

GLFWimage Loader::LoadGLFWImage(const std::string& path)
{
    if (LoadItem(path))
    {
        GLFWimage image;
        image.width = width;
        image.height = height;
        image.pixels = data;
        return image;
    }
    
}





std::string Loader::LoadItemAsString(const std::string& path)
{

    std::ifstream in(path, std::ios::binary);

    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);

}
