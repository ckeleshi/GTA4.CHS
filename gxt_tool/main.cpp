#include "../common/stdinc.h"
#include "IVText.h"

int main(int argc, char** argv)
{
    //GXTTool -b2t american.gxt D:/text
    //GXTTool -t2b D:/text D:/binary
    //GXTTool -collect D:/text D:/collection

    IVText instance;

    if (argc == 4)
    {
        if (std::strcmp(argv[1],"-b2t") == 0)
        {
            instance.ProcessB2T(argv[2], argv[3]);
        }
        else if (std::strcmp(argv[1], "-t2b") == 0)
        {
            instance.ProcessT2B(argv[2], argv[3]);
        }
        else if (std::strcmp(argv[1], "-collect") == 0)
        {
            instance.ProcessCollect(argv[2], argv[3]);
        }
    }
    else
    {
        std::printf("打开方式不对。\n");
    }

    return 0;
}
