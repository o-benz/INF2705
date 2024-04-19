#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inColor;

uniform mat4 mvp;

out vec3 color;

void main()
{
    gl_Position = mvp * vec4(position.x, position.y, position.z, 1.0);
    color = inColor;
}
