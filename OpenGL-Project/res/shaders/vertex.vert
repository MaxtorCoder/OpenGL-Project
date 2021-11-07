#version 460 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_vertexUV;
layout (location = 2) in vec3 in_normal;

out vec2 uv;
out vec3 positionWorldspace;
out vec3 normalCameraspace;
out vec3 eyeDirectionCameraspace;
out vec3 lightDirectionCameraspace;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;
uniform vec3 u_lightProjection;

void main()
{
    // Output position of the vertex, in clip space : projection * view * model * pos
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(in_position, 1.0);

    // Position of the vertex, in worldspace
    positionWorldspace = (u_modelMatrix * vec4(in_position, 1)).xyz;

    // Vector that goes from the vertex to the camera, in camera space.
    // In camera space, the camera is at the origin (0, 0, 0).
    vec3 vertexPosCameraSpace = (u_viewMatrix * u_modelMatrix * vec4(in_position, 1)).xyz;
    eyeDirectionCameraspace = vec3(0, 0, 0) - vertexPosCameraSpace;

    // Vector that goes from the vertex to the light, in camera space.
    // u_modelMatrix is ommited because it's identity
    vec3 lightPositionCameraSpace = (u_viewMatrix * vec4(u_lightProjection, 1)).xyz;
    lightDirectionCameraspace = lightPositionCameraSpace + eyeDirectionCameraspace;

    // Normal of the vertex, in camera space.
    // Only correct if u_modelMatrix does not scale the model, use it's inverse transpose if not.
    normalCameraspace = (u_viewMatrix * u_modelMatrix * vec4(in_normal, 0)).xyz;

    // UV of the vertex, no special space
    uv = in_vertexUV;
}