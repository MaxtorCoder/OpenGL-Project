#version 460 core

// Interpolated values from the vertex shaders
in vec2 uv;
in vec3 positionWorldspace;
in vec3 normalCameraspace;
in vec3 eyeDirectionCameraspace;
in vec3 lightDirectionCameraspace;

// Output data
out vec3 color;

// Values that stay constant for the whole mesh
uniform sampler2D u_textureSampler;
uniform mat4 u_modelMatrix;
uniform vec3 u_lightProjection;

void main() {
    // Light emission properties
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    float lightPower = 50.0f;

    // Materia properties
    vec3 materialDiffuseColor = texture(u_textureSampler, uv).rgb;
    vec3 materialAmbientColor = vec3(0.1f, 0.1f, 0.1f) * materialDiffuseColor;
    vec3 materialSpecularColor = vec3(0.3f, 0.3f, 0.3f);

    // Distance to the light
    float distance = length(u_lightProjection - positionWorldspace);

    // Normal of the computed fragment, in camera space
    vec3 n = normalize(normalCameraspace);

    // Diretion of the light (from the fragment to the light)
    vec3 l = normalize(lightDirectionCameraspace);

    // Cosine of the angle between the normal and the light direction,
    // clamped above 0
    //  - light is at the vertical of the triangle -> 1
    //  - light is perpendicular to the triangle -> 0
    //  - light is behind the triangle -> 0
    float cosTheta = clamp(dot(n, l), 0, 1);

    vec3 e = normalize(eyeDirectionCameraspace);
    vec3 r = reflect(vec3(0, 0, 0), n);

    float cosAlpha = clamp(dot(e, r), 0, 1);

    color = materialAmbientColor +
            materialDiffuseColor * lightColor * lightPower * cosTheta / (distance * distance) +
            materialSpecularColor * lightColor * lightPower * pow(cosAlpha, 5) / (distance * distance);
}
