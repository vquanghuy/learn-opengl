#include "Skybox.h"
#include <iostream>

// Definition of the static constant vertices for the skybox cube
const float Skybox::skyboxVertices[108] = {
    // positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};


// Constructor: Initializes geometry
Skybox::Skybox()
{
    setupMesh(); // Setup VAO and VBO on construction
}

// Destructor
Skybox::~Skybox()
{
    // Delete the OpenGL resources owned by this class
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
    }
    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
    }
}

// Move constructor
Skybox::Skybox(Skybox&& other) noexcept
: vao(other.vao), vbo(other.vbo),
cubeTexture(other.cubeTexture), shader(other.shader)
{
    // Transfer ownership by setting other's IDs and pointers to 0/nullptr
    other.vao = 0;
    other.vbo = 0;
    other.cubeTexture = nullptr;
    other.shader = nullptr;
}

// Move assignment operator
Skybox& Skybox::operator=(Skybox&& other) noexcept
{
    if (this != &other) {
        // Delete our own OpenGL resources if they exist
        if (vao != 0) glDeleteVertexArrays(1, &vao);
        if (vbo != 0) glDeleteBuffers(1, &vbo);
        // Note: We do NOT delete our old shader or cubeTexture pointers
        
        // Transfer ownership from other
        vao = other.vao;
        vbo = other.vbo;
        cubeTexture = other.cubeTexture;
        shader = other.shader;
        
        // Set other's IDs and pointers to 0/nullptr
        other.vao = 0;
        other.vbo = 0;
        other.cubeTexture = nullptr;
        other.shader = nullptr;
    }
    return *this;
}


// Assign the shader to this skybox
void Skybox::setShader(Shader* shader)
{
    if (!shader || !shader->isValid()) {
        std::cerr << "WARNING::SKYBOX::SETSHADER::INVALID_SHADER_POINTER" << std::endl;
        this->shader = nullptr; // Assign nullptr if invalid - Renamed
    } else {
        this->shader = shader;
        this->shader->use();
        this->shader->setInt("uCubeTexture", 0);
    }
}

// Assign the cubemap texture to this skybox
void Skybox::setCubeTexture(CubeTexture* texture)
{
    if (!texture || !texture->isValid()) {
        std::cerr << "WARNING::SKYBOX::SETCUBETEXTURE::INVALID_TEXTURE_POINTER" << std::endl;
        this->cubeTexture = nullptr; // Assign nullptr if invalid - Renamed
    } else {
        this->cubeTexture = texture;
    }
}

// Draws the skybox
void Skybox::draw(const glm::mat4& view, const glm::mat4& projection)
{
    // Ensure necessary resources are assigned and valid before drawing
    if (!isValid()) { // Updated call to isValid()
        // Error message already printed by isValid or set methods
        return;
    }
    
    // Draw skybox as last (or first with depth testing disabled)
    // Drawing last with GL_LEQUAL is common to ensure it's behind everything
    glDepthFunc(GL_LEQUAL);
    
    shader->use();
    
    // We need the view matrix without the translation component for the skybox
    // Get the camera's view matrix, then remove the translation part
    glm::mat4 viewWithoutTranslation = glm::mat4(glm::mat3(view));
    
    shader->setMat4("uView", viewWithoutTranslation);
    shader->setMat4("uProjection", projection);
    
    // Bind the skybox VAO and texture
    glBindVertexArray(vao);
    cubeTexture->bind(0); // Bind to texture unit 0 (assuming uniform "skybox" is set to 0) - Renamed
    
    glDrawArrays(GL_TRIANGLES, 0, 36); // Draw the 36 vertices of the cube
    
    glBindVertexArray(0); // Unbind VAO
    cubeTexture->unbind(0); // Unbind texture from unit 0 - Renamed
    
    glDepthFunc(GL_LESS); // Set depth function back to default
}

// Check if the necessary resources (shader and texture) are assigned and valid
// Renamed from isReadyToDraw for consistency
bool Skybox::isValid() const
{
    return shader != nullptr && shader->isValid() &&
    cubeTexture != nullptr && cubeTexture->isValid() &&
    vao != 0 && vbo != 0; // Also check if geometry is set up - Renamed
}


// Initializes the skybox geometry (VAO and VBO)
bool Skybox::setupMesh()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0); // Unbind VAO
    
    // Check if VAO was successfully created (a basic check)
    if (vao == 0)
    {
        std::cerr << "Failed to generate VAO during mesh setup.";
        return false;
    }
    
    return true;
}

