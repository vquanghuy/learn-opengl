#include "FPSLimiter.h"

// Constructor implementation
FPSLimiter::FPSLimiter(int targetFPS) : targetFPS(targetFPS), deltaTime(0.0f) { // Initialize deltaTime
    targetFrameTime = 1.0 / targetFPS;
    lastTime = std::chrono::high_resolution_clock::now();
}

// Method to call each frame to limit the frame rate.
// Calculates and updates the internal delta time.
void FPSLimiter::limit() {
    const auto currentTime = std::chrono::high_resolution_clock::now();
    const double elapsedTime = std::chrono::duration<double>(currentTime - lastTime).count();
    
    if (elapsedTime < targetFrameTime) {
        std::this_thread::sleep_for(std::chrono::duration<double>(targetFrameTime - elapsedTime));
    }
    
    // Calculate the actual time elapsed since the last frame *after* sleeping
    const auto frameEndTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<float>(frameEndTime - lastTime).count(); // Store deltaTime
    
    lastTime = frameEndTime; // Update lastTime to the end of the current frame
}

// Get the delta time calculated during the last call to limit().
float FPSLimiter::getDeltaTime() const {
    return deltaTime;
}

// Set a new target FPS.
void FPSLimiter::setTargetFPS(int targetFPS) {
    this->targetFPS = targetFPS;
    targetFrameTime = 1.0 / targetFPS;
}
