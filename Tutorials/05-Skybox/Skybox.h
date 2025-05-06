#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "CubeTexture.h"

class Skybox
{
public:
    // Constructor: Initializes the geometry (VAO/VBO).
    // Does NOT load shader or texture in the constructor.
    Skybox();

    // Destructor
    ~Skybox();

    // Prevent copying (Skybox manages OpenGL resources)
    Skybox(const Skybox&) = delete;
    Skybox& operator=(const Skybox&) = delete;

    // Allow moving (transfer ownership of OpenGL resources)
    Skybox(Skybox&& other) noexcept;
    Skybox& operator=(Skybox&& other) noexcept;
    
    // Initializes the skybox geometry (VAO and VBO)
    bool setupMesh();

    // Assign the shader to this skybox
    void setShader(Shader* shader);

    // Assign the cubemap texture to this skybox
    void setCubeTexture(CubeTexture* texture);

    // Draws the skybox
    // Takes view and projection matrices as parameters
    void draw(const glm::mat4& view, const glm::mat4& projection);

    // Check if the necessary resources (shader and texture) are assigned and valid
    // Renamed from isReadyToDraw for consistency
    bool isValid() const;

private:
    GLuint vao = 0; // Initialize to 0 - Renamed from skyboxVAO
    GLuint vbo = 0; // Initialize to 0 - Renamed from skyboxVBO
    CubeTexture* cubeTexture = nullptr; // Pointer to the CubeTexture object (owned externally) - Renamed from cubemapTexture
    Shader* shader = nullptr;      // Pointer to the Shader object (owned externally) - Renamed from skyboxShader

    // Declaration of the static constant vertices for the skybox cube
    // The definition is in the .cpp file
    static const float skyboxVertices[108];
};

#endif

