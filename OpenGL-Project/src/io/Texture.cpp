#include "Texture.h"

#include <fstream>

#include "common/Define.h"

Texture::Texture(): m_header({}), m_dataPosition(0), m_width(0), m_height(0), m_imageSize(0), m_data(nullptr) {}
Texture::~Texture() = default;

bool Texture::Load(std::string const& path)
{
    FILE* file = nullptr;
    fopen_s(&file, path.c_str(), "rb");
    if (!file)
    {
        Log::Print("Failed to load texture: %s", path.c_str());
        return false;
    }

    if (fread(m_header.data(), 1, 54, file) != 54)
    {
        Log::Print("Not a correct bmp file: %s", path.c_str());
        return false;
    }

    if (m_header[0] != 'B' || m_header[1] != 'M')
    {
        Log::Print("Invalid texture file: %s", path.c_str());
        return false;
    }

    m_dataPosition  = *reinterpret_cast<uint32_t*>(&m_header[0x0A]);
    m_width         = *reinterpret_cast<int32_t*>(&m_header[0x12]);
    m_height        = *reinterpret_cast<int32_t*>(&m_header[0x16]);
    m_imageSize     = *reinterpret_cast<uint32_t*>(&m_header[0x22]);

    if (!m_imageSize)
        m_imageSize = m_width * m_height * 3;   ///< 3 : one byte for each Red, Green and Blue component
    if (!m_dataPosition)
        m_dataPosition = 54;                    ///< End of header

    m_data = new uint8_t[m_imageSize];
    fread(m_data, 1, m_imageSize, file);
    fclose(file);

    return true;
}

uint32_t Texture::Bind() const
{
    uint32_t textureId;
    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGR, GL_UNSIGNED_BYTE, m_data);

    delete[] m_data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    return textureId;
}
