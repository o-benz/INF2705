#version 330 core

in ATTRIB_VS_OUT
{
    vec2 texCoords;
    vec3 normal;
    vec3 lightDir[3];
    vec3 spotDir[3];
    vec3 obsPos;
} attribIn;

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

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

out vec4 FragColor;

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
	vec3 color = vec3(0.0);
	vec3 texDiffuse = texture(diffuseSampler, attribIn.texCoords).rgb;

	color += mat.ambient * lights[lightIndex].ambient * texDiffuse;

	float LdotN = dot(lightDir, normal);
	if (LdotN > 0.0) {
		float spot = useSpotlight ? computeSpotlight(attribIn.spotDir[lightIndex], lightDir, normal) : 1.0f;
		color += spot * mat.diffuse * lights[lightIndex].diffuse * LdotN * texDiffuse;

		float spec = 0.0;
		if (useBlinn) {
			spec = dot(normalize(lightDir + obsPos), normal);
		} else {
			spec = dot(reflect(-lightDir, normal), obsPos);
		}
		if (spec > 0) {
			float texSpec = texture(specularSampler, attribIn.texCoords).r;
			spec = pow(spec, mat.shininess);
			color += spot * mat.specular * lights[lightIndex].specular * spec * texSpec;
		}
	}
	return color;
}

void main() {
	vec3 color = mat.emission;
	vec3 texDiffuse = vec3(1.0);
	vec4 tex = texture(diffuseSampler, attribIn.texCoords);
	if (tex.a < 0.3) discard;
	texDiffuse = tex.rgb;
	color += mat.ambient * lightModelAmbient * texDiffuse;

	vec3 normal = normalize(attribIn.normal);
	vec3 obsPos = normalize(attribIn.obsPos);

	for (int i = 0; i < 3; ++i) {
		vec3 lightDir = normalize(attribIn.lightDir[i]);
		color += computeLightModel(i, normal, lightDir, obsPos);
	}

	FragColor = clamp(vec4(color, 1.0), 0.0, 1.0);
}