#version 330 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;

uniform mat4 mvp;

void main()
{
    vec3 normal_unit = normalize(normal);
    vec3 pos = position + 0.1 * normal_unit;
    gl_Position = mvp * vec4(pos, 1.0);
}
