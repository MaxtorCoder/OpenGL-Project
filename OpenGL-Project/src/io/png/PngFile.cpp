#include "PngFile.h"

#include <png.h>

std::optional<PngInfo> PngFile::Load(std::string const& path)
{
    auto closeMethod = [&](FILE* file, png_structp* png, png_infop* info) -> void
    {
        png_destroy_read_struct(png, info, nullptr);
        fclose(file);
    };

    FILE* file = fopen(path.c_str(), "rb");
    if (!file)
        return {};

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png)
        return {};

    png_infop info = png_create_info_struct(png);
    if (!info)
        return {};

    if (setjmp(png_jmpbuf(png)))
    {
        closeMethod(file, &png, &info);
        return {};
    }

    png_init_io(png, file);
    png_read_info(png, info);

    std::optional<PngInfo> pngInfo{};
    pngInfo.emplace();

    pngInfo->m_width = png_get_image_width(png, info);
    pngInfo->m_height = png_get_image_height(png, info);
    pngInfo->m_colorType = png_get_color_type(png, info);
    pngInfo->m_bitDepth = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt
    if (pngInfo->m_bitDepth == 16)
        png_set_strip_16(png);

    if (pngInfo->m_colorType == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth
    if (pngInfo->m_colorType == PNG_COLOR_TYPE_GRAY && pngInfo->m_bitDepth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if (pngInfo->m_colorType == PNG_COLOR_TYPE_RGB ||
        pngInfo->m_colorType == PNG_COLOR_TYPE_GRAY ||
        pngInfo->m_colorType == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if (pngInfo->m_colorType == PNG_COLOR_TYPE_GRAY ||
        pngInfo->m_colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    const std::size_t rowBytes = png_get_rowbytes(png, info);
    pngInfo->m_data = static_cast<uint8_t*>(malloc(rowBytes * pngInfo->m_height * sizeof(uint8_t)));
    if (!pngInfo->m_data)
    {
        closeMethod(file, &png, &info);
        free(pngInfo->m_data);
        return {};
    }

    uint8_t** rowPointers = static_cast<uint8_t**>(malloc(pngInfo->m_height * sizeof(uint8_t*)));
    if (!rowPointers)
    {
        closeMethod(file, &png, &info);
        free(pngInfo->m_data);
        return {};
    }

    for (uint32_t i = 0; i < pngInfo->m_height; ++i)
        rowPointers[pngInfo->m_height - 1 - i] = pngInfo->m_data + i * rowBytes;

    png_read_image(png, rowPointers);

    closeMethod(file, &png, &info);
    free(rowPointers);

    return pngInfo;
}
