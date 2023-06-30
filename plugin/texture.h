#pragma once
#include "../common/stdinc.h"
#include "grcTexturePC.h"

namespace Texture
{
struct STextureData
{
    uint width = 0, height = 0;
    std::vector<uchar> pixels;
};

// 读取RGBA PNG图像
STextureData read_png(const std::filesystem::path &filename);
void convert_texture(const STextureData &rgba32_texture, grcTexturePC &texture);
void read_png_as_texture(const std::filesystem::path &filename, grcTexturePC &texture);
} // namespace Texture
