#version 410 core

in vec3 vTexCoord;
out vec4 fragColor;

uniform samplerCube uCubeTexture;

void main()
{
    fragColor = texture(uCubeTexture, vTexCoord);
}
