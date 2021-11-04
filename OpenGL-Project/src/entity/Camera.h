#ifndef CAMERA_H__
#define CAMERA_H__

#include "Entity.h"

class Camera : public Entity
{
public:
    glm::mat4x4 const& GetProjectionMatrix() const { return m_projectionMatrix; }
    glm::mat4x4 const& GetViewMatrix() const { return m_viewMatrix; }

private:
    glm::mat4x4 m_projectionMatrix{};
    glm::mat4x4 m_viewMatrix{};
};

#endif // CAMERA_H__
