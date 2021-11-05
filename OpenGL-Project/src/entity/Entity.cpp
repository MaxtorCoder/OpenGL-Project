#include "Entity.h"

Entity::Entity() = default;
Entity::~Entity() = default;

void Entity::UpdatePosition(glm::vec3 const& newPosition)
{
    m_position = newPosition;
}

