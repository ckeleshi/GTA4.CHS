#include "HtmlTextExport.h"

int main(int argc, char *argv[])
{
    CHtmlTextExport exporter;

    bool error = false;

    if (argc == 4)
    {
        if (std::strcmp(argv[1], "-gendb") == 0)
        {
            //读取json文本并生成汉化补丁用数据库
            CHtmlTextExport::path_type p1 = argv[2];
            CHtmlTextExport::path_type p2 = argv[3];

            exporter.GenerateDataBase(p1, p2);
        }
        else
        {
            error = true;
        }
    }
    else if (argc == 3)
    {
        CHtmlTextExport::path_type p1 = argv[2];

        if (std::strcmp(argv[1], "-export") == 0)
        {
            //读取所有whm文件并在原位生成json
            exporter.ExportHtml(p1);
        }
        else if (std::strcmp(argv[1], "-convert") == 0)
        {
            //读取txt并在原位生成json文本
            exporter.ConvertJson(p1);
        }
        else
        {
            error = true;
        }
    }
    else
    {
        exporter.ConvertJson("D:/text/html/");
    }

    if (error)
    {
        fmt::printf("使用方法:\n"
                    "生成汉化文件: whm_io.exe -gendb [json文件夹] [输出文件文件]\n"
                    "导出whm文本: whm_io.exe -export [whm文件夹]\n"
                    "txt转json: whm_io.exe -convert [txt文件夹]\n");
    }

    return 0;
}
