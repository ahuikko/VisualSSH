#pragma once

#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>


#include<glad/glad.h>
#include <GLFW/glfw3.h>




std::string getFileContents(const char* fileName);

class Shader
{
public:
	GLuint ID;
	Shader();
	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();

	void compileErrors(unsigned int shader, const char* type);

};




#endif