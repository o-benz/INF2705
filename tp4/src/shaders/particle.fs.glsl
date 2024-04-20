#version 330 core

out vec4 FragColor;


uniform sampler2D textureSampler;

in ATTRIB_GS_OUT
{
    vec4 color;
    vec2 texCoords;
} attribIn;

void main() {
    FragColor = attribIn.color;
    vec4 texel = texture(textureSampler, attribIn.texCoords);
    FragColor.a = mix(texel.a, attribIn.color.a, 0.5); // after testing I find that only mixing alpha leads to a better looking effect
    if (FragColor.a <= 0.05) discard;
}