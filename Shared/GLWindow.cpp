#include "GLWindow.h"
#include <thread> // For debugger sleep workaround
#include <chrono> // For debugger sleep workaround

// Default Constructor: Initializes member variables.
GLWindow::GLWindow()
    : window(nullptr), width(0), height(0), gladLoaded(false)
{
}

// Destructor implementation: Destroys the window and terminates GLFW (conditionally).
GLWindow::~GLWindow()
{
    if (window != nullptr) // Only destroy if a valid window was created
    {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

// Move constructor
GLWindow::GLWindow(GLWindow&& other) noexcept
    : window(other.window), width(other.width), height(other.height),
      title(std::move(other.title)), gladLoaded(other.gladLoaded)
{
    other.window = nullptr; // Set other's window pointer to null to prevent double destruction
    // Update the user pointer in the moved window
    if (window) {
        glfwSetWindowUserPointer(window, this);
    }
}

// Move assignment operator
GLWindow& GLWindow::operator=(GLWindow&& other) noexcept
{
    if (this != &other)
    {
        if (window != nullptr) // Destroy the current window if this object holds one
        {
            glfwDestroyWindow(window);
        }

        // Transfer ownership
        window = other.window;
        width = other.width;
        height = other.height;
        title = std::move(other.title);
        gladLoaded = other.gladLoaded;

        // Update the user pointer in the moved window
        if (window) {
             glfwSetWindowUserPointer(window, this);
        }

        // Set other's window pointer to null
        other.window = nullptr;
    }
    return *this;
}

// Method to initialize GLFW, create the window, make context current, and load GLAD.
bool GLWindow::create(int width, int height, const std::string& title, int glMajorVersion, int glMinorVersion)
{
    // Store dimensions and title
    this->width = width;
    this->height = height;
    this->title = title;

    // 1. Initialize GLFW (only if it hasn't been initialized already)
    // This is a common place to initialize GLFW for a single window application.
    if (!glfwInit())
    {
        logError("Failed to initialize GLFW");
        return false; // Indicate failure
    }

    // Configure GLFW (e.g., OpenGL version, core profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on macOS
#endif

    // 2. Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(this->width, this->height, this->title.c_str(), NULL, NULL);
    if (!window)
    {
        logError("Failed to create GLFW window");
        glfwTerminate(); // Terminate GLFW if window creation fails
        return false; // Indicate failure
    }

    // Make the window's context current on the calling thread
    glfwMakeContextCurrent(window);

    // Set the user pointer to this instance for callbacks
    glfwSetWindowUserPointer(window, this);

    // Set the static framebuffer size callback
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);


    // 3. Initialize GLAD (load OpenGL function pointers)
    // This must be done AFTER the OpenGL context has been made current.
    int version = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
    if (!version)
    {
        logError("Failed to initialize GLAD");
        // Clean up resources created before GLAD failed
        glfwDestroyWindow(window);
        glfwTerminate();
        window = nullptr; // Reset window pointer
        gladLoaded = false;
        return false; // Indicate failure
    }

    std::cout << "Loaded OpenGL version: "
              << GLAD_VERSION_MAJOR(version) << "."
              << GLAD_VERSION_MINOR(version)
              << std::endl;

    gladLoaded = true; // Mark GLAD as successfully loaded

    return true; // Indicate success
}


// Check if the window should close.
bool GLWindow::shouldClose() const
{
    if (window != nullptr)
    {
        return glfwWindowShouldClose(window);
    }
    return true; // If window is invalid, it should be considered closed
}

// Swap the front and back buffers.
void GLWindow::swapBuffers()
{
    if (window != nullptr)
    {
        glfwSwapBuffers(window);
    }
}

// Poll for and process events.
void GLWindow::pollEvents()
{
    glfwPollEvents();
}

// Set the framebuffer size callback
void GLWindow::setFramebufferSizeCallback(GLFWframebuffersizefun callback)
{
    if (window != nullptr)
    {
        glfwSetFramebufferSizeCallback(window, callback);
    }
}

// Clear the color and/or depth buffers
void GLWindow::clear(float r, float g, float b, float a, GLbitfield mask)
{
    if (window != nullptr) // Only clear if window is valid
    {
        glClearColor(r, g, b, a); // Set the clear color
        glClear(mask);          // Clear the specified buffers
    }
}


// Utility function for reporting errors
void GLWindow::logError(const std::string& message) const
{
    std::cerr << "GLWindow ERROR: " << message << std::endl;
}

// Static framebuffer size callback implementation
void GLWindow::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Retrieve the GLWindow instance using the user pointer
    GLWindow* glWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    if (glWindow)
    {
        // Update the stored dimensions (optional, but good practice)
        glWindow->width = width;
        glWindow->height = height;
        // Call the OpenGL function to update the viewport
        glViewport(0, 0, width, height);
    }
}

// --- Static helper for the debugger sleep workaround ---
void GLWindow::debuggerSleepWorkaround(int seconds)
{
#ifdef __APPLE__
    std::cout << "Waiting " << seconds << " second(s) for debugger attachment/cleanup..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
    std::cout << "Resuming execution." << std::endl;
#endif
}

