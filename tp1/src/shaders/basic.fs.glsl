#version 330 core


uniform vec4 globalColor;

out vec4 fragColor;

void main()
{
    fragColor = globalColor;
}