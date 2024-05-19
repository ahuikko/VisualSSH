#pragma once


#ifndef LAYER_H
#define LAYER_H


#include <GLFW/glfw3.h> // Make sure you include GLFW here or in a precompiled header
#include <string>

class Layer
{


private:
    GLFWwindow* _window;
    std::string _name;


protected:
    bool _enabled = true;

public:
    // Constructor declaration (if you have any specific constructor)



    Layer(GLFWwindow* window, std::string name);

    virtual ~Layer() = default; // Use a virtual destructor for proper cleanup


    virtual void Update();

    std::string getName() {
        return _name;
    }

    // Getter for the window
    GLFWwindow* getWindow() const {
        return _window;
    }

    bool isEnabled()
    {
        return _enabled;
    }
    void setEnabled(bool enabled) {
        _enabled = enabled;
    }

};


#endif
