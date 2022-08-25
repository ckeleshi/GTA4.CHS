#include "../common/stdinc.h"

void write_dxt5(const tPath &filename, const std::vector<uchar> &pixels, uint width, uint height)
{
    BinaryFile bf(filename, "wb");

    bf.Write(width).Write(height).WriteArray(pixels);
}

void compress_dxt5(const std::vector<uchar> &pixels, std::vector<uchar> &compressed, uint width, uint height)
{
    compressed.resize(squish::GetStorageRequirements(width, height, squish::kDxt5));
    squish::CompressImage(pixels.data(), width, height, compressed.data(), squish::kDxt5);
}

bool read_png(const tPath &filename, std::vector<uchar> &pixels, uint &width, uint &height)
{
    //要求4通道RGBA图
}

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        std::vector<uchar> pixels, compressed;
        uint width, height;

        tPath in_p = argv[1];

        if (in_p.extension() != ".png")
            return 0;

        auto out_p = in_p.replace_extension(".dxt5");

        if (read_png(in_p, pixels, width, height))
        {
            compress_dxt5(pixels, compressed, width, height);
            write_dxt5(out_p, compressed, width, height);
        }
    }
    else
    {
        fmt::printf("请提供一个png图片");
    }

    return 0;
}
