#include "Entity.h"

Entity::Entity()
{
    m_worldMatrix = glm::mat4x4(1.0f);
}

Entity::~Entity()
{
    delete m_model;
}

void Entity::LoadModel(std::string const& filename)
{
    m_model = Model::Load(filename);
}

