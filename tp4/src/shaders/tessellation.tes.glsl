#version 400 core

layout(quads) in;

/*
in Attribs {
    vec4 couleur;
} AttribsIn[];*/


out ATTRIB_TES_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
} attribOut;

uniform mat4 mvp;

uniform sampler2D heighmapSampler;

vec4 interpole( vec4 v0, vec4 v1, vec4 v2, vec4 v3 )
{
    // mix( x, y, f ) = x * (1-f) + y * f.
    // TODO
    // Interpolation for each vertex
    vec4 v01 = mix(v0, v1, gl_TessCoord.x);
    vec4 v32 = mix(v3, v2, gl_TessCoord.x);
    return mix(v01, v32, gl_TessCoord.y);
}


const float PLANE_SIZE = 256.0f;

void main()
{
	// TODO
    // 2D interpolation to find vertices position in a [0,1] interval
    vec4 position = interpole(
        gl_in[0].gl_Position, 
        gl_in[1].gl_Position,
        gl_in[2].gl_Position, 
        gl_in[3].gl_Position
    );

    // Find texCoords using interpolated position and converting it to [0,1] interval with the size of the plane
    vec2 texCoords = position.xz / PLANE_SIZE + 0.5f;

    // Extract a texel from the heigtmapSampler image with only one active component (it's in black and white)
    // texCoord is divided by 4 to reduce the mountain frequencies
    float height = texture(heighmapSampler, texCoords / 4.0f).r;

    // set output height attribute
    attribOut.height = height;

    // set patchDistance output value using gl_TessCoord
    // each component of the vector is the distance relative to a side of the patch
    attribOut.patchDistance = vec4(
        gl_TessCoord.x, 
        gl_TessCoord.y, 
        1 - gl_TessCoord.x, 
        1 - gl_TessCoord.y
    );

    // set output texture coordinate attribute multiplied by 2 to add repetition to the texture
    attribOut.texCoords = gl_TessCoord.xy * 2.0f;

    // Matrix transformation for position output with height conversion from [0, 1] to [-32, 32] (y axis)
    gl_Position = mvp * (position + vec4(0.0f, height * 64.0f - 32.0f, 0.0f, 0.0f));
}
