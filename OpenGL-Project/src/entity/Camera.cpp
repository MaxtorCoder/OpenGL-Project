#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Camera::UpdateProjectionMatrix()
{
    m_projectionMatrix = glm::perspective(
        glm::radians(GetFieldOfView()),
        4.0f / 3.0f,
        0.1f,
        100.0f
    );
}

void Camera::UpdateViewMatrix(glm::vec3 const& direction, glm::vec3 const& up)
{
    m_viewMatrix = glm::lookAt(
        GetPosition(),
        GetPosition() + direction,
        up
    );
}
