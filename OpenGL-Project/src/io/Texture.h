#ifndef TEXTURE_H__
#define TEXTURE_H__

#include <string>

enum class TextureType : uint32_t
{
    BMP,
    BLP,
    DDS,
    PNG,
};

class Texture
{
public:
    Texture();
    ~Texture();

    bool Load(std::string const& path, TextureType const& textureType);
    [[nodiscard]] uint32_t Bind() const;

private:
    TextureType m_currentType;
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_colorType;   // Used for PNG
    uint8_t* m_data;
};

#endif // TEXTURE_H__
