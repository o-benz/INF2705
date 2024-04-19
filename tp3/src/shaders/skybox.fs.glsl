#version 330 core

in vec3 texCoords;

uniform samplerCube textureSampler;

out vec4 FragColor;

void main()
{
    vec4 texel = texture(textureSampler, texCoords);
    FragColor = vec4(texel.rgb, 1.0f);
}
