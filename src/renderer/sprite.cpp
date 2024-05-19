#include "sprite.h"

#include <nlohmann/json.hpp>
#include <utilities/loader.h>




using json = nlohmann::json;



Sprite3D::Sprite3D()
{

}

Sprite3D::Sprite3D(const char* textureFile)
{
	spriteTexture = Texture(textureFile, "diffuse", 0); // Use texture type "diffuse" and slot 0
	spriteMesh = CreateSpriteMesh(); // Assuming textureFile is defined or passed as an argument
}

Sprite3D::Sprite3D(const Texture& texture)
{
	spriteTexture = texture;
	spriteMesh = CreateSpriteMesh();

}



void Sprite3D::Draw(Shader& shader, Camera& camera)
{
	Vector3 spritePosition = transform.position;
	Quaternion spriteRotation = transform.rotation; 


	Vector3 spriteScale = Vector3(transform.scale.x, transform.scale.y, 1.f);


	Matrix4x4 translationMatrix = Matrix4x4::Translate(Matrix4x4(1.0f), spritePosition);
	Matrix4x4 rotationMatrix = spriteRotation.ToMatrix4x4(); 
	Matrix4x4 scaleMatrix = Matrix4x4::Scale(Matrix4x4(1.0f), spriteScale);

	Matrix4x4 transform = translationMatrix * rotationMatrix * scaleMatrix;

	spriteMesh.textures[0].Bind(); 
	spriteMesh.textures[0].texUnit(shader, "spriteTexture", 0); 

	spriteMesh.Draw(shader, camera, transform);
}

Mesh Sprite3D::CreateSpriteMesh()
{

	// Define vertices for a quad (two triangles)
	std::vector<Vertex> vertices = {
		// Position         // Normal         // Color         // TexUV
		{{0.5f, 0.5f, 0.0f},{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f},{1.0f, 1.0f}}, // top right
		{{0.5f, -0.5f, 0.0f},{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f},{1.0f, 0.0f}}, // bottom right
		{{-0.5f, -0.5f, 0.0f},{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f},{0.0f, 0.0f}}, // bottom left
		{{-0.5f, 0.5f, 0.0f},{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f},{0.0f, 1.0f}}  // top left
	};


	// Define indices for drawing the two triangles
	std::vector<GLuint> indices = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// Create a mesh object
	std::vector<Texture> textures = { spriteTexture };
	return Mesh(vertices, indices, textures);
}


Sprite2D::Sprite2D()
{
}

Sprite2D::Sprite2D(const char* textureFile)
{
	spriteTexture = Texture(textureFile, "diffuse", 0); // Use texture type "diffuse" and slot 0
	spriteMesh = CreateSpriteMesh(textureFile); // Assuming textureFile is defined or passed as an argument
}

void Sprite2D::Draw(Shader& shader, Camera& camera)
{
	Vector3 spritePosition = transform.position;

	// Billboarding: Align the sprite's rotation with the camera's rotation
	Matrix4x4 viewMatrix = camera.getViewMatrix();
	Matrix4x4 projectionMatrix = camera.getProjectionMatrix();


	Matrix4x4 billboardingRotation = Matrix4x4(Matrix3x3(viewMatrix));
	billboardingRotation.Transposed();

	// Set the scale of the sprite
	Vector3 spriteScale = Vector3(transform.scale.x, transform.scale.y, 1.f); // Assuming a 2D sprite

	// Create the transformation matrix
	Matrix4x4 translationMatrix = Matrix4x4::Translate(Matrix4x4(1.0f), spritePosition);
	Matrix4x4 scaleMatrix = Matrix4x4::Scale(Matrix4x4(1.0f), spriteScale);

	// Combine the transformations
	Matrix4x4 transform = translationMatrix * billboardingRotation * scaleMatrix;

	spriteMesh.textures[0].Bind(); // Assuming the first texture is the sprite's texture
	spriteMesh.textures[0].texUnit(shader, "spriteTexture", 0); // Assuming "spriteTexture" is the sampler2D in your fragment shader
	spriteMesh.Draw(shader, camera, transform);

}

Mesh Sprite2D::CreateSpriteMesh(const char* textureFile)
{
	// Define vertices for a quad (two triangles)
	std::vector<Vertex> vertices = {
		// Position         // Normal         // Color         // TexUV
		{{0.5f, 0.5f, 0.0f},{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f},{1.0f, 1.0f}}, // top right
		{{0.5f, -0.5f, 0.0f},{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f},{1.0f, 0.0f}}, // bottom right
		{{-0.5f, -0.5f, 0.0f},{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f},{0.0f, 0.0f}}, // bottom left
		{{-0.5f, 0.5f, 0.0f},{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f},{0.0f, 1.0f}}  // top left
	};


	// Define indices for drawing the two triangles
	std::vector<GLuint> indices = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// Create a mesh object
	std::vector<Texture> textures = { spriteTexture };
	return Mesh(vertices, indices, textures);
}



