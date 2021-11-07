#include "Model.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include "common/Define.h"

Model::~Model()
{
    // delete m_shader;
    // delete m_texture;
}

bool Model::LoadOBJ(std::string const& path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals)
{
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    std::ifstream file(path, std::ios::in);
    if (!file.is_open())
        return false;

#define READ_FROM_STREAM(stream, value) \
    stream >> value; \
    stream.ignore(1, ' ');

#define READ_VERTICES(stream, value) \
    stream >> value; \
    if (stream.peek() == '/' || stream.peek() == ' ') \
    { \
        uint64_t length = stream.tellg(); \
        stream.seekg(length + 1); \
    } \

    std::string line;
    while (getline(file, line))
    {
        std::stringstream stringStream(line);
        if (line.find("v ") != std::string::npos)
        {
            stringStream.ignore(2, ' ');

            glm::vec3 position{};
            READ_FROM_STREAM(stringStream, position.x)
            READ_FROM_STREAM(stringStream, position.y)
            READ_FROM_STREAM(stringStream, position.z)
            temp_vertices.emplace_back(position);
        }
        else if (line.find("vt ") != std::string::npos)
        {
            stringStream.ignore(3, ' ');

            glm::vec2 uv{};
            READ_FROM_STREAM(stringStream, uv.x)
            READ_FROM_STREAM(stringStream, uv.y)
            temp_uvs.emplace_back(uv);
        }
        else if (line.find("vn ") != std::string::npos)
        {
            stringStream.ignore(3, ' ');

            glm::vec3 normals{};
            READ_FROM_STREAM(stringStream, normals.x)
            READ_FROM_STREAM(stringStream, normals.y)
            READ_FROM_STREAM(stringStream, normals.z)
            temp_normals.emplace_back(normals);
        }
        else if (line.find("f ") != std::string::npos)
        {
            stringStream.ignore(2, ' ');

            uint32_t vertexIndex[3]{};
            uint32_t uvIndex[3]{};
            uint32_t normalIndex[3]{};

            READ_VERTICES(stringStream, vertexIndex[0])
            READ_VERTICES(stringStream, uvIndex[0])
            READ_VERTICES(stringStream, normalIndex[0])

            READ_VERTICES(stringStream, vertexIndex[1])
            READ_VERTICES(stringStream, uvIndex[1])
            READ_VERTICES(stringStream, normalIndex[1])

            READ_VERTICES(stringStream, vertexIndex[2])
            READ_VERTICES(stringStream, uvIndex[2])
            READ_VERTICES(stringStream, normalIndex[2])

            out_vertices.push_back(temp_vertices[vertexIndex[0] - 1]);
            out_vertices.push_back(temp_vertices[vertexIndex[1] - 1]);
            out_vertices.push_back(temp_vertices[vertexIndex[2] - 1]);

            out_uvs.push_back(temp_uvs[uvIndex[0] - 1]);
            out_uvs.push_back(temp_uvs[uvIndex[1] - 1]);
            out_uvs.push_back(temp_uvs[uvIndex[2] - 1]);

            out_normals.push_back(temp_normals[normalIndex[0] - 1]);
            out_normals.push_back(temp_normals[normalIndex[1] - 1]);
            out_normals.push_back(temp_normals[normalIndex[2] - 1]);
        }
    }

    file.close();
#undef READ_FROM_STREAM
#undef READ_VERTICES

    return true;
}

Model* Model::Load(std::string const& filename)
{
    Model* model = new Model();
    if (!LoadOBJ(filename, model->m_vertices, model->m_uvs, model->m_normals))
    {
        delete model;
        return nullptr;
    }

    //Shader* shader = new Shader();
    //if (shader->LoadShader(PROJECT_DIR("res/shaders/vertex.vert"), PROJECT_DIR("res/shaders/fragment.frag")))
    //    model->m_shader = shader;
    //else
    //{
    //    delete model;
    //    delete shader;
    //    return nullptr;
    //}

    //Texture* texture = new Texture();
    //if (texture->Load(PROJECT_DIR("res/model/Cube_UV_Texture.png"), TextureType::PNG))
    //    model->m_texture = texture;
    //else
    //{
    //    delete model;
    //    delete texture;
    //    return nullptr;
    //}

    return model;
}

void Model::BindTexture() const
{
    const int32_t textureIdShader = m_shader->GetUniformLocation("u_textureSampler");

    // Bind our texture in Texture Unit 0 and set our
    // texture sampler to use Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glUniform1i(textureIdShader, 0);
}
