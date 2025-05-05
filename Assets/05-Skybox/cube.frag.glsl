#version 410 core

in vec2 vTexCoord;
out vec4 fragColor;

uniform sampler2D uTexture1;

void main()
{
    fragColor = texture(uTexture1, vTexCoord);
}
