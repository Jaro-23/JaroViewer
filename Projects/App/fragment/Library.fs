#version 420 core

#define MAXNUMDIRLIGHTS 2
#define MAXNUMPOINTLIGHTS 20
#define MAXNUMSPOTLIGHTS 10

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct LightPosSet {
	vec3 normal;
	vec3 fragPos;
	vec3 viewPos;
};

struct DirectionalLight {
	vec3 direction;
	bool enable;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec3 position;
	bool enable;
	vec3 ambient;
	float constant;
	vec3 diffuse;
	float linear;
	vec3 specular;
	float quadratic;
};

struct Spotlight{
	vec3 position;
	float cutOff;
	vec3 direction;
	float outerCutOff;
	vec3 ambient;
	float constant;
	vec3 diffuse;
	float linear;
	vec3 specular;
	float quadratic;
	bool enable;
};

layout(std140, binding = 1) uniform LightSet {
	DirectionalLight dirLights[MAXNUMDIRLIGHTS];
	PointLight pointLights[MAXNUMPOINTLIGHTS];
	Spotlight spotlights[MAXNUMSPOTLIGHTS];
	int numDirLights;
	int numPointLights;
	int numSpotlights;
} lightSet;

vec3 calculateAmbient(vec3 ambLight, vec3 fragColor) {
	return ambLight * fragColor;
}

vec3 calculateDiffuse(vec3 diffLight, vec3 fragColor, vec3 normal, vec3 lightDir) {
	float diff = max(dot(normal, lightDir), 0.0);
	return diffLight * (diff * fragColor);
}

vec3 calculateSpecular(vec3 specLight, vec3 fragColor, float shininess, vec3 normal, vec3 lightDir, vec3 viewDir) {
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
	return specLight * (spec * fragColor);
}

vec3 calculateDirectionLight(DirectionalLight light, Material mat, vec2 texCoords, LightPosSet posSet) {

	vec3 norm = normalize(posSet.normal);
	vec3 lightDir = normalize(light.direction);
	vec3 viewDir = normalize(posSet.viewPos - posSet.fragPos);

	vec3 diffuseColor = vec3(texture(mat.diffuse, texCoords));
	vec3 specularColor = vec3(texture(mat.specular, texCoords));

	vec3 ambient = calculateAmbient(light.ambient, diffuseColor);
	vec3 diffuse = calculateDiffuse(light.diffuse, diffuseColor, norm, -lightDir);
	vec3 specular = calculateSpecular(light.specular, specularColor, mat.shininess, norm, -lightDir, viewDir);

	return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, Material mat, vec2 texCoords, LightPosSet posSet) {
	if (!light.enable)
		return vec3(0.0);

	vec3 norm = normalize(posSet.normal);
	vec3 lightDir = normalize(light.position - posSet.fragPos);
	vec3 viewDir = normalize(posSet.viewPos - posSet.fragPos);

	vec3 diffuseColor = vec3(texture(mat.diffuse, texCoords));
	vec3 specularColor = vec3(texture(mat.specular, texCoords));

	vec3 ambient = calculateAmbient(light.ambient, diffuseColor);
	vec3 diffuse = calculateDiffuse(light.diffuse, diffuseColor, norm, lightDir);
	vec3 specular = calculateSpecular(light.specular, specularColor, mat.shininess, norm, lightDir, viewDir);

	float distance = length(light.position - posSet.fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	return attenuation * (ambient + diffuse + specular);
}

vec3 calculateSpotlight(Spotlight light, Material mat, vec2 texCoords, LightPosSet posSet) {
	if (!light.enable)
		return vec3(0.0);

	vec3 norm = normalize(posSet.normal);
	vec3 lightDir = normalize(light.position - posSet.fragPos);
	vec3 viewDir = normalize(posSet.viewPos - posSet.fragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	if (theta < light.outerCutOff)
		return vec3(0.0f);

	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 diffuseColor = vec3(texture(mat.diffuse, texCoords));
	vec3 specularColor = vec3(texture(mat.specular, texCoords));

	vec3 ambient = calculateAmbient(light.ambient, diffuseColor);
	vec3 diffuse = calculateDiffuse(light.diffuse, diffuseColor, norm, lightDir);
	vec3 specular = calculateSpecular(light.specular, specularColor, mat.shininess, norm, lightDir, viewDir);

	float distance = length(light.position - posSet.fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	return intensity * attenuation * (ambient + diffuse + specular);
}

vec4 getLightCorrectedColor(Material mat, vec2 texCoords, LightPosSet posSet) {
	vec3 result = vec3(0.0);
	for (int i = 0; i < lightSet.numDirLights; i++)
		result = calculateDirectionLight(lightSet.dirLights[i], mat, texCoords, posSet);
	for (int i = 0; i < lightSet.numPointLights; i++)
		result += calculatePointLight(lightSet.pointLights[i], mat, texCoords, posSet);
	for (int i = 0; i < lightSet.numSpotlights; i++)
		result += calculateSpotlight(lightSet.spotlights[i], mat, texCoords, posSet);

	return vec4(result, 1.0);
}
