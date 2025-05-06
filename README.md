# Learn OpenGL

This repository contains my personal implementation and modifications of the tutorials found on the excellent [learnopengl.com](https://learnopengl.com/) and is specifically configured for macOS on **Apple Silicon (arm64)** and utilizes **OpenGL 4.1**.

Each tutorial is structured as a separate target within a single Xcode project (LearnOpenGL.xcodeproj), allowing for focused learning and execution of individual concepts.

The project aims to provide a working example of setting up an OpenGL development environment on macOS, including integrating necessary external libraries and macOS frameworks.

For detailed documentation, setup instructions, and troubleshooting guides, please visit the [Wiki](https://github.com/vquanghuy/learn-opengl/wiki).

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
│           └── x86_64/      // libglfw3.a for Intel
├── Assets/                  // Textures, models, shaders - will be copied to executable folder
└── README.md
```

The source code also structured based on [Modularizing OpenGL Development with C++: Building Essential Classes](https://github.com/vquanghuy/learn-opengl/wiki/Modularizing-OpenGL-Development-with-C--:-Building-Essential-Classes)

## Environment and Libraries

This project is developed and tested on the following environment:

- Processor: M1 Pro (arm64 architecture)
- Operating System: macOS 15.4.1
- OpenGL Version: 4.1

The following external libraries are used:

- [GLAD](https://glad.dav1d.de/): Used as the OpenGL loader to manage function pointers.
- [GLFW](https://www.glfw.org/): Provides window creation, OpenGL context management, and input handling.
- [STB](https://github.com/nothings/stb): Used for loading images (specifically [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)) for textures.
- [GLM](https://github.com/g-truc/glm): A header-only mathematics library for graphics software.
- [Dear ImGui](https://github.com/ocornut/imgui): A bloat-free graphical user interface library for C++.
- [tinygltf](https://github.com/syoyo/tinygltf): A minimal C++ library for loading glTF 3D models.
- [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader): A minimal C++ library for loading OBJ 3D models.
- [Jolt Physics](https://github.com/jrouwe/JoltPhysics): A high-performance, open-source, C++ physics engine.

## Acknowledgements

This project is based on the fantastic tutorials provided by [learnopengl.com](https://learnopengl.com). The content and structure of the tutorials have been an invaluable resource for learning OpenGL. 

My implementations include personal modifications and adaptations for the macOS environment.

## Tutorials

<img width="400" alt="tut-01-triangle" src="https://github.com/user-attachments/assets/4b3b75ce-9d32-43f8-980e-07c608e9c7f7" />

<img width="400" alt="tut-02-texture" src="https://github.com/user-attachments/assets/ad73842b-d2fa-402e-bd86-bd69d9382da1" />

<img width="400" alt="tut-03-transformation" src="https://github.com/user-attachments/assets/c0be0457-2635-4677-8287-0c5e0c114c77" />

<img width="400" alt="tut-04-camera" src="https://github.com/user-attachments/assets/c5f8145d-9536-4fbc-b673-122d8334cd99" />

<img width="400" alt="tut-05-skybox" src="https://github.com/user-attachments/assets/6ac7f9fd-7402-4e8a-b36d-ba6645aaec67" />






