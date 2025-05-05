#ifndef MESH_H
#define MESH_H

#include <glad/gl.h> // Include glad
#include <vector>    // For storing vertices and indices
#include <string>    // For error messages
#include <iostream>  // For error reporting
#include <glm/glm.hpp> // For glm::vec3, glm::vec2 etc. (if not included by glad/gl.h)


// Define a simple Vertex structure to hold common vertex attributes
// This makes it easier to pass vertex data around.
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh
{
public:
    // Constructor: Stores the vertex and index data.
    // Does NOT generate OpenGL buffers or VAO here.
    // If indices is empty, the mesh will be drawn using glDrawArrays.
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices = std::vector<unsigned int>()); // Default empty indices

    // Destructor: Deletes the OpenGL buffers and VAO.
    ~Mesh();

    // Prevent copying (meshes are not copyable)
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // Optional: Allow moving (transfer ownership of OpenGL IDs)
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    // Method to setup OpenGL buffers and VAO.
    // This must be called AFTER creating the Mesh object and
    // AFTER a valid OpenGL context has been made current.
    // Returns true on success, false on failure.
    // Errors will be printed to cerr.
    bool setupMesh();

    // Method to draw the mesh.
    // Draws using glDrawElements if indices are available, otherwise uses glDrawArrays.
    // Requires the appropriate shader to be used beforehand.
    // Only safe to call if isValid() is true.
    void draw() const;

    // Check if the mesh was set up successfully (VAO is valid).
    bool isValid() const { return VAO != 0; }

    // Get the VAO ID (use with caution).
    GLuint getVAO() const { return VAO; }


private:
    // Mesh Data (stored in the object)
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    // std::vector<TextureInfo> textures; // If handling textures

    // OpenGL Render Data (generated in setupMesh)
    GLuint VAO = 0; // Vertex Array Object
    GLuint VBO = 0; // Vertex Buffer Object
    GLuint EBO = 0; // Element Buffer Object (Index Buffer) - 0 if not used

    // Utility function for reporting errors
    void logError(const std::string& message) const;

    // Helper function to setup the buffer objects and vertex attributes
    void setupBuffers();
};

#endif // MESH_H

