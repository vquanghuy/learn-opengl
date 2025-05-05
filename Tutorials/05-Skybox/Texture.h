#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/gl.h>
#include <string>
#include <iostream>

class Texture
{
public:
    // Constructor: Stores the file path but does NOT load the image or create the OpenGL texture.
    Texture(const std::string& filePath);

    // Destructor: Deletes the OpenGL texture object.
    ~Texture();

    // Prevent copying (textures are not copyable)
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Optional: Allow moving (transfer ownership)
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    // Method to load the image, create the OpenGL texture, and configure it.
    // This must be called AFTER creating the Texture object and
    // AFTER a valid OpenGL context has been made current.
    // Returns true on success, false on failure.
    // Errors will be printed to cerr.
    bool load();

    // Bind the texture to a specific texture unit.
    // Only safe to call if isValid() is true.
    void bind(GLuint textureUnit = 0) const; // Default to texture unit 0
    
    // Unbinds the texture from a specific texture unit
    void unbind(unsigned int textureUnit = 0) const;

    // Check if the texture was loaded and created successfully.
    bool isValid() const { return ID != 0; }

    // Get the OpenGL texture ID.
    GLuint getID() const { return ID; }

private:
    GLuint ID = 0; // The OpenGL texture ID (0 indicates invalid/not loaded)
    std::string filePath; // Stored file path to the image

    int width = 0;  // Image width
    int height = 0; // Image height
    int nrChannels = 0; // Number of color channels

    // Utility function for reporting errors
    void logError(const std::string& message) const;
};

#endif // TEXTURE_H

