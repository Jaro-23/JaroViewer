#version 420 core

uniform mat4 model;
layout(std140, binding = 0) uniform Transformation {
	mat4 projection;
	mat4 view;
};

vec4 transform(vec3 pos) {
	return projection * view * model * vec4(pos, 1.0);
}
