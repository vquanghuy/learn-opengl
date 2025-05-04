#include "FPSLimiter.h"

// Constructor implementation
FPSLimiter::FPSLimiter(int targetFPS) : targetFPS(targetFPS) {
    targetFrameTime = 1.0 / targetFPS;
    lastTime = std::chrono::high_resolution_clock::now();
}

// Method to call each frame to limit the frame rate.
void FPSLimiter::limit() {
    const auto currentTime = std::chrono::high_resolution_clock::now();
    const double elapsedTime = std::chrono::duration<double>(currentTime - lastTime).count();

    if (elapsedTime < targetFrameTime) {
        std::this_thread::sleep_for(std::chrono::duration<double>(targetFrameTime - elapsedTime));
    }

    lastTime = std::chrono::high_resolution_clock::now();
}

// Set a new target FPS.
void FPSLimiter::setTargetFPS(int targetFPS) {
    this->targetFPS = targetFPS;
    targetFrameTime = 1.0 / targetFPS;
}
