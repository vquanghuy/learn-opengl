#include "Shader.h"

// Include necessary headers for file operations and error handling
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> // Needed for checkCompileErrors infoLog
#include <cassert> // For assert (optional)

// Constructor implementation: Simply stores the file paths.
Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    : ID(0), vertexFilePath(vertexPath), fragmentFilePath(fragmentPath)
{
    // No OpenGL calls here. Shaders are loaded and compiled in the load() method.
    // The ID is initialized to 0, indicating no valid program is loaded yet.
}

// Destructor implementation: Cleans up the OpenGL shader program if it was created.
Shader::~Shader()
{
    if (ID != 0) // Only delete if a valid program was created (ID is not 0)
    {
        glDeleteProgram(ID);
    }
}

// Move constructor: Transfers ownership of the OpenGL program ID and file paths.
Shader::Shader(Shader&& other) noexcept
    : ID(other.ID), vertexFilePath(std::move(other.vertexFilePath)), fragmentFilePath(std::move(other.fragmentFilePath))
{
    // Set the other object's ID to 0 so its destructor doesn't delete the transferred program.
    other.ID = 0;
}

// Move assignment operator: Transfers ownership and cleans up the current program if it exists.
Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other) // Prevent self-assignment
    {
        if (ID != 0) // Delete the current program if this object holds one
        {
            glDeleteProgram(ID);
        }

        // Transfer ownership of the program ID and file paths
        ID = other.ID;
        vertexFilePath = std::move(other.vertexFilePath);
        fragmentFilePath = std::move(other.fragmentFilePath);

        // Set the other object's ID to 0
        other.ID = 0;
    }
    return *this; // Return reference to this object
}


// Helper function to read shader file source code into a string.
// Returns true on success, false on failure (prints error to cerr).
bool Shader::readShaderFile(const std::string& filePath, std::string& outCode)
{
    std::ifstream shaderFile;
    // Ensure ifstream objects can throw exceptions on failure
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // Open the file
        shaderFile.open(filePath);
        // Use stringstream to efficiently read the entire file content
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        // Close the file handler
        shaderFile.close();
        // Convert the stream buffer into a string
        outCode = shaderStream.str();
        return true; // File read successfully
    }
    catch (std::ifstream::failure& e)
    {
        // Report error if file reading failed
        logError("SHADER::FILE_NOT_SUCCESFULLY_READ: " + filePath + " - " + e.what());
        return false; // Indicate failure
    }
}

// Internal logging function for standardized error output
void Shader::logError(const std::string& message) const
{
    std::cerr << "ERROR::" << message << std::endl;
}


// Utility function to check shader compilation or program linking errors.
// Reports errors using the internal logging function and returns true if no errors were found, false otherwise.
bool Shader::checkCompileErrors(GLuint shader, ShaderType type)
{
    GLint success;
    GLchar infoLog[1024]; // Buffer to store error messages
    std::string typeString;

    if (type == ShaderType::VERTEX)
        typeString = "VERTEX";
    else if (type == ShaderType::FRAGMENT)
        typeString = "FRAGMENT";
    else if (type == ShaderType::PROGRAM)
        typeString = "PROGRAM";
    else
        typeString = "UNKNOWN"; // Should not happen with enum class

    if (type != ShaderType::PROGRAM) // Checking Shader (Vertex or Fragment) compilation errors
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            logError("SHADER_COMPILATION_ERROR of type: " + typeString + "\n" + infoLog + "\n -- --------------------------------------------------- -- ");
        }
    }
    else // Checking Shader Program linking errors
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            // Note: Use the program ID (this->ID) for getting the program info log
            glGetProgramInfoLog(this->ID, 1024, NULL, infoLog);
            logError("PROGRAM_LINKING_ERROR of type: " + typeString + "\n" + infoLog + "\n -- --------------------------------------------------- -- ");
        }
    }
    return success; // Return the success status (true if no errors, false if errors)
}


// Method to load, compile, and link the shader program.
// This method performs the actual OpenGL API calls.
// Returns true on success, false on failure.
bool Shader::load()
{
    // Clean up any existing program if load() is called multiple times on the same object
    if (ID != 0)
    {
        glDeleteProgram(ID);
        ID = 0; // Reset ID to 0 before attempting to load a new program
    }

    // 1. Retrieve the vertex/fragment source code from stored file paths
    std::string vertexCode;
    std::string fragmentCode;

    // Read vertex shader file
    if (!readShaderFile(vertexFilePath, vertexCode))
    {
        // Error already reported by readShaderFile
        return false; // Indicate failure
    }
    // Read fragment shader file
    if (!readShaderFile(fragmentFilePath, fragmentCode))
    {
        // Error already reported by readShaderFile
        return false; // Indicate failure
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. Compile shaders
    GLuint vertex, fragment;

    // Vertex shader compilation
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // Check for compilation errors using the utility function
    if (!checkCompileErrors(vertex, ShaderType::VERTEX)) // Use enum class
    {
        glDeleteShader(vertex); // Clean up the shader object on error
        return false; // Indicate failure
    }

    // Fragment shader compilation
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // Check for compilation errors using the utility function
    if (!checkCompileErrors(fragment, ShaderType::FRAGMENT)) // Use enum class
    {
        glDeleteShader(vertex); // Clean up the vertex shader (if successful)
        glDeleteShader(fragment); // Clean up the fragment shader on error
        return false; // Indicate failure
    }

    // 3. Create and Link Shader Program
    ID = glCreateProgram(); // Create the shader program
    glAttachShader(ID, vertex); // Attach the compiled vertex shader
    glAttachShader(ID, fragment); // Attach the compiled fragment shader
    glLinkProgram(ID); // Link the program

    // Check for linking errors using the utility function
    if (!checkCompileErrors(ID, ShaderType::PROGRAM)) // Use enum class
    {
        glDeleteProgram(ID); // Clean up the program object on error
        glDeleteShader(vertex); // Clean up the attached shader objects
        glDeleteShader(fragment);
        ID = 0; // Reset ID to 0 to indicate an invalid program
        return false; // Indicate failure
    }

    // Delete the shader objects as they are now linked into the program and no longer needed
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return true; // Shader program loaded and linked successfully
}


// Activate the shader program for rendering.
// Only safe to call if isValid() is true (ID != 0).
void Shader::use() const
{
    if (ID != 0) // Check if the shader program is valid
    {
        glUseProgram(ID);
    }
    else
    {
        logError("Attempted to use an invalid shader program (ID is 0).");
    }
}

// Helper function to print a warning when attempting to set a uniform on an invalid shader.
void Shader::warnInvalidUniformSet(const std::string& name) const
{
    logError("Attempted to set uniform '" + name + "' on an invalid shader program.");
}


// Utility uniform functions implementations.
// These functions set the value of uniform variables in the currently active shader program.
// They are only safe to call if the shader program is valid (ID != 0) and currently in use (glUseProgram(ID)).
// A basic check for ID != 0 is included for safety.

void Shader::setBool(const std::string& name, bool value) const { if(ID) glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); else { warnInvalidUniformSet(name); } }
void Shader::setInt(const std::string& name, int value) const { if(ID) glUniform1i(glGetUniformLocation(ID, name.c_str()), value); else { warnInvalidUniformSet(name); } }
void Shader::setFloat(const std::string& name, float value) const { if(ID) glUniform1f(glGetUniformLocation(ID, name.c_str()), value); else { warnInvalidUniformSet(name); } }
void Shader::setVec2(const std::string& name, const glm::vec2& value) const { if(ID) glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); else { warnInvalidUniformSet(name); } }
void Shader::setVec2(const std::string& name, float x, float y) const { if(ID) glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); else { warnInvalidUniformSet(name); } }
void Shader::setVec3(const std::string& name, const glm::vec3& value) const { if(ID) glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); else { warnInvalidUniformSet(name); } }
void Shader::setVec3(const std::string& name, float x, float y, float z) const { if(ID) glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); else { warnInvalidUniformSet(name); } }
void Shader::setVec4(const std::string& name, const glm::vec4& value) const { if(ID) glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); else { warnInvalidUniformSet(name); } }
void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const { if(ID) glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); else { warnInvalidUniformSet(name); } }
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const { if(ID) glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat)); else { warnInvalidUniformSet(name); } }
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const { if(ID) glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat)); else { warnInvalidUniformSet(name); } }
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const { if(ID) glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat)); else { warnInvalidUniformSet(name); } }


