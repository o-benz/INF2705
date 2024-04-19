#version 330 core

in vec2 vTexture;

uniform sampler2D tex;

out vec4 FragColor;

void main()
{
    FragColor = texture(tex, vTexture);
    if (FragColor.a < 0.3) discard;
}