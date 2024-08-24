
uniform Material materials[32];
uniform int numTextures;
uniform vec3 viewPos;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main() {
	LightPosSet posSet = LightPosSet(
		Normal,
		FragPos,
		viewPos
	);
	
	FragColor = getLightCorrectedColor(materials[0], TexCoord, posSet);
}
