#include <glad/gl.h>

/**
 * File loader function
 * @param filePath Path to the file to load
 * @return Pointer to the file contents
 */
std::string loadFile(const std::string& filePath);

/**
 * Compile shader function
 * @param shaderCode Shader source code
 * @param shaderType Shader type, GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
 * @return compiled shader ID
 */
GLuint compileShader(const std::string& shaderCode, GLenum shaderType);

/**
 * Link shader program function
 * @param vertexShaderID Vertex shader ID
 * @param fragmentShaderID Fragment shader ID
 * @return Shader program ID
 */
GLuint linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID);

/**
 * Shader loader function
 * @param vertexPath Path to the vertex shader file
 * @param fragmentPath Path to the fragment shader file
 * @return Shader program ID
*/
GLuint loadShader(const char* vertexPath, const char* fragmentPath);