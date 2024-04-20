#version 330 core

layout (location = 0) in vec3 position;

out vec3 texCoords;

uniform mat4 mvp;

void main()
{
    vec4 pos = mvp * vec4(position.x, position.y, position.z, 1.0);
    gl_Position = pos.xyww;
    texCoords = position;
}
