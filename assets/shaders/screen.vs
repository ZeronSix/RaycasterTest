#version 330

in vec2 vert;
in vec2 uv;

out vec2 UVCoords;

void main() {
	UVCoords = uv;
	gl_Position = vec4( vert, 0.0, 1.0 );
}