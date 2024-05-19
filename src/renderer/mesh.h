#pragma once

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <iostream>
#include "camera.h"
#include "transform.h"

#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "shader.h"
#include "texture.h"
#include "scene.h"




    using glm::vec3;
    using glm::quat;
    using glm::mat4;

	class  Mesh
	{
	public:

		std::vector <Vertex> vertices;
		std::vector <GLuint> indices;
		std::vector <Texture> textures;

		VAO VAO;


		Mesh();
		Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

		void Draw(Shader& shader, Camera& camera, Matrix4x4 matrix, Vector3 translation = Vector3(0.f, 0.f, 0.f), Quaternion rotation = Quaternion::Identity, Vector3 scale = Vector3(1.f,1.f,1.f));


		Transform transform;	// Transform attached to this object


	};


