#ifndef TEXTURE_H__
#define TEXTURE_H__

#include <array>
#include <string>

class Texture
{
public:
    Texture();
    ~Texture();

    bool Load(std::string const& path);
    uint32_t Bind() const;

private:
    // BMP, replace
    std::array<uint8_t, 54> m_header;
    uint32_t m_dataPosition;
    int32_t m_width;
    int32_t m_height;
    uint32_t m_imageSize;
    uint8_t* m_data;
};

#endif // TEXTURE_H__
