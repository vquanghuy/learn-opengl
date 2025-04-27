#version 410 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inCoord;

out vec2 texCoord;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(inPos, 1.0);
    texCoord = inCoord;
}