#include "texture_read.h"

std::tuple<uint, uint, std::vector<uchar>> read_dxt5(const std::filesystem::path &filename)
{
    std::tuple<uint, uint, std::vector<uchar>> result;

    BinaryFile bf(filename, "rb");

    bf.Read(std::get<0>(result)).Read(std::get<1>(result));
    bf.ReadArray(std::get<0>(result) * std::get<1>(result), std::get<2>(result));

    return result;
}
