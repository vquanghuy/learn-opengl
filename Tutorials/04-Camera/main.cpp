#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h> // Still needed for GLFW types and functions not wrapped by GLWindow

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/gtc/type_ptr.hpp>

#include "GLWindow.h"
#include "FPSLimiter.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Camera.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

Mesh loadCube() {
    float cubeRawVertices[] = {
        // positions          // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Back face
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Front face
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Left face
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Right face
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Bottom face
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Top face
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    // Number of floats per vertex in your original data
    const int FLOATS_PER_VERTEX = 5;
    // Number of vertices (36 vertices for a cube defined this way)
    const int NUM_VERTICES = sizeof(cubeRawVertices) / (FLOATS_PER_VERTEX * sizeof(float));
    
    // Create the vector of Vertex objects
    std::vector<Vertex> cubeVertices;
    cubeVertices.reserve(NUM_VERTICES); // Reserve space for efficiency
    
    // Manually populate the vector
    for (int i = 0; i < NUM_VERTICES; ++i) {
        Vertex vertex;
        
        // Populate position (3 floats)
        vertex.position.x = cubeRawVertices[i * FLOATS_PER_VERTEX + 0];
        vertex.position.y = cubeRawVertices[i * FLOATS_PER_VERTEX + 1];
        vertex.position.z = cubeRawVertices[i * FLOATS_PER_VERTEX + 2];
        
        // Populate texture coordinates (2 floats)
        vertex.texCoords.x = cubeRawVertices[i * FLOATS_PER_VERTEX + 3];
        vertex.texCoords.y = cubeRawVertices[i * FLOATS_PER_VERTEX + 4];
        
        // Add the created Vertex to the vector
        cubeVertices.push_back(vertex);
    }
    
    return Mesh(cubeVertices);
}

void processKeyInput(GLWindow* window, Camera* camera, float deltaTime) {
    // Close window on Escape key press
    if (window->getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) // Use window.getKey()
        window->requestClose(); // Use GLWindow method
    
    // Camera movement input
    if (window->getKey(GLFW_KEY_W) == GLFW_PRESS) // Use window.getKey()
        camera->processKeyboard(FORWARD, deltaTime);
    if (window->getKey(GLFW_KEY_S) == GLFW_PRESS) // Use window.getKey()
        camera->processKeyboard(BACKWARD, deltaTime);
    if (window->getKey(GLFW_KEY_A) == GLFW_PRESS) // Use window.getKey()
        camera->processKeyboard(LEFT, deltaTime);
    if (window->getKey(GLFW_KEY_D) == GLFW_PRESS) // Use window.getKey()
        camera->processKeyboard(RIGHT, deltaTime);
    if (window->getKey(GLFW_KEY_SPACE) == GLFW_PRESS) // Use window.getKey()
        camera->processKeyboard(UP, deltaTime);
    if (window->getKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // Use window.getKey()
        camera->processKeyboard(DOWN, deltaTime);
}

int main(void) {
    // Apply the debugger workaround BEFORE creating the window
    GLWindow::debuggerSleepWorkaround(1);
    
    // Create the window object using the GLWindow class
    GLWindow window;
    
    // Initialize the window and OpenGL context
    if (!window.create(WINDOW_WIDTH, WINDOW_HEIGHT, "04 - Camera", 4, 1)) {
        return -1; // Exit application if window creation failed
    }
    
    // Set window callback
    
    
    // Initialize camera
    Camera mainCamera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
    
    // Load shaders using your Shader class
    Shader cubeShader("./Assets/04-Camera/cube.vert.glsl", "./Assets/04-Camera/cube.frag.glsl");
    if (!cubeShader.load()) {
        // Handle shader loading error (message already printed by Shader::load)
        return -1; // Exit application if shader loading failed
    }
    
    // Load texture
    Texture cubeTexture = Texture("./Assets/04-Camera/cube.jpg");
    if (!cubeTexture.load()) {
        return -1; // Exit application if texture loading failed
    }
    
    // setup cube mesh
    Mesh cubeMesh = loadCube();
    if (!cubeMesh.setupMesh()) {
        return -1; // Exit application if cube loading failed
    }
    
    // Define Cube Positions in a 10x10x10 Grid
    std::vector<glm::vec3> cubePositions;
    int gridSize = 10;
    float spacing = 2.0f; // Spacing between cube centers
    
    for (int x = 0; x < gridSize; ++x) {
        for (int y = 0; y < gridSize; ++y) {
            for (int z = 0; z < gridSize; ++z) {
                glm::vec3 position;
                position.x = (float)x * spacing - (gridSize * spacing / 2.0f) + (spacing / 2.0f); // Center the grid
                position.y = (float)y * spacing - (gridSize * spacing / 2.0f) + (spacing / 2.0f); // Center the grid
                position.z = (float)z * spacing - (gridSize * spacing / 2.0f) + (spacing / 2.0f); // Center the grid
                cubePositions.push_back(position);
            }
        }
    }
    
    float fovDegrees = 45.0f; // Field of View in degrees
    // Get aspect ratio from the window object
    float aspectRatio = window.getAspectRatio();
    float nearPlane = 0.1f; // Distance to the near clipping plane
    float farPlane = 100.0f; // Distance to the far clipping plane
    
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(fovDegrees), aspectRatio, nearPlane, farPlane);
    
    // Create an FPS limiter object
    FPSLimiter fpsLimiter(60); // Target 60 FPS
    
    /* Loop until the user closes the window */
    // Use the GLWindow method to check if the window should close
    while (!window.shouldClose()) {
        // Process key input
        processKeyInput(&window, &mainCamera, fpsLimiter.getDeltaTime());
        
        // Clear the color buffer using the GLWindow clear method
        window.clear(0.16f, 0.24f, 0.32f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Activate the shader program using the Shader class
        cubeShader.use();
        cubeShader.setInt("uTexture1", 0); // Tell the shader's uTexture1 uniform to use texture unit 0
        
        // Update matrix uniforms using the Shader class set methods
        // Get the View matrix from the Camera
        glm::mat4 viewMatrix = mainCamera.getViewMatrix();
        cubeShader.setMat4("uView", viewMatrix);
        cubeShader.setMat4("uProjection", projectionMatrix);
        
        // Active texture
        cubeTexture.bind(0);
        
        // Ask cube to draw
        for (unsigned int i = 0; i < cubePositions.size(); i++)
        {
            glm::mat4 modelMatrix = glm::mat4(1.0f); // Start with identity
            modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(.0f, .0f, -30.0f));
            cubeShader.setMat4("uModel", modelMatrix);
            
            cubeMesh.draw();
        }
        
        // Limit the frame rate using the FPSLimiter object
        fpsLimiter.limit();
        
        // Swap front and back buffers using the GLWindow method
        window.swapBuffers();
        
        // Poll for and process events using the GLWindow method
        window.pollEvents();
    }
    
    return 0;
}

