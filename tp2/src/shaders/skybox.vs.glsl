#version 330 core

layout (location = 0) in vec3 inPosition;

uniform mat4 mvp;

out vec3 vTexture;

void main()
{
    vec4 out_position = mvp * vec4(inPosition, 1.0);
    gl_Position = out_position.xyww;
    vTexture = inPosition;
}