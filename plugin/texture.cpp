#include "texture.h"

namespace Texture
{
    STextureData read_png(const std::filesystem::path &filename)
    {
        STextureData result;

        try
        {
            png::image<png::rgba_pixel> image(filename.string());

            result.width = image.get_width();
            result.height = image.get_height();
            auto row_bytes = result.width * 4;
            result.pixels.resize(result.height * row_bytes);

            for (uint row = 0; row < result.height; ++row)
            {
                std::memcpy(result.pixels.data() + row * row_bytes, image.get_row(row).data(), row_bytes);
            }
        }
        catch (std::exception& e)
        {

        }

        return result;
    }

    void convert_texture(const STextureData &rgba32_texture, grcTexturePC &texture)
    {
        auto width = rgba32_texture.width;
        auto height = rgba32_texture.height;
        auto data = rgba32_texture.pixels.data();

        if (width && height && data)
        {
            if (texture.m_pPixelData)
                delete[] texture.m_pPixelData;

            texture.m_wWidth = width;
            texture.m_wHeight = height;
            texture.m_wStride = width; // DXT5是这样
            texture.m_pPixelData = new uchar[squish::GetStorageRequirements(width, height, squish::kDxt5)];
            squish::CompressImage(data, width, height, texture.m_pPixelData, squish::kDxt5);
        }
    }

    void read_png_as_texture(const std::filesystem::path &filename, grcTexturePC &texture)
    {
        convert_texture(read_png(filename), texture);
    }
}
