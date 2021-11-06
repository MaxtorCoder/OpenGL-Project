#ifndef ENTITY_H__
#define ENTITY_H__

#include <glm/mat4x4.hpp>

#include "io/Model.h"

class Entity
{
public:
    Entity();
    ~Entity();

    void LoadModel(std::string const& filename);

    void SetPosition(const glm::vec3& position) { m_worldMatrix[3] = glm::vec4(position, 1.0f); }
    glm::vec3 GetPosition() const { return m_worldMatrix[3]; }

    const glm::mat4x4& GetWorldMatrix() const { return m_worldMatrix; }

    Model* GetModel() const { return m_model; }

private:
    glm::mat4x4 m_worldMatrix{};

    Model* m_model = nullptr;
};

#endif // ENTITY_H__
