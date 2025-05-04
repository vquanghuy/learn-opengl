#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h> // Corrected GLAD include path
#include <glm/glm.hpp> // For GLM matrix and vector types
#include <glm/gtc/type_ptr.hpp> // For glm::value_ptr to pass matrices to OpenGL

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // Constructor stores the file paths but does NOT load or compile the shaders.
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    // Destructor to clean up the shader program
    ~Shader();

    // Prevent copying (shaders are not copyable)
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    // Optional: Allow moving (transfer ownership)
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    // Method to load, compile, and link the shader program.
    // This must be called AFTER creating the Shader object and
    // AFTER a valid OpenGL context has been made current.
    // Returns true on success, false on failure.
    bool load(); // Error logging is handled internally

    // Use/activate the shader
    // Only safe to call if isValid() is true
    void use() const;

    // Utility uniform functions
    // These methods allow setting uniform values from your C++ code
    // Only safe to call if isValid() is true
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;

    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;

    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;

    void setMat2(const std::string& name, const glm::mat2& mat) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    // Get the program ID
    // Check this after calling load() to see if it was successful.
    GLuint getID() const { return ID; }

    // Helper to check if the shader program was loaded successfully
    bool isValid() const { return ID != 0; }


private:
    enum class ShaderType {
        VERTEX,
        FRAGMENT,
        PROGRAM
    };

    // The program ID
    GLuint ID = 0; // Initialize to 0 (invalid program ID)

    // Stored file paths
    std::string vertexFilePath;
    std::string fragmentFilePath;

    // Utility function for checking shader compilation/linking errors.
    // Reports errors using the internal logging function and returns true on success, false on failure.
    bool checkCompileErrors(GLuint shader, ShaderType type); // Updated signature

    // Helper function to read shader file source
    bool readShaderFile(const std::string& filePath, std::string& outCode);

    // Internal logging function for standardized error output
    void logError(const std::string& message) const;

    // Helper function to print a warning when attempting to set a uniform on an invalid shader.
    void warnInvalidUniformSet(const std::string& name) const;
};

#endif

