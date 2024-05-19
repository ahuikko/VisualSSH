#pragma once



#include "shader.h"



class Texture
{
public:

	GLuint ID;
	const char* type;
	GLuint unit;


	Texture() {};

	Texture(const char* textureFile);

	Texture(const char* image, const char* texType, GLuint slot);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();



};
