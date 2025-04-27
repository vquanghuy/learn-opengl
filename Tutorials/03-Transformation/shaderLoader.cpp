#include <iostream>
#include <fstream>
#include <stdexcept>

#include "shaderLoader.h"

std::string loadFile(const std::string& path) {
    std::ifstream file(path);

    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::string fileContents(
        (std::istreambuf_iterator<char>(file)), 
        std::istreambuf_iterator<char>()
    );

    return fileContents;
}

GLuint compileShader(std::string& shaderCode, GLenum shaderType) {
    // Create shader ID and compile
    GLuint shaderID = glCreateShader(shaderType);
    const GLchar* shaderCodePtr = shaderCode.c_str();

    glShaderSource(shaderID, 1, &shaderCodePtr, nullptr);
    glCompileShader(shaderID);

    // Check compilation status
    GLint result = GL_FALSE;
    int infoLogLength;

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0) {
        std::string errorMessage = "";
        errorMessage.resize(infoLogLength + 1);
        glGetShaderInfoLog(shaderID, infoLogLength, nullptr, &errorMessage[0]);
        glDeleteShader(shaderID);

        throw std::runtime_error("Shader compilation failed: " + errorMessage);
    }

    return shaderID;
}

GLuint linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID) {
    // Create program ID and link
    GLuint programID = glCreateProgram();

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check linking status
    GLint result = GL_FALSE;
    int infoLogLength;

    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0) {
        std::string errorMessage = "";
        errorMessage.resize(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, nullptr, &errorMessage[0]);
        glDeleteProgram(programID);

        throw std::runtime_error("Shader linking failed: " + errorMessage);
    }

    return programID;
}

GLuint loadShader(const char* vertexPath, const char* fragmentPath)
{
    try
    {
        std::string vertexCode = loadFile(vertexPath);
        std::string fragmentCode = loadFile(fragmentPath);

        GLuint vertexShaderID = compileShader(vertexCode, GL_VERTEX_SHADER);
        GLuint fragmentShaderID = compileShader(fragmentCode, GL_FRAGMENT_SHADER); 

        return linkProgram(vertexShaderID, fragmentShaderID);  
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR::SHADER::LOADER_EXCEPTION" << e.what() << std::endl;
    }

    return 0;
}