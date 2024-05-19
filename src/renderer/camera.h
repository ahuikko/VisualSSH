#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H


#include "transform.h"
#include "../math/vector3.h"
#include "../math/quaternion.h"
#include "../math/matrix4x4.h"

#include "shader.h"



class Camera
{

public:
	Camera();


	Transform transform;

	// Stores the main vectors of the camera


	Matrix4x4 cameraMatrix = Matrix4x4(1.0f);


	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	// Stores the width and height of the window
	int width;
	int height;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float speed = 0.1f;
	float sensitivity = 100.0f;
	float nearPlane = 0.1f;
	float farPlane = 150.f;

	float fovDeg = 75.f;
	float cumulativePitch = 0.0f; // Cumulative pitch angle

	// Camera constructor to set up initial values
	Camera(int width, int height, Vector3 position);

	// Updates the camera matrix to the Vertex Shader
	void updateMatrix();
	// Exports the camera matrix to a shader
	void Matrix(Shader& shader, const char* uniform);
	Matrix4x4 getProjectionMatrix();
	Matrix4x4 getViewMatrix();
	// Handles camera inputs
	void Inputs(GLFWwindow* window);
};


#endif // !CAMERA_CLASS_H