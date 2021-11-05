#ifndef MODEL_H__
#define MODEL_H__

#include <vector>

#include "Texture.h"

class Model
{
private:
    std::vector<uint16_t> vertices{};
    std::vector<uint16_t> normals{};

    Texture m_texture{};
};

#endif // MODEL_H__
