#include "camera.h"
#include "shader.h"


Camera::Camera()
{
}
Camera::Camera(int width, int height, Vector3 position)
{
    Camera::width = width;
    Camera::height = height;
    transform.position = position;
    transform.rotation = Quaternion::Identity; // Identity quaternion for initial rotation

}

void Camera::updateMatrix()
{
    // Convert the quaternion rotation to a forward direction
    Vector3 forwardDirection = Quaternion::Rotate(transform.rotation, -Vector3::Forward); // Assuming operator* is implemented for rotation

    Matrix4x4 view = Matrix4x4::LookAt(transform.position, transform.position + forwardDirection, Vector3::Up);
    Matrix4x4 projection = Matrix4x4::Perspective(fovDeg, (float)width / height, nearPlane, farPlane);

    cameraMatrix = projection * view; // Assuming operator* is implemented for Matrix4x4
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
    // Exports camera matrix
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, cameraMatrix.value_ptr());
}

Matrix4x4 Camera::getProjectionMatrix()
{
    Matrix4x4 projection = Matrix4x4::Perspective(fovDeg, (float)width / height, nearPlane, farPlane);
    return projection;

}
Matrix4x4 Camera::getViewMatrix()
{
    Vector3 forwardDirection = Quaternion::Rotate(transform.rotation, -Vector3::Forward); // Assuming operator* is implemented for rotation
    Matrix4x4 view = Matrix4x4::LookAt(transform.position, transform.position + forwardDirection, Vector3::Up);
    return view;
}


void Camera::Inputs(GLFWwindow* window)
{

    // Calculate the forward direction
    Vector3 forwardDir = Quaternion::Rotate(transform.rotation, Vector3(0, 0, -1));

    // Calculate the right direction (perpendicular to forward and up)
    Vector3 rightDir = Vector3::Cross(Vector3::Up, forwardDir);

    // Handles key inputs
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        transform.position += forwardDir * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        transform.position += rightDir * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        transform.position -= forwardDir * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        transform.position -= rightDir * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        transform.position += speed * Vector3::Up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        transform.position -= speed * Vector3::Up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        speed = 0.4f;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        speed = 0.1f;
    }

    // Handles mouse inputs
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    {
        // Hides mouse cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Prevents camera from jumping on the first click
        if (firstClick)
        {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }

        // Stores the coordinates of the cursor
        double mouseX;
        double mouseY;
        // Fetches the coordinates of the cursor
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
        // and then "transforms" them into degrees
        float deltaX = (float)(mouseX - (width / 2)) / width;
        float deltaY = (float)(mouseY - (height / 2)) / height;

        float rotX = sensitivity * deltaY;
        float rotY = sensitivity * deltaX;


        cumulativePitch += rotX;
        if (cumulativePitch > 80.0f)
        {
            cumulativePitch = 80.0f;
            rotX = 0; // Prevent further upward rotation
        }
        else if (cumulativePitch < -80.0f)
        {
            cumulativePitch = -80.0f;
            rotX = 0; // Prevent further downward rotation
        }


        Quaternion rotXQuat = Quaternion::FromAxisAngle(Vector3::Right, -rotX);
        Quaternion rotYQuat = Quaternion::FromAxisAngle(Vector3::Up, -rotY);


        // Apply the rotations to the camera's rotation quaternion
        transform.rotation = rotYQuat * transform.rotation; // Apply yaw rotation globally
        transform.rotation = transform.rotation * rotXQuat; // Apply pitch rotation locally
        transform.rotation = Quaternion::Normalize(transform.rotation);


        // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
        glfwSetCursorPos(window, (width / 2), (height / 2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
    {
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Makes sure the next time the camera looks around it doesn't jump
        firstClick = true;
    }
}


