#pragma once
#include "grcTexturePC.h"

std::tuple<uint, uint, std::vector<uchar>> read_dxt5(const std::filesystem::path &filename);
