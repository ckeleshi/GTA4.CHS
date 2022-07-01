#include "texture_read.h"

grcTexturePC read_png_as_texture(const std::filesystem::path &filename)
{
    std::tuple<uint32, uint32, std::vector<uchar>> result;
    std::vector<png_bytep> row_pointers;
    std::vector<uchar> raw_pixels;

    auto fp = std::fopen(filename.string().c_str(), "rb");

    if (!fp)
    {
        return {};
    }

    auto psp = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

    if (setjmp(png_jmpbuf(psp)))
    {
        png_destroy_read_struct(&psp, nullptr, nullptr);
        std::fclose(fp);
        return {};
    }

    auto pip = png_create_info_struct(psp);
    png_init_io(psp, fp);
    png_read_info(psp, pip);

    auto w = png_get_image_width(psp, pip);
    auto h = png_get_image_height(psp, pip);
    auto stride = png_get_rowbytes(psp, pip);
    assert(png_get_channels(psp, pip) == 4);

    raw_pixels.resize(h * stride);
    row_pointers.resize(h);

    for (uint i = 0; i < h; i++)
    {
        row_pointers[i] = &raw_pixels[stride * i];
    }

    png_read_image(psp, row_pointers.data());

    png_read_end(psp, pip);
    png_destroy_read_struct(&psp, nullptr, nullptr);
    std::fclose(fp);

    std::get<0>(result) = w;
    std::get<1>(result) = h;
    auto &compressed_pixels = std::get<2>(result);

    compressed_pixels.resize(squish::GetStorageRequirements(w, h, squish::kDxt5));
    squish::CompressImage(raw_pixels.data(), w, h, compressed_pixels.data(), squish::kDxt5);

    return result;
}
