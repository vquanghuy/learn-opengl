# LEARN OPENGL

This repository contains my personal implementation and modifications of the tutorials found on the excellent learnopengl.com website and is specifically configured for macOS on Apple Silicon (arm64) and utilizes OpenGL 4.1.
Each tutorial is structured as a separate target within a single Xcode project (LearnOpenGL.xcodeproj), allowing for focused learning and execution of individual concepts.

The project aims to provide a working example of setting up an OpenGL development environment on macOS, including integrating necessary external libraries and macOS frameworks.

## Structure

The project follows a structured folder layout:

```
LearnOpenGL/
├── LearnOpenGL.xcodeproj/   // The main Xcode project file
├── Source/                  // Contains all C/C++ source code
│   ├── Shared/              // Optional: Common helper code (shaders, camera, etc.)
│   └── Tutorials/           // Source code for each tutorial/target
│       ├── 01-Triangle/     // Target 01: Drawing a triangle
│       ├── 02-Texture/      // Target 02: Adding texture
│       └── ...              // More tutorial folders
├── External/                // Contains external libraries
│   ├── glm/                 // GLM header-only library
│   ├── stb/                 // STB single-file headers (stb_image.h, etc.)
│   └── glfw/                // GLFW library (headers and compiled binaries)
│       ├── include/         // GLFW header files
│       └── lib/             // Compiled GLFW library files (architecture-specific)
│           ├── arm64/       // libglfw3.a for Apple Silicon
│           └── x86_64/    // libglfw3.a for Intel
├── Assets/                  // Optional: Textures, models, shaders
└── README.md
``` 

## Acknowledgements

This project is based on the fantastic tutorials provided by learnopengl.com. The content and structure of the tutorials have been an invaluable resource for learning OpenGL. My implementations include personal modifications and adaptations for the macOS environment.
