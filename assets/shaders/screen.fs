#version 330

in vec2 UVCoords;
uniform sampler2D screenTex;

out vec4 outColor;

void main() {
	outColor = texture( screenTex, UVCoords );
}