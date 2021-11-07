#ifndef MODEL_H__
#define MODEL_H__

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Shader.h"
#include "Texture.h"

class Model
{
public:
    ~Model();

    static Model* Load(std::string const& filename);

    static bool LoadOBJ(std::string const& path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals);
    void BindTexture() const;

public:
    std::vector<glm::vec3> m_vertices{};
    std::vector<glm::vec3> m_normals{};
    std::vector<glm::vec2> m_uvs{};

    Shader* m_shader = nullptr;
    Texture* m_texture = nullptr;
    uint32_t m_textureId = 0;
};

#endif // MODEL_H__
