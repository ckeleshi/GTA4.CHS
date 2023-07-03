#pragma once
#include "../common/stdinc.h"

struct CHtmlNode;

class CHtmlTextExport
{
  public:
    typedef std::filesystem::path path_type;

    struct exported_text_entry
    {
        uint hash;
        std::string ori, str;
    };

    void ExportHtml(const path_type &input_folder); //加载文件夹中所有whm文件，在原地生成json文本
    void GenerateDataBase(const path_type &input_folder, const path_type &output_file);

    void ConvertJson(const path_type &input_folder);    //加载文件夹中所有txt文本，在原地生成json文本

  private:
    std::vector<std::vector<std::string>> m_Texts;

    static std::vector<uchar> UnpackWhm(const path_type &input_file); //解包whm
    static void ExtractNodeStrings(CHtmlNode *node, std::vector<uchar> &block,
                                   std::vector<exported_text_entry> &container,
                                   std::set<std::size_t> &hashes); //收集Node及其子Node的字符串
    static void TryAppendString(std::vector<exported_text_entry> &container, char *ptr,
                                std::set<std::size_t> &hashes); //排除没用的字符串
    static std::vector<exported_text_entry> ExtractWhmStrings(const std::filesystem::path &filename,
                                                              std::set<std::size_t> &hashes); //收集whm中所有字符串
    static std::string Windows1252ToUtf8(const std::string &str);                             //转码
    static void ExportJson(const std::filesystem::path &filename,
                           const std::vector<exported_text_entry> &container); //将whm文本输出到txt

    static bool IsBlankText(const std::string &str);
    static std::vector<exported_text_entry> LoadText(const std::filesystem::path &filename);
    static std::vector<exported_text_entry> LoadJson(const std::filesystem::path &filename);
};
