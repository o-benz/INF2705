#version 330 core

in ATTRIB_GS_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
    vec3 barycentricCoords;
} attribIn;

uniform sampler2D groundSampler;
uniform sampler2D sandSampler;
uniform sampler2D snowSampler;
uniform bool viewWireframe;

out vec4 FragColor;

float edgeFactor(vec3 barycentricCoords, float width)
{
    vec3 d = fwidth(barycentricCoords);
    vec3 f = step(d * width, barycentricCoords);
    return min(min(f.x, f.y), f.z);
}

float edgeFactor(vec4 barycentricCoords, float width)
{
    vec4 d = fwidth(barycentricCoords);
    vec4 f = step(d * width, barycentricCoords);
    return min(min(min(f.x, f.y), f.z), f.w);
}

const vec3 WIREFRAME_COLOR = vec3(0.5f);
const vec3 PATCH_EDGE_COLOR = vec3(1.0f, 0.0f, 0.0f);

const float WIREFRAME_WIDTH = 0.5f;
const float PATCH_EDGE_WIDTH = 0.5f;

vec4 computeColor(vec4 sandTex, vec4 groundTex, vec4 snowTex, float height) {
    // height < 0.6 => sand and ground texture for fragment color
    // height > 0.6 => ground and snow texture for fragment color
    if (height < 0.6f) {
        // height < 0.3f => sand, mixFactor = 0; height > 0.35f => ground, mixFactor = 1; 0.3 < height < 0.35 => Hermite interpolation with smoothstep()
        float mixFactor = smoothstep(0.3f, 0.35f, height);
        return mix(sandTex, groundTex, mixFactor);
    } else {
        // height < 0.6f => ground, mixFactor = 0; height > 0.65f => snow, mixFactor = 1; 0.6 < height < 0.65 => Hermite interpolation with smoothstep()
        float mixFactor = smoothstep(0.6f, 0.65f, height);
        return mix(groundTex, snowTex, mixFactor);
    }
}

void applyWireframeEffect() {
    // mix final color of textures with wireframe color to show wireframe
    float edgeMixFactor = edgeFactor(attribIn.barycentricCoords, WIREFRAME_WIDTH);
    float patchEdgeMixFactor = edgeFactor(attribIn.patchDistance, PATCH_EDGE_WIDTH);

    // mix finale color of textures with patch edge color to show patches edges
    FragColor.rgb = mix(FragColor.rgb, WIREFRAME_COLOR, 1 - edgeMixFactor);
    FragColor.rgb = mix(FragColor.rgb, PATCH_EDGE_COLOR, 1 - patchEdgeMixFactor);
}

void main() {
    // Get texels for terrain color
    vec4 sandTex = texture(sandSampler, attribIn.texCoords);
    vec4 groundTex = texture(groundSampler, attribIn.texCoords);
    vec4 snowTex = texture(snowSampler, attribIn.texCoords);

    // Find FragColor based on height of vertex
    FragColor = computeColor(sandTex, groundTex, snowTex, attribIn.height);

    // show wireframe if activated
    if (viewWireframe) {
        // Modify FragColor for wireframe
        applyWireframeEffect();
    }
}