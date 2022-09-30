#include "../common/stdinc.h"
#include "IVText.h"

int main(int argc, char **argv)
{
    IVText instance;
    bool error = false;

    if (argc == 4)
    {
        if (std::strcmp(argv[1], "-b2t") == 0)
        {
            instance.ProcessB2T(argv[2], argv[3]);
        }
        else if (std::strcmp(argv[1], "-b2j") == 0)
        {
            instance.ProcessB2J(argv[2], argv[3]);
        }
        else if (std::strcmp(argv[1], "-t2b") == 0)
        {
            instance.ProcessT2B(argv[2], argv[3]);
        }
        else if (std::strcmp(argv[1], "-j2b") == 0)
        {
            instance.ProcessJ2B(argv[2], argv[3]);
        }
        else if (std::strcmp(argv[1], "-collect") == 0)
        {
            instance.ProcessCollect(argv[2], argv[3]);
        }
        else
        {
            error = true;
        }
    }
    else if (argc == 3)
    {
        if (std::strcmp(argv[1], "-t2j") == 0)
        {
            instance.ProcessT2J(argv[2]);
        }
        else
        {
            error = true;
        }
    }
    else
    {
        error = true;
    }

    if (error)
    {
        fmt::printf("使用方法:\n"
                    "gxt转txt: gxt_tool.exe -b2t [gxt文件] [txt文件夹]\n"
                    "gxt转json: gxt_tool.exe -b2j [gxt文件] [json文件夹]\n"
                    "txt转gxt: gxt_tool.exe -t2b [txt文件夹] [gxt文件夹]\n"
                    "json转gxt: gxt_tool.exe -j2b [json文件夹] [gxt文件夹]\n"
                    "txt转json: gxt_tool.exe -t2j [txt文件夹]\n"
                    "生成字库: gxt_tool.exe -collect [(任意文本格式)文件夹] [输出文件夹]\n");
    }

    return 0;
}
