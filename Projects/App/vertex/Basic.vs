layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

uniform mat3 normalModel;

void main() {
	gl_Position = transform(aPos);
	TexCoord = aTexCoord;
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = normalModel * aNormal;
}
