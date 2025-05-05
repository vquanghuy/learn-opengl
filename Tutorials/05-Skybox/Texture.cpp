#include "Texture.h"

// Tell stb_image to implement the functions
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Constructor implementation: Simply stores the file path.
Texture::Texture(const std::string& filePath)
    : ID(0), filePath(filePath)
{
    // No OpenGL or image loading calls here.
    // Image loading and texture creation happen in the load() method.
}

// Destructor implementation: Deletes the OpenGL texture object if it was created.
Texture::~Texture()
{
    if (ID != 0) // Only delete if a valid texture was created (ID is not 0)
    {
        glDeleteTextures(1, &ID);
    }
}

// Move constructor
Texture::Texture(Texture&& other) noexcept
    : ID(other.ID), filePath(std::move(other.filePath)),
      width(other.width), height(other.height), nrChannels(other.nrChannels)
{
    other.ID = 0; // Set other's ID to 0 to prevent double deletion
    other.width = 0;
    other.height = 0;
    other.nrChannels = 0;
}

// Move assignment operator
Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other) // Prevent self-assignment
    {
        if (ID != 0) // Delete the current texture if this object holds one
        {
            glDeleteTextures(1, &ID);
        }

        // Transfer ownership
        ID = other.ID;
        filePath = std::move(other.filePath);
        width = other.width;
        height = other.height;
        nrChannels = other.nrChannels;

        // Set other's state to default
        other.ID = 0;
        other.width = 0;
        other.height = 0;
        other.nrChannels = 0;
    }
    return *this;
}

// Method to load the image, create the OpenGL texture, and configure it.
bool Texture::load()
{
    // Clean up any existing texture if load() is called multiple times
    if (ID != 0)
    {
        glDeleteTextures(1, &ID);
        ID = 0; // Reset ID
    }

    // 1. Load image data using stb_image.h
    // Flip texture vertically because OpenGL expects the first pixel to be at the bottom-left
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    if (!data)
    {
        logError("Failed to load texture image: " + filePath);
        return false; // Indicate failure
    }

    // Determine the image format based on the number of channels
    GLenum format;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    else
    {
        logError("Unsupported number of texture channels: " + std::to_string(nrChannels) + " for " + filePath);
        stbi_image_free(data); // Free image data
        return false; // Indicate failure
    }

    // 2. Create OpenGL texture
    glGenTextures(1, &ID); // Generate a texture ID
    glBindTexture(GL_TEXTURE_2D, ID); // Bind the texture

    // 3. Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping for S axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Set texture wrapping for T axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Set minification filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Set magnification filter

    // 4. Upload image data to the texture
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    // 5. Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // 6. Free image data after uploading to the GPU
    stbi_image_free(data);

    // Unbind the texture (optional, but good practice)
    glBindTexture(GL_TEXTURE_2D, 0);

    return true; // Indicate success
}

// Bind the texture to a specific texture unit.
void Texture::bind(GLuint textureUnit) const
{
    if (ID != 0) // Only bind if the texture is valid
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit); // Activate the specified texture unit
        glBindTexture(GL_TEXTURE_2D, ID);             // Bind this texture to the active unit
    }
    else
    {
        // Optional: Add a warning if trying to bind an invalid texture
        logError("Attempted to bind an invalid texture.");
    }
}

// Utility function for reporting errors
void Texture::logError(const std::string& message) const
{
    std::cerr << "Texture ERROR: " << message << std::endl;
}

