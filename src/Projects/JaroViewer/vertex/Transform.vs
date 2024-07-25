#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec4 transform(vec3 pos) {
	return projection * view * model * vec4(pos, 1.0);
}
