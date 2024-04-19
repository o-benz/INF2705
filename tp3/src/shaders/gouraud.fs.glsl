#version 330 core

in ATTRIB_VS_OUT
{
    vec2 texCoords;
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} attribIn;

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

out vec4 FragColor;

void main() {
	vec3 texDiffuse = vec3(1.0);
	float texSpec = 1.0;

	
	vec4 diffTex = texture(diffuseSampler, attribIn.texCoords);
	if (diffTex.a < 0.3) discard;
	texDiffuse = diffTex.rgb;
	texSpec = texture(specularSampler, attribIn.texCoords).r;
	

	vec3 color = attribIn.emission;
	color += (attribIn.ambient + attribIn.diffuse) * texDiffuse;
	color += attribIn.specular * texSpec;
	FragColor = vec4(clamp(color, 0.0f, 1.0f), 1.0f);
}
