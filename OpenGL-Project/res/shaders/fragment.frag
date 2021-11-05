#version 330 core
in vec2 UV;

out vec3 color;

uniform sampler2D u_textureSampler;

void main() {
	color = texture(u_textureSampler, UV).rgb;
}
