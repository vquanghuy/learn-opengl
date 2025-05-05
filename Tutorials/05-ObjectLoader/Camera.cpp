#include "Camera.h"

// Include necessary headers for trigonometric functions
#include <cmath>
#include <algorithm> // For std::clamp

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) // Updated initializer list
{
    this->position = position; // Use 'this->' to distinguish from parameter
    worldUp = up;
    this->yaw = yaw; // Use 'this->' to distinguish from parameter
    this->pitch = pitch; // Use 'this->' to distinguish from parameter
    updateCameraVectors();
}

// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) // Updated initializer list
{
    position = glm::vec3(posX, posY, posZ);
    worldUp = glm::vec3(upX, upY, upZ);
    this->yaw = yaw; // Use 'this->' to distinguish from parameter
    this->pitch = pitch; // Use 'this->' to distinguish from parameter
    updateCameraVectors();
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix() const
{
    // The LookAt matrix creates a view matrix based on a camera position, a target position, and an up vector.
    // We use the camera's position, the point it's looking at (position + front), and its up vector. (Updated member names)
    return glm::lookAt(position, position + front, up);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
    if (direction == UP)
        position += worldUp * velocity;
    if (direction == DOWN)
        position -= worldUp * velocity;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;
    
    yaw   += xoffset;
    pitch += yoffset;
    
    // Make sure that when pitch is out of bounds, the screen doesn't flip
    if (constrainPitch)
    {
        // Clamp pitch to avoid flipping (e.g., between -89 and 89 degrees)
        pitch = std::clamp(pitch, -89.0f, 89.0f);
    }
    
    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::processMouseScroll(float yoffset)
{
    zoom -= (float)yoffset;
    // Clamp zoom to a reasonable range
    zoom = std::clamp(zoom, 1.0f, 45.0f);
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
    // Calculate the new Front vector using spherical coordinates derived from Yaw and Pitch
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front); // Normalize to get a unit vector (Updated member name)
    
    // Also re-calculate the Right and Up vector
    // The cross product of WorldUp and Front gives the Right vector (perpendicular to both)
    this->right = glm::normalize(glm::cross(this->front, worldUp));
    // The cross product of Right and Front gives the Up vector (perpendicular to both)
    this->up = glm::normalize(glm::cross(right, this->front));
}

