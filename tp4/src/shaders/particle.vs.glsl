#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 velocity;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 size;
layout (location = 4) in float timeToLive;

uniform mat4 modelView;

out ATTRIB_VS_OUT
{
    vec4 color;
    vec2 size;
} attribOut;

void main() {
    gl_Position = modelView * vec4(position, 1.0);
    attribOut.color = color;
    attribOut.size = size;
}
