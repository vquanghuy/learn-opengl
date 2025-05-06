#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <iostream> // For potential logging

class AssetManager
{
public:
    // Set the global base directory for all assets.
    // This should typically be called once at the beginning of the application.
    static void setBaseDirectory(const std::string& dir);
    
    // Get the global base directory.
    static const std::string& getBaseDirectory();
    
    // Get the directory for shaders (derived from base directory).
    static std::string getShaderDirectory();
    
    // Get the directory for textures (derived from base directory).
    static std::string getTextureDirectory();
    
    // Get the full path for a shader file.
    // Combines base directory, shader directory, and filename.
    static std::string getShaderPath(const std::string& filename);
    
    // Get the full path for a texture file.
    // Combines base directory, texture directory, and filename.
    static std::string getTexturePath(const std::string& filename);
    
private:
    // Static member to store the global base directory.
    static std::string baseDirectory;
    
    // Private constructor to prevent instantiation (it's a static utility class)
    AssetManager() = delete;
};

// Define macros to get the full paths for assets
#define SHADER_PATH(filename) AssetManager::getShaderPath(filename)
#define TEXTURE_PATH(filename) AssetManager::getTexturePath(filename)

#endif // ASSETMANAGER_H

