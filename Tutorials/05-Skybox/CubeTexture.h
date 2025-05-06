#ifndef CUBETEXTURE_H
#define CUBETEXTURE_H

#include <vector>
#include <string>
#include <glad/gl.h>

#include <iostream>

class CubeTexture
{
public:
    // Constructor: Stores the file paths but does NOT load the images or create the OpenGL texture.
    CubeTexture(const std::vector<std::string>& faces);

    // Destructor: Deletes the OpenGL texture object.
    ~CubeTexture();

    // Prevent copying (textures are not copyable)
    CubeTexture(const CubeTexture&) = delete;
    CubeTexture& operator=(const CubeTexture&) = delete;

    // Allow moving (transfer ownership)
    CubeTexture(CubeTexture&& other) noexcept;
    CubeTexture& operator=(CubeTexture&& other) noexcept;

    // Method to load the images, create the OpenGL cubemap texture, and configure it.
    // This must be called AFTER creating the CubeTexture object and
    // AFTER a valid OpenGL context has been made current.
    // Returns true on success, false on failure.
    // Errors will be printed to cerr.
    bool load();

    // Bind the cubemap texture to a specific texture unit.
    // Only safe to call if isValid() is true.
    void bind(GLuint textureUnit = 0) const; // Default to texture unit 0

    // Unbinds the cubemap texture from a specific texture unit
    void unbind(unsigned int textureUnit = 0) const; // Unbind from GL_TEXTURE_CUBE_MAP target

    // Check if the texture was loaded and created successfully.
    bool isValid() const { return ID != 0; }

    // Get the OpenGL texture ID.
    GLuint getID() const { return ID; }

private:
    GLuint ID = 0; // The OpenGL texture ID (0 indicates invalid/not loaded)
    std::vector<std::string> faces; // Stored file paths to the cubemap faces

    // Optional: Store dimensions if needed, though cubemap faces should be square and same size
    // int width = 0;
    // int height = 0;

    // Utility function for reporting errors
    void logError(const std::string& message) const;

    // Helper function to load the cubemap data (internal to the load method)
    // unsigned int loadCubemap(std::vector<std::string> faces); // Removed as logic is in load()
};

#endif // CUBETEXTURE_H

