#ifndef CAMERA_H__
#define CAMERA_H__

#include "Entity.h"

class Camera : public Entity
{
public:
    void SetProjectionMatrix(glm::mat4x4 const& matrix) { m_projectionMatrix = matrix; }
    glm::mat4x4& GetProjectionMatrix() { return m_projectionMatrix; }

    void SetViewMatrix(glm::mat4x4 const& matrix) { m_viewMatrix = matrix; }
    glm::mat4x4& GetViewMatrix() { return m_viewMatrix; }

    float const& GetFieldOfView() const { return m_initialFov; }

private:
    glm::mat4x4 m_projectionMatrix{};
    glm::mat4x4 m_viewMatrix{};

    float m_initialFov = 45.0f;

};

#endif // CAMERA_H__
