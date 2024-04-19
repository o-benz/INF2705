#version 330 core

const float DTIME_FACTOR = 0.1;
const float X_AMPLITUDE = 0.025;
const float Y_AMPLITUDE = 0.01;
const vec2 TIME_SCALE = vec2(1, 2);
const vec2 OFFSET_SCALE = vec2(2, 2);

in vec2 vTexture;

uniform sampler2D tex;
uniform float dtime;

out vec4 FragColor;

void main()
{
    float dx = DTIME_FACTOR * dtime + X_AMPLITUDE * cos(dtime * TIME_SCALE.x + (vTexture.x + vTexture.y) * OFFSET_SCALE.x);
    float dy = Y_AMPLITUDE * sin(dtime * TIME_SCALE.y + (vTexture.x + vTexture.y) * OFFSET_SCALE.y);
    FragColor = texture(tex, vTexture + vec2(dx, dy));
}