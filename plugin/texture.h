#pragma once
#include "../common/stdinc.h"
#include "grcTexturePC.h"

namespace Texture
{
    //读取RGBA PNG图像
    std::tuple<uint, uint, std::vector<uchar>> read_png(const std::filesystem::path &filename);
    void convert_texture(const std::tuple<uint, uint, std::vector<uchar>> &image, grcTexturePC &texture);
    void read_png_as_texture(const std::filesystem::path &filename, grcTexturePC &texture);
}
