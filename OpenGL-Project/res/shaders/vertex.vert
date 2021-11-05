#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 vertexUV;

out vec2 UV;

uniform mat4 u_modelMatrix;

void main()
{
    gl_Position = u_modelMatrix * vec4(aPos, 1.0);
    UV = vertexUV;
}