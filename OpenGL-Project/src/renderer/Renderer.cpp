#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "common/Define.h"
#include "entity/Camera.h"

#include "io/Shader.h"

Renderer::Renderer() : m_vertexBufferId(0), m_uvBufferId(0), m_vertexArrayId(0) {}
Renderer::~Renderer()
{
    glDeleteBuffers(1, &m_vertexBufferId);
    glDeleteBuffers(1, &m_vertexArrayId);
    glDeleteBuffers(1, &m_uvBufferId);

    delete m_camera;
}

void Renderer::Initialize()
{
    m_camera = new Camera();

    glGenBuffers(1, &m_vertexBufferId);
    glGenVertexArrays(1, &m_vertexArrayId);
    glGenBuffers(1, &m_uvBufferId);
}

void Renderer::EnqueueDrawEntity(Entity* entity)
{
    // m_enqueuedEntities.emplace(entity);
}

void Renderer::Draw(const Entity* entity) const
{
    Model* model = entity->GetModel();
    if (!model)
        return;

    if (!model->m_textureId)
        model->BindTexture();

    const std::vector<float>& vertices = model->m_vertices;
    const std::vector<float>& uvs = model->m_uvs;

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_uvBufferId);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_DYNAMIC_DRAW);

    model->m_shader->SetCameraMatrix(glm::value_ptr(m_camera->GetProjectionMatrix()), glm::value_ptr(m_camera->GetViewMatrix()));
    model->m_shader->SetModelMatrix(glm::value_ptr(entity->GetWorldMatrix()));

    model->m_shader->Bind();

    // Enable 0 location of vertex shader
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Enable 1 location of vertex shader
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvBufferId);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(m_vertexArrayId);
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Renderer::Update(float /*deltaTime*/)
{
    for (const Entity* entity : m_activeEntities)
        Draw(entity);
}

