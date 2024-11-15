#version 420 core

layout(std140, binding = 0) uniform Transformation {
	mat4 projection;
	mat4 view;
};
uniform mat4 model;

vec4 transform(vec3 pos) {
	return projection * view * model * vec4(pos, 1.0);
}
