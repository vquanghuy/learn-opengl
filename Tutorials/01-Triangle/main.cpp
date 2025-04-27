#include <iostream>
#include <chrono>
#include <thread>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "loader.h"

int main(int argc, const char * argv[])
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
    window = glfwCreateWindow(640, 480, "01 - Color Triangle", NULL, NULL);
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
        "./Assets/01-Triangle/triangle.vert.glsl",
        "./Assets/01-Triangle/triangle.frag.glsl"
    );

    // setup vertices data (and buffer(s)) and configure vertex attributes
    GLfloat vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top

    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // as we only have a single shader, we could also just activate our shader once beforehand if we want to
    glUseProgram(shaderProgram);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.16f, 0.24f, 0.32f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window); // Destroy the window
    glfwTerminate();
    return 0;
}

