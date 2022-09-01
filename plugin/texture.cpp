#include "texture.h"

namespace Texture
{
    std::tuple<uint, uint, std::vector<uchar>> read_png(const std::filesystem::path &filename)
    {
        std::tuple<uint, uint, std::vector<uchar>> result;
        auto f = std::fopen(filename.string().c_str(), "rb");
        png_structp png_ptr = nullptr;
        png_infop info_ptr = nullptr;

        do
        {
            if (f == nullptr)
                break;

            uchar sig[8];

            fread(sig, 1, 8, f);
            if (!png_check_sig(sig, 8))
                break;

            png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
            if (!png_ptr)
                return result;

            info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr)
            {
                png_destroy_read_struct(&png_ptr, nullptr, nullptr);
                return result;
            }

            if (setjmp(png_jmpbuf(png_ptr)))
                break;

            png_init_io(png_ptr, f);
            png_set_sig_bytes(png_ptr, 8);
            png_read_info(png_ptr, info_ptr);

            uint width, height;
            int bit_depth, color_type;
            png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
                         &color_type, NULL, NULL, NULL);

            if (bit_depth != 8 || color_type != PNG_COLOR_TYPE_RGB_ALPHA)
                break;

            std::vector<uchar *> row_ptrs;
            std::vector<uchar> pixels;

            row_ptrs.resize(height);
            pixels.resize(width * height * 4);

            for (uint row = 0; row < height; ++row)
            {
                row_ptrs[row] = pixels.data() + row * width * 4;
            }

            png_read_image(png_ptr, row_ptrs.data());
            png_read_end(png_ptr, nullptr);

            std::get<0>(result) = width;
            std::get<1>(result) = height;
            pixels.swap(std::get<2>(result));

        } while (0);

        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

        if (f)
            std::fclose(f);

        return result;
    }

    void convert_texture(const std::tuple<uint, uint, std::vector<uchar>> &image, grcTexturePC &texture)
    {
        auto width = std::get<0>(image);
        auto height = std::get<1>(image);
        auto data = std::get<2>(image).data();

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