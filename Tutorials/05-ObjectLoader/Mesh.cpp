#include "Mesh.h"
#include <glm/glm.hpp> // For glm::vec3, glm::vec2 etc. (if not included by glad/gl.h)
#include <cstddef> // For offsetof


// Constructor implementation: Stores the vertex and index data.
// Default empty indices vector allows for glDrawArrays.
Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : vertices(vertices), indices(indices) // Copy the provided data
{
    // OpenGL buffers and VAO are generated in the setupMesh() method.
    // VAO, VBO, EBO are initialized to 0 by default.
}

// Destructor implementation: Deletes the OpenGL buffers and VAO.
Mesh::~Mesh()
{
    // Only delete if the mesh was set up successfully (VAO is not 0)
    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        if (EBO != 0) { // Only delete EBO if it was generated
             glDeleteBuffers(1, &EBO);
        }
    }
}

// Move constructor
Mesh::Mesh(Mesh&& other) noexcept
    : vertices(std::move(other.vertices)), indices(std::move(other.indices)),
      VAO(other.VAO), VBO(other.VBO), EBO(other.EBO)
{
    // Set other's IDs to 0 to prevent double deletion
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
}

// Move assignment operator
Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other) // Prevent self-assignment
    {
        // Delete the current OpenGL objects if this object holds any
        if (VAO != 0)
        {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            if (EBO != 0) { // Only delete EBO if it was generated
                 glDeleteBuffers(1, &EBO);
            }
        }

        // Transfer ownership of data and OpenGL IDs
        vertices = std::move(other.vertices);
        indices = std::move(other.indices);
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;

        // Set other's IDs to 0
        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
    }
    return *this;
}

// Utility function for reporting errors
void Mesh::logError(const std::string& message) const
{
    std::cerr << "Mesh ERROR: " << message << std::endl;
}


// Helper function to setup the buffer objects and vertex attributes
void Mesh::setupBuffers()
{
    // Generate VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // If indices are provided, generate and bind the EBO
    if (!indices.empty()) {
        glGenBuffers(1, &EBO);
    }


    // Bind the Vertex Array Object first
    glBindVertexArray(VAO);

    // Bind and upload vertex data to VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Use sizeof(Vertex) and vertices.size() to calculate the buffer size
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // If indices are provided, bind and upload index data to EBO
    if (!indices.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    }


    // Configure Vertex Attributes

    // Position attribute (layout (location = 0))
    glEnableVertexAttribArray(0);
    // Use offsetof to calculate the offset of the position member within the Vertex struct
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Normal attribute (layout (location = 1))
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Texture Coordinates attribute (layout (location = 2))
    glEnableVertexAttribArray(2); // Assuming texture coords are at location 2 in your shader
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    // Unbind the VAO (important!)
    glBindVertexArray(0);

    // Note: VBO is unbound when the VAO is unbound.
    // EBO remains bound to the VAO if it was generated.
}


// Method to setup OpenGL buffers and VAO.
bool Mesh::setupMesh()
{
    // Ensure we have vertex data to setup
    if (vertices.empty())
    {
        logError("Attempted to setup mesh with empty vertex data.");
        return false;
    }

    // Clean up any existing OpenGL objects if setupMesh is called multiple times
    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        if (EBO != 0) { // Only delete EBO if it was generated
             glDeleteBuffers(1, &EBO);
        }
        VAO = VBO = EBO = 0; // Reset IDs
    }

    // Perform the OpenGL buffer and VAO setup
    setupBuffers();

    // Check if VAO was successfully created (a basic check)
    if (VAO == 0)
    {
        logError("Failed to generate VAO during mesh setup.");
        return false;
    }

    return true; // Setup successful
}


// Method to draw the mesh.
// Draws using glDrawElements if indices are available, otherwise uses glDrawArrays.
// Requires the appropriate shader to be used beforehand.
// Only safe to call if isValid() is true.
void Mesh::draw() const
{
    if (VAO != 0) // Only draw if the mesh is valid and was set up
    {
        // Bind the VAO before drawing
        glBindVertexArray(VAO);

        if (!indices.empty())
        {
            // Draw using indices (glDrawElements)
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        }
        else
        {
            // Draw using vertex array (glDrawArrays)
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        }


        // Unbind the VAO after drawing (optional, but good practice)
        glBindVertexArray(0);
    }
    else
    {
        // Optional: Add a warning if trying to draw an invalid mesh
        logError("Attempted to draw an invalid mesh.");
    }
}

