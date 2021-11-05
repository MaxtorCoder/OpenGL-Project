#ifndef ENTITY_H__
#define ENTITY_H__

#include <glm/mat4x4.hpp>
#include "renderer/Shader.h"

class Entity
{
public:
    Entity();
    ~Entity();

    glm::mat4x4 const& GetWorldMatrix() const { return m_worldMatrix; }

private:
    glm::mat4x4 m_worldMatrix {};
    Shader* m_shader = nullptr;
};

#endif // ENTITY_H__
