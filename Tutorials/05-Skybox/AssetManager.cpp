#include "AssetManager.h"

// Define and initialize the static base directory member
std::string AssetManager::baseDirectory = "";

// Set the global base directory
void AssetManager::setBaseDirectory(const std::string& dir)
{
    baseDirectory = dir;
    // Ensure the base directory ends with a slash for easier concatenation
    if (!baseDirectory.empty() && baseDirectory.back() != '/' && baseDirectory.back() != '\\') {
        baseDirectory += "/";
    }
    
    std::cout << "[AssetManager] Base directory set to: " << baseDirectory << std::endl;
}

// Get the global base directory
const std::string& AssetManager::getBaseDirectory()
{
    return baseDirectory;
}

// Get the directory for shaders (derived from base directory)
std::string AssetManager::getShaderDirectory()
{
    // Assuming shaders are in a "shaders/" subdirectory within the base directory
    return baseDirectory + "shaders/";
}

// Get the directory for textures (derived from base directory)
std::string AssetManager::getTextureDirectory()
{
    // Assuming textures are in a "textures/" subdirectory within the base directory
    return baseDirectory + "textures/";
}

// Get the full path for a shader file.
// Combines base directory, shader directory, and filename.
std::string AssetManager::getShaderPath(const std::string& filename)
{
    return getShaderDirectory() + filename;
}

// Get the full path for a texture file.
// Combines base directory, texture directory, and filename.
std::string AssetManager::getTexturePath(const std::string& filename)
{
    return getTextureDirectory() + filename;
}
