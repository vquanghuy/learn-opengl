#version 410 core

layout (location = 0) in vec3 aPos;

out vec3 vTexCoord;

uniform mat4 uProjection;
uniform mat4 uView;

void main()
{
    vTexCoord = aPos;
    // We use a 4x4 matrix multiplication for the view matrix,
    // but we discard the translation part by setting the w component to 0.0.
    // This makes the skybox appear infinitely far away.
    vec4 pos = uProjection * uView * vec4(aPos, 1.0);
    gl_Position = pos.xyww; // Set z to w to ensure z is always 1.0 after perspective divide
}

