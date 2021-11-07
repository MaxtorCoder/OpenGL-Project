#include "Renderer.h"

#include <execution>

#include "common/Define.h"
#include "entity/Camera.h"

#include "io/Shader.h"

Renderer::Renderer() : m_vertexBufferId(0), m_uvBufferId(0), m_normalBufferId(0), m_vertexArrayId(0) {}
Renderer::~Renderer()
{
    glDeleteBuffers(1, &m_vertexBufferId);
    glDeleteBuffers(1, &m_uvBufferId);
    glDeleteBuffers(1, &m_normalBufferId);
    glDeleteBuffers(1, &m_vertexArrayId);

    delete m_camera;
}

void Renderer::Initialize()
{
    m_camera = new Camera();

    glGenBuffers(1, &m_vertexBufferId);
    glGenBuffers(1, &m_uvBufferId);
    glGenBuffers(1, &m_normalBufferId);
    glGenVertexArrays(1, &m_vertexArrayId);
}

void Renderer::EnqueueDrawEntity(Entity* entity)
{
    m_activeEntities.emplace_back(entity);
    // m_enqueuedEntities.emplace(entity);
}

void Renderer::Draw(const Entity* entity) const
{
    Model* model = entity->GetModel();
    if (!model)
        return;

    if (!model->m_textureId)
        model->BindTexture();

    const std::vector<glm::vec3>& vertices = model->m_vertices;
    const std::vector<glm::vec3>& normals = model->m_normals;
    const std::vector<glm::vec2>& uvs = model->m_uvs;

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_uvBufferId);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferId);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    // Set the projection matrix and view matrix based of camera data
    // also set the model matrix based of world position
    model->m_shader->SetCameraMatrix(m_camera->GetProjectionMatrix(), m_camera->GetViewMatrix());
    model->m_shader->SetModelMatrix(entity->GetWorldMatrix());

    // Bind the shader to the instance
    model->m_shader->Bind();

    // Update light position
    model->m_shader->SetVector3Location(model->m_shader->GetUniformLocation("u_lightProjection"), glm::vec3(4, 4, -4));

    // Enable location 0 of vertex shader
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Enable location 1 of vertex shader
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvBufferId);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Enable location 2 of vertex shader
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferId);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(m_vertexArrayId);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Renderer::Update(float /*deltaTime*/)
{
    for (const Entity* entity : m_activeEntities)
        Draw(entity);
}

