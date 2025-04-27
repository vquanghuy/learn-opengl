#include <iostream>
#include <chrono>
#include <thread>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "shaderLoader.h"
#include "textureLoader.h"

int main(void)
{
    GLFWwindow* window;

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
    window = glfwCreateWindow(640, 480, "02 - Texture Mapping", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    /* Initialize OpenGL context */
    int version = gladLoadGL((GLADloadfunc) glfwGetProcAddress);
    if (!version)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    std::cout << "Loaded OpenGL version: "
        << GLAD_VERSION_MAJOR(version) << "."
        << GLAD_VERSION_MINOR(version)
        << std::endl;

    // load shaders
    GLuint shaderProgram = loadShader(
        "./Assets/02-Texture/rectangle.vert.glsl",
        "./Assets/02-Texture/rectangle.frag.glsl"
    );

    // Load texture
    GLuint texId = loadTexture("./Assets/02-Texture/crate1.jpg");

    // setup vertices data (and buffer(s)) and configure vertex attributes
    GLfloat vertices[] = {
        // positions        // texture coords
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f    // top left 
    };
    GLuint indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    // tex coords attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // as we only have a single shader, we could also just activate our shader once beforehand if we want to 
    glUseProgram(shaderProgram);
    
    // loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // clear color
        glClearColor(0.16f, 0.24f, 0.32f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle with texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texId);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glDeleteProgram(shaderProgram);
    
    // Destroy the window
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
