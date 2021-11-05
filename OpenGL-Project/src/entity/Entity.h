#ifndef ENTITY_H__
#define ENTITY_H__

#include <glm/mat4x4.hpp>

#include "io/Model.h"

class Entity
{
public:
    Entity();
    ~Entity();

    void UpdatePosition(glm::vec3 const& newPosition);

    glm::vec3 const& GetPosition() const { return m_position; }
    glm::mat4x4 const& GetWorldMatrix() const { return m_worldMatrix; }

private:
    glm::vec3 m_position = glm::vec3(0, 0, 5);
    glm::mat4x4 m_worldMatrix {};

    Model m_model{};
};

#endif // ENTITY_H__
