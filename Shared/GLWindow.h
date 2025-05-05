#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <glad/gl.h> // Include glad to get all the required OpenGL headers
#include <GLFW/glfw3.h> // Include GLFW

#include <string>
#include <iostream> // For error reporting

class GLWindow
{
public:
    // Default Constructor: Initializes member variables but does NOT initialize GLFW, create the window, or load GLAD.
    GLWindow();

    // Destructor: Destroys the window and terminates GLFW (conditionally).
    ~GLWindow();

    // Prevent copying (windows are not copyable)
    GLWindow(const GLWindow&) = delete;
    GLWindow& operator=(const GLWindow&) = delete;

    // Optional: Allow moving (transfer ownership)
    GLWindow(GLWindow&& other) noexcept;
    GLWindow& operator=(GLWindow&& other) noexcept;

    // Method to initialize GLFW, create the window, make context current, and load GLAD.
    // This must be called AFTER creating the GLWindow object.
    // Returns true on success, false on failure.
    // Errors will be printed to cerr.
    bool create(int width, int height, const std::string& title, int glMajorVersion, int glMinorVersion);

    // Check if the window should close.
    bool shouldClose() const;
    
    // Request the window to close (sets the close flag).
    void requestClose() const;

    // Swap the front and back buffers.
    void swapBuffers();

    // Poll for and process events.
    void pollEvents();

    // Check if the window was created successfully and GLAD loaded.
    bool isValid() const { return window != nullptr && gladLoaded; }

    // Get the underlying GLFWwindow pointer (use with caution).
    GLFWwindow* getGLFWwindow() const { return window; }

    // Get window dimensions
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    float getAspectRatio() const { return static_cast<float>(width) / height; }

    // Clear the color and/or depth buffers
    void clear(float r, float g, float b, float a, GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --- Static helper for the debugger sleep workaround ---
    // Call this at the very beginning of your main function, before creating any GLWindow objects.
    static void debuggerSleepWorkaround(int seconds = 1);
    
    // Get the last reported state of a keyboard key.
    int getKey(int key) const;

private:
    GLFWwindow* window = nullptr; // The GLFW window pointer
    int width = 0;                // Initialize to 0
    int height = 0;               // Initialize to 0
    std::string title;
    bool gladLoaded = false;

    // Utility function for reporting errors
    void logError(const std::string& message) const;
};

#endif // GLWINDOW_H

