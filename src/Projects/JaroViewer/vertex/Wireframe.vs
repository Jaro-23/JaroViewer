layout (location = 0) in vec3 aPos;

void main() {
	gl_Position = transform(aPos);
}
