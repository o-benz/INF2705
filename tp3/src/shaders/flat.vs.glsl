#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out ATTRIB_OUT
{
    vec3 position;
    vec2 texCoords;
} attribOut;

uniform mat4 mvp;

void main() {
	gl_Position = mvp * vec4(position, 1.0);
	attribOut.position = position;
	attribOut.texCoords = texCoords;
}
