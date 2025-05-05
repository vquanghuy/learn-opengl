#ifndef FPS_LIMITER_H
#define FPS_LIMITER_H

#include <chrono>
#include <thread>

class FPSLimiter {
public:
    // Constructor: Sets the target FPS.
    FPSLimiter(int targetFPS);
    
    // Method to call each frame to limit the frame rate.
    void limit();
    
    // Get the delta time calculated during the last call to limit().
    float getDeltaTime() const;
    
    // Set a new target FPS.
    void setTargetFPS(int targetFPS);
    
    // Get the current target FPS.
    int getTargetFPS() const { return targetFPS; }
    
private:
    int targetFPS;
    double targetFrameTime;
    std::chrono::high_resolution_clock::time_point lastTime;
    float deltaTime;
};

#endif // FPS_LIMITER_H
