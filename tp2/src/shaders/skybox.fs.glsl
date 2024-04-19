#version 330 core

in vec3 vTexture;

uniform samplerCube tex;

out vec4 FragColor;

void main()
{
    FragColor = texture(tex, vTexture);
}