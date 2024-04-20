#version 400 core

layout(vertices = 4) out;

uniform mat4 modelView;


const float MIN_TESS = 4;
const float MAX_TESS = 64;

const float MIN_DIST = 30.0f;
const float MAX_DIST = 100.0f;

void main()
{
	// TODO

    // output position
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    // Only calculate the tessellation level once using the first vertex (no need to repeat for all vertices)
    if (gl_InvocationID == 0) {
        vec4 centers[4];
        centers[0] = gl_in[0].gl_Position; // (0,0)
        centers[1] = gl_in[1].gl_Position; // (1,0)
        centers[2] = gl_in[2].gl_Position; // (1,1)
        centers[3] = gl_in[3].gl_Position; // (0,1)

        // calculate the length of the positon vector at the center of each edge of the patch
        for (int i; i < 4; i++){
            centers[i] = (centers[i] + gl_in[(i+3) % 4].gl_Position) / 2.0f;
            // Proj center of edge to view referential to get the distance with obs
            float obsDist = length((modelView * centers[i]).xyz);
            // calculate linear mix factor and restrain it to [0,1] interval
            float mixFactor = (obsDist - MIN_DIST) / (MAX_DIST - MIN_DIST);
            mixFactor = clamp(mixFactor, 0.0f, 1.0f);
            // linear interpolation with mix function
            gl_TessLevelOuter[i] = mix(MIN_TESS, MAX_TESS, 1 - mixFactor);
        }
        gl_TessLevelInner[0] = max(gl_TessLevelOuter[1], gl_TessLevelOuter[3]);
        gl_TessLevelInner[1] = max(gl_TessLevelOuter[0], gl_TessLevelOuter[2]);
    }
}
