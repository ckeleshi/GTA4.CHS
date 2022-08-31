#pragma once
#include "../common/stdinc.h"

//读取RGBA PNG图像
std::tuple<uint, uint, std::vector<uchar>> read_png(const std::filesystem::path &filename);
