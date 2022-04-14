#include "HtmlTextExport.h"

int main(int argc, char* argv[])
{
    CHtmlTextExport exporter;

    if (argc == 3)
    {
        //读取翻译后文本并生成汉化补丁用数据库
        CHtmlTextExport::path_type p1 = argv[1];
        CHtmlTextExport::path_type p2 = argv[2];

        exporter.GenerateDataBase(p1, p2);

    }
    else if (argc == 2)
    {
        //读取whm文件并在原位生成输出文本
        CHtmlTextExport::path_type p1 = argv[1];

        exporter.ExportHtml(p1);
    }

    return 0;
}
