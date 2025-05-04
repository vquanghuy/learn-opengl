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

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

// Framebuffer size callback (can be a lambda in main or a separate function)
// This will be called by GLFW when the window is resized.
// The GLWindow::framebufferSizeCallback handles updating the viewport.
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    // You might add other resize-related logic here if needed,
    // but glViewport is already handled by the static callback in GLWindow.cpp
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

    // Set the framebuffer size callback using the GLWindow method
    window.setFramebufferSizeCallback(framebufferSizeCallback);

    // Load shaders using your Shader class
    Shader cubeShader("./Assets/04-Camera/rectangle.vert.glsl", "./Assets/04-Camera/rectangle.frag.glsl");
    if (!cubeShader.load()) {
        // Handle shader loading error (message already printed by Shader::load)
        return -1; // Exit application if shader loading failed
    }

    // Load texture
    Texture cubeTexture = Texture("./Assets/04-Camera/crate1.jpg");
    if (!cubeTexture.load()) {
        return -1; // Exit application if texture loading failed
    }

    // setup vertices data (and buffer(s)) and configure vertex attributes (kept from original)
    GLfloat vertices[] = {
        // positions        // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f // top left
    };
    GLuint indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute (using layout (location = 0))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) 0);
    glEnableVertexAttribArray(0);

    // tex coords attribute (using layout (location = 1))
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // unbind the VAO
    glBindVertexArray(0);

    // initialize matrices (kept from original)
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(.0f, .0f, -2.5f));

    glm::mat4 viewMatrix = glm::mat4(1.0f);

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
        // Clear the color buffer using the GLWindow clear method
        window.clear(0.16f, 0.24f, 0.32f, 1.0f, GL_COLOR_BUFFER_BIT); // Or GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT

        // calculate new rotation (kept from original)
        modelMatrix = glm::rotate(modelMatrix, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        
        // Activate the shader program using the Shader class
        cubeShader.use();

        // update matrix uniforms using the Shader class set methods
        cubeShader.setMat4("uModel", modelMatrix);
        cubeShader.setMat4("uView", viewMatrix);
        cubeShader.setMat4("uProjection", projectionMatrix);

        // Active texture
        cubeTexture.bind(0);

        // bind VAO
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, nullptr);

        // Limit the frame rate using the FPSLimiter object
        fpsLimiter.limit();

        // Swap front and back buffers using the GLWindow method
        window.swapBuffers();

        // Poll for and process events using the GLWindow method
        window.pollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose: (kept from original)
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}

