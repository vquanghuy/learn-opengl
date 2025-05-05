#ifndef CAMERA_H
#define CAMERA_H

#include <glad/gl.h> // Include glad
#include <glm/glm.hpp> // Core GLM
#include <glm/gtc/matrix_transform.hpp> // glm::lookAt, glm::perspective
#include <glm/gtc/constants.hpp> // For glm::pi

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW         = -90.0f; // Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
const float PITCH       =  0.0f;
const float SPEED       =  5.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes (renamed to camel case)
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Euler Angles (renamed to camel case)
    float yaw;
    float pitch;

    // Camera options (renamed to camel case)
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw = YAW, float pitch = PITCH);

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 getViewMatrix() const;

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboard(CameraMovement direction, float deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(float yoffset);

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};
#endif

