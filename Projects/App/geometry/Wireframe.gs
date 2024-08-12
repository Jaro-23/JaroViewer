#version 420 core
layout (triangles) in;
layout (line_strip, max_vertices = 10) out;

#define MAGNITUDE 0.4

layout(std140) uniform Transformation {
	mat4 projection;
	mat4 view;
};
uniform mat4 model;

in VS_OUT {
	vec3 Normal;
} gs_in[];

out vec4 Color;

void drawNormal(vec3 normal, int index) {
	Color = vec4(1.0, 0.0, 0.0, 1.0);
	gl_Position = projection * view * model * gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = projection * view * model * (gl_in[index].gl_Position + vec4(normal, 0.0) * MAGNITUDE);
	EmitVertex();
	EndPrimitive();
}

void main() {
	Color = vec4(1.0, 1.0, 1.0, 1.0);	
	gl_Position = projection * view * model * gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = projection * view * model * gl_in[1].gl_Position;
	EmitVertex();
	gl_Position = projection * view * model * gl_in[2].gl_Position;
	EmitVertex();
	gl_Position = projection * view * model * gl_in[0].gl_Position;
	EmitVertex();
	EndPrimitive();

	drawNormal(gs_in[0].Normal, 0);
	drawNormal(gs_in[1].Normal, 1);
	drawNormal(gs_in[2].Normal, 2);
}
