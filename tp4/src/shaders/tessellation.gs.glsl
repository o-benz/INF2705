#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;


in ATTRIB_TES_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
} attribIn[];

out ATTRIB_GS_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
    vec3 barycentricCoords;
} attribOut;

void setVertexAttributes(int index, vec3 vertexBarycentricCoords) {
    gl_Position = gl_in[index].gl_Position;
    attribOut.height = attribIn[index].height;
    attribOut.texCoords = attribIn[index].texCoords;
    attribOut.patchDistance = attribIn[index].patchDistance;
    attribOut.barycentricCoords = vertexBarycentricCoords;
}

void main() {
    // Define a barycentric coordinate array for each vertex of the triangle
    const vec3 vertexBarycentricCoords[3] = {
        vec3(1.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 0.0f, 1.0f)
    };

    // Emit each vertex with corresponding barycentric coordinates
    for (int i = 0; i < gl_in.length(); ++i) {
        setVertexAttributes(i, vertexBarycentricCoords[i]);
        EmitVertex();
    }
    EndPrimitive();
}