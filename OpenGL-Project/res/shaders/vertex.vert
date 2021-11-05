#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

uniform mat4 u_modelMatrix;

void main()
{
    gl_Position = u_modelMatrix * vec4(aPos, 1.0);
    fragmentColor = vertexColor;
}