#include "Texture.h"

#include <fstream>
#include <array>
#include <optional>
#include <png.h>

#include "common/Define.h"
#include "io/png/PngFile.h"

Texture::Texture(): m_currentType(), m_width(0), m_height(0), m_colorType(0), m_data(nullptr) {}
Texture::~Texture() = default;

bool Texture::Load(std::string const& path, TextureType const& textureType)
{
    m_currentType = textureType;

    switch (textureType)
    {
        case TextureType::PNG:
        {
            const std::optional<PngInfo> pngInfo = PngFile::Load(path);
            if (!pngInfo)
                return false;

            m_width = pngInfo->m_width;
            m_height = pngInfo->m_height;
            m_colorType = pngInfo->m_colorType;
            m_data = pngInfo->m_data;
            break;
        }
        case TextureType::BMP:
        {
            FILE* file = nullptr;
            fopen_s(&file, path.c_str(), "rb");
            if (!file)
            {
                Log::Print("Failed to load texture: %s", path.c_str());
                return false;
            }

            std::array<uint8_t, 56> header{};
            if (fread(header.data(), 1, 54, file) != 54)
            {
                Log::Print("Not a correct bmp file: %s", path.c_str());
                return false;
            }

            if (header[0] != 'B' || header[1] != 'M')
            {
                Log::Print("Invalid texture file: %s", path.c_str());
                return false;
            }

            m_width = *reinterpret_cast<uint32_t*>(&header[0x12]);
            m_height = *reinterpret_cast<uint32_t*>(&header[0x16]);
            uint32_t imageSize = *reinterpret_cast<uint32_t*>(&header[0x22]);

            if (!imageSize)
                imageSize = m_width * m_height * 3;   ///< 3 : one byte for each Red, Green and Blue component

            m_data = new uint8_t[imageSize];
            fread(m_data, 1, imageSize, file);
            fclose(file);
            break;
        }
        case TextureType::BLP:
        case TextureType::DDS:
            return false;
    }

    return true;
}

uint32_t Texture::Bind() const
{
    uint32_t textureId;
    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);

    if (m_currentType == TextureType::PNG)
    {
        uint32_t alpha = GL_BGR;
        switch (m_colorType)
        {
            case PNG_COLOR_TYPE_RGBA:
                alpha = GL_RGBA;
                break;
            case PNG_COLOR_TYPE_RGB:
                alpha = GL_RGB;
                break;
            default:
                break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, alpha, GL_UNSIGNED_BYTE, m_data);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGR, GL_UNSIGNED_BYTE, m_data);

    delete[] m_data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    return textureId;
}
