#version 420 core

#define MAXNUMPOINTLIGHTS 20

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
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec3 position;
	vec3 ambient;
	float constant;
	vec3 diffuse;
	float linear;
	vec3 specular;
	float quadratic;
};

layout(std140, binding = 1) uniform LightSet {
	DirectionalLight dirLight;
	PointLight pointLights[MAXNUMPOINTLIGHTS];
	int numPointLights;
} lightSet;

vec3 calculateAmbient(vec3 ambLight, vec3 fragColor) {
	return ambLight * fragColor;
}

vec3 calculateDiffuse(vec3 diffLight, vec3 fragColor, vec3 normal, vec3 lightDir) {
	float diff = max(dot(normal, lightDir), 0.0);
	return diffLight * (diff * fragColor);
}

vec3 calculateSpecular(vec3 specLight, vec3 fragColor, float shininess, vec3 normal, vec3 lightDir, vec3 viewDir) {
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
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

vec4 getLightCorrectedColor(Material mat, vec2 texCoords, LightPosSet posSet) {
	vec3 result = calculateDirectionLight(lightSet.dirLight, mat, texCoords, posSet);
	result = vec3(0.0);
	for (int i = 0; i < lightSet.numPointLights; i++)
		result += calculatePointLight(lightSet.pointLights[i], mat, texCoords, posSet);

	return vec4(result, 1.0);
}
