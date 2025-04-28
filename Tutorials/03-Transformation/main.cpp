#include <iostream>
#include <thread>
#include <chrono>
#include <thread>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/gtc/type_ptr.hpp>

#include "shaderLoader.h"
#include "textureLoader.h"

#define WINDOW_WIDTH 640.0f
#define WINDOW_HEIGHT 480.0f

void limitFPS(int targetFPS) {
    static auto lastTime = std::chrono::high_resolution_clock::now();
    const double targetFrameTime = 1.0 / targetFPS;

    const auto currentTime = std::chrono::high_resolution_clock::now();
    const double elapsedTime = std::chrono::duration<double>(currentTime - lastTime).count();

    if (elapsedTime < targetFrameTime) {
        std::this_thread::sleep_for(std::chrono::duration<double>(targetFrameTime - elapsedTime));
    }

    lastTime = std::chrono::high_resolution_clock::now();
}

int main(void) {
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // Sleep for 1 second as waiting debugger to start
    std::this_thread::sleep_for(std::chrono::seconds(1));
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "03 - Transformation", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize OpenGL context */
    int version = gladLoadGL((GLADloadfunc) glfwGetProcAddress);
    if (!version) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "Loaded OpenGL version: "
            << GLAD_VERSION_MAJOR(version) << "."
            << GLAD_VERSION_MINOR(version)
            << std::endl;

    // load shaders
    GLuint shaderProgram = loadShader(
        "./Assets/03-Transformation/rectangle.vert.glsl",
        "./Assets/03-Transformation/rectangle.frag.glsl"
    );

    // Load texture
    GLuint texId = loadTexture("./Assets/03-Transformation/crate1.jpg");

    // setup vertices data (and buffer(s)) and configure vertex attributes
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

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) 0);
    glEnableVertexAttribArray(0);

    // tex coords attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // unbind the VAO
    glBindVertexArray(0);

    // as we only have a single shader, we could also just activate our shader once beforehand if we want to 
    glUseProgram(shaderProgram);

    // initialize matrices
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(.0f, .0f, -2.5f));
    
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    
    float fovDegrees = 45.0f; // Field of View in degrees
    float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT; // Aspect ratio of window
    float nearPlane = 0.1f; // Distance to the near clipping plane
    float farPlane = 100.0f; // Distance to the far clipping plane

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(fovDegrees), aspectRatio, nearPlane, farPlane);

    // uniform location
    GLuint uModelLoc = glGetUniformLocation(shaderProgram, "uModel");
    GLuint uViewLoc = glGetUniformLocation(shaderProgram, "uView");
    GLuint uProjectionLoc = glGetUniformLocation(shaderProgram, "uProjection");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        // Render here
        glClearColor(0.16f, 0.24f, 0.32f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // calculate new rotation
        modelMatrix = glm::rotate(modelMatrix, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        // update matrix
        glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(uProjectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        // render the triangle with texture
        // enable texture unit
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texId);

        // bind VAO
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, nullptr);

        // limit FPS
        limitFPS(60);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
