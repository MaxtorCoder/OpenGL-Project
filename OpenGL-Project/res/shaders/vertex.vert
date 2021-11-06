#version 330 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_vertexUV;

out vec2 UV;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

void main()
{
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(in_position, 1.0);
    UV = in_vertexUV;
}