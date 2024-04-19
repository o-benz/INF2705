#version 330 core

in vec2 texCoords;

uniform sampler2D textureSampler;

out vec4 FragColor;

void main()
{
    vec4 texel = texture(textureSampler, texCoords);
    if (texel.a < 0.3) discard;
    FragColor = texel;
}
