#include <stb_image.h>

#include "CubeTexture.h"

// Constructor: Stores the file paths
CubeTexture::CubeTexture(const std::vector<std::string>& faces) : faces(faces) // Initialize faces vector
{
    // ID is initialized to 0 by default
}

// Destructor: Deletes the OpenGL texture
CubeTexture::~CubeTexture()
{
    if (ID != 0) { // Only delete if a valid texture was created
        glDeleteTextures(1, &ID);
    }
}

// Move constructor
CubeTexture::CubeTexture(CubeTexture&& other) noexcept
: ID(other.ID), faces(std::move(other.faces)) // Move ID and faces
{
    other.ID = 0; // Set other's ID to 0 to prevent double deletion
}

// Move assignment operator
CubeTexture& CubeTexture::operator=(CubeTexture&& other) noexcept
{
    if (this != &other) {
        if (ID != 0) { // Delete our own texture if it exists
            glDeleteTextures(1, &ID);
        }
        
        ID = other.ID; // Transfer ownership of ID
        faces = std::move(other.faces); // Move faces vector
        
        other.ID = 0; // Set other's ID to 0
    }
    return *this;
}


// Method to load the cubemap texture
bool CubeTexture::load()
{
    if (ID != 0) {
        logError("Attempted to load an already loaded cubemap texture.");
        return false; // Already loaded
    }
    
    if (faces.size() != 6) {
        logError("Cubemap requires exactly 6 faces, but " + std::to_string(faces.size()) + " were provided.");
        return false;
    }
    
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    
    int width, height, nrChannels;
    // Set stb_image to flip loaded textures vertically (important for some image formats)
    stbi_set_flip_vertically_on_load(false); // Cubemaps should NOT be flipped vertically
    
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        std::string facePath = faces[i];
        unsigned char *data = stbi_load(facePath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format = GL_RGB;
            if (nrChannels == 4)
                format = GL_RGBA;
            
            // Load image data into the correct cubemap face
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            logError("Cubemap texture failed to load at path: " + faces[i]);
            stbi_image_free(data); // Still free data even if loading failed
            // Clean up the partially created texture to avoid issues
            glDeleteTextures(1, &ID);
            ID = 0; // Mark as invalid
            return false; // Loading failed
        }
    }
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    // Unbind texture after configuration
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    // Loading successful
    return true;
}

// Bind the cubemap texture
void CubeTexture::bind(GLuint textureUnit) const
{
    if (ID != 0) { // Only bind if valid
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    } else {
        logError("Attempted to bind an invalid cubemap texture.");
    }
}

// Unbind the cubemap texture
void CubeTexture::unbind(unsigned int textureUnit) const
{
    // Activate the specified texture unit
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    // Unbind the texture from the currently active texture unit
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

// Utility function for reporting errors
void CubeTexture::logError(const std::string& message) const
{
    std::cerr << "ERROR::CUBETEXTURE::" << message << std::endl;
}

