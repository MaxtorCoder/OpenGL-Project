#ifndef PNG_FILE_H__
#define PNG_FILE_H__

#include <string>
#include <optional>

struct PngInfo
{
    uint32_t m_width;
    uint32_t m_height;
    uint8_t m_colorType;
    uint8_t m_bitDepth;
    uint8_t* m_data;
};

class PngFile
{
public:
    static std::optional<PngInfo> Load(std::string const& path);
};

#endif // PNG_FILE_H__
