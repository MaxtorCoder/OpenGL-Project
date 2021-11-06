#include "Model.h"

#include <fstream>

#include "common/Define.h"

Model::~Model()
{
    delete m_shader;
    delete m_texture;
}

Model* Model::Load(std::string const& filename)
{
    std::ifstream file(filename, std::ios::in);
    if (!file.is_open())
        return nullptr;




    /*Model* model = new Model();
    model->m_vertices = std::move(vertices);
    model->m_uvs = std::move(uvs);

    Shader* shader = new Shader();
    if (shader->LoadShader(PROJECT_DIR("res/shaders/vertex.vert"), PROJECT_DIR("res/shaders/fragment.frag")))
        model->m_shader = shader;
    else
        delete shader;

    Texture* texture = new Texture();
    if (texture->Load(PROJECT_DIR("res/images/cube.bmp")))
        model->m_texture = texture;
    else
        delete texture;

    return model;*/
}

void Model::BindTexture()
{
    m_textureId = m_texture->Bind();

    const int32_t textureIdShader = m_shader->GetUniformLocation("u_textureSampler");

    // Bind our texture in Texture Unit 0 and set our
    // texture sampler to use Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glUniform1i(textureIdShader, 0);
}
