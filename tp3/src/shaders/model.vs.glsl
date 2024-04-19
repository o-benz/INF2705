#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 inTexCoords;

out vec2 texCoords;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(position.xyz, 1.0);
    texCoords = inTexCoords;
}
