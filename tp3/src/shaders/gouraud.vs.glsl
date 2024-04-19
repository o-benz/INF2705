#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

out ATTRIB_VS_OUT
{
    vec2 texCoords;
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} attribOut;

uniform mat4 mvp;
uniform mat4 view;
uniform mat4 modelView;
uniform mat3 normalMatrix;

struct Material
{
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct UniversalLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    vec3 spotDirection;
};

layout (std140) uniform LightingBlock
{
    Material mat;
    UniversalLight lights[3];
    vec3 lightModelAmbient;
    bool useBlinn;
    bool useSpotlight;
    bool useDirect3D;
    float spotExponent;
    float spotOpeningAngle;
};


float computeSpotlight(in vec3 spotDir, in vec3 lightDir, in vec3 normal) {
	float cosDelta = cos(radians(spotOpeningAngle));
	float cosGamma = dot(normalize(spotDir), lightDir);

	if (!useDirect3D) {
		if (cosGamma > cosDelta)
			return pow(cosGamma, spotExponent);
		else
			return 0.0;
	} else {
		float cosInner = cosDelta;
		float cosOuter = pow(cosDelta, 1.01 + spotExponent / 2.0);

		if (cosGamma > cosInner)
			return 1.0;
		else if (cosGamma < cosOuter)
			return 0.0;
		else
			return (cosGamma - cosOuter) / (cosInner - cosOuter);
	}
	return 0.0;
}

vec3 computeLightModel(in int lightIndex, in vec3 normal, in vec3 lightDir, in vec3 obsPos) {
	vec3 spotDir = mat3(view) * -lights[lightIndex].spotDirection;

	attribOut.ambient += mat.ambient * lights[lightIndex].ambient;
	
	float LdotN = dot(lightDir, normal);
	if (LdotN > 0.0) {
		float spot = useSpotlight ? computeSpotlight(spotDir, lightDir, normal) : 1.0f;
		attribOut.diffuse += spot * mat.diffuse * lights[lightIndex].diffuse * LdotN;

		float spec = 0.0;
		if (useBlinn) {
			spec = dot(normalize(lightDir + obsPos), normal);
		} else {
			spec = dot(reflect(-lightDir, normal), obsPos);
		}

		if (spec > 0) {
			spec = pow(spec, mat.shininess);
			attribOut.specular += spot * mat.specular * lights[lightIndex].specular * spec;
		}
	}
	return vec3(0.0);
}

void main() {
	gl_Position = mvp * vec4(position, 1.0);

	attribOut.texCoords = texCoords;

	attribOut.emission = mat.emission;
	attribOut.ambient = mat.ambient * lightModelAmbient;
	attribOut.diffuse = vec3(0.0);
	attribOut.specular = vec3(0.0);

	vec3 pos = (modelView * vec4(position, 1.0)).xyz;
	vec3 lightDir[3];
	for (int i = 0; i < 3; ++i) {
		lightDir[i] = (view * vec4(lights[i].position, 1.0)).xyz - pos;
		computeLightModel(i,
			normalize(normalMatrix * normal),
			normalize(lightDir[i]),
			normalize(-pos)
		);
	}
}
