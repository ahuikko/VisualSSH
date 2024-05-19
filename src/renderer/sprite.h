#pragma once
#ifndef SPRITE_CLASS_H
#define SPRITE_CLASS_H

#include "transform.h"
#include "shader.h"
#include "camera.h"
#include <vector>

#include "mesh.h"

#include "vbo.h";




class Sprite3D
{
public:

	Transform transform;
	Texture spriteTexture;

	Sprite3D();
	Sprite3D(const char* textureFile);
	Sprite3D(const Texture& texture);

	

	void Draw(Shader& shader, Camera& camera);

	Mesh CreateSpriteMesh();

	Mesh spriteMesh;

private:
	const char* textureFile;

	std::vector<unsigned char> getData();

};

class Sprite2D
{
public:
	Transform transform;

	Sprite2D();
	Sprite2D(const char* textureFile);

	Texture spriteTexture;

	void Draw(Shader& shader, Camera& camera);


private:
	const char* textureFile;
	Mesh CreateSpriteMesh(const char* textureFile);
	Mesh spriteMesh;

};







#endif