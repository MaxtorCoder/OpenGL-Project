#ifndef MODEL_H__
#define MODEL_H__

#include <vector>

#include "Shader.h"
#include "Texture.h"

class Model
{
public:
    ~Model();

    static Model* Load(std::string const& filename);

    void BindTexture();

public:
    std::vector<float> m_vertices{};
    std::vector<float> m_uvs{};

    Shader* m_shader = nullptr;
    Texture* m_texture = nullptr;
    uint32_t m_textureId = 0;
};

#endif // MODEL_H__
