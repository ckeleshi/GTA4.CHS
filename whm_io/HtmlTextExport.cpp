#include "HtmlTextExport.h"
#include "../common/fnv_hash.h"
#include "HTMLDocument.h"
#include "rsc_header.h"
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/reader.h>
#include <rapidjson/stringbuffer.h>

void CHtmlTextExport::ExportHtml(const path_type &input_folder)
{
    std::set<std::size_t> hashes;

    std::filesystem::recursive_directory_iterator it{input_folder};

    for (; it != std::filesystem::recursive_directory_iterator{}; ++it)
    {
        if (!it->is_regular_file())
            continue;

        auto filename = it->path();

        if (filename.extension() == ".whm")
        {
            ExportJson(filename.replace_extension(".json"), ExtractWhmStrings(it->path(), hashes));
        }
    }
}

void CHtmlTextExport::GenerateDataBase(const path_type &input_folder, const path_type &output_file)
{
    std::vector<exported_text_entry> texts;

    std::filesystem::recursive_directory_iterator it{input_folder};

    for (; it != std::filesystem::recursive_directory_iterator{}; ++it)
    {
        if (!it->is_regular_file())
        {
            continue;
        }

        auto filename = it->path();

        if (filename.extension() == ".json")
        {
            for (auto &entry : LoadJson(filename))
            {
                texts.push_back(std::move(entry));
            }
        }
    }

    std::vector<WhmTextData> text_table;
    std::vector<char> text_data;

    for (auto &entry : texts)
    {
        //忽略没翻译的文本
        if (entry.hash == fnv_hash::hash_string(entry.str, false))
        {
            continue;
        }

        WhmTextData bin_entry;

        bin_entry.hash = entry.hash;
        bin_entry.offset = text_data.size();

        ranges::copy(entry.str, std::back_inserter(text_data));
        text_data.push_back(0);

        text_table.push_back(bin_entry);
    }

    BinaryFile out(output_file, "wb");

    if (!out)
    {
        fmt::printf("创建输出文件 %s 失败。\n", output_file.string());
        return;
    }

    out.WriteArray2(std::span{text_table});
    out.WriteArray2(std::span{text_data});
    out.Close();
}

void CHtmlTextExport::ConvertJson(const path_type &input_folder)
{
    std::filesystem::recursive_directory_iterator it{input_folder};

    for (; it != std::filesystem::recursive_directory_iterator{}; ++it)
    {
        if (!it->is_regular_file())
        {
            continue;
        }

        auto filename = it->path();

        if (filename.extension() == ".txt")
        {
            ExportJson(filename.replace_extension(".json"), LoadText(filename));
        }
    }
}

void CHtmlTextExport::ExportJson(const std::filesystem::path &filename,
                                 const std::vector<exported_text_entry> &container)
{
    if (container.empty())
    {
        return;
    }

    auto out = std::fopen(filename.string().c_str(), "w");

    if (!out)
    {
        return;
    }

    rapidjson::StringBuffer buffer(nullptr, 1024 * 1024 * 10); // 10MB
    rapidjson::PrettyWriter writer(buffer);

    buffer.Clear();

    writer.StartArray();

    for (auto &entry : container)
    {
        writer.StartObject();
        writer.Key("hash");
        writer.Uint(entry.hash);
        writer.Key("original");
        writer.String(entry.ori.c_str());
        writer.Key("translated");
        writer.String(entry.str.c_str());
        writer.Key("desc");
        writer.String("");
        writer.EndObject();
    }

    writer.EndArray();

    fwrite(buffer.GetString(), buffer.GetSize(), 1, out);
    fclose(out);
}

bool CHtmlTextExport::IsBlankText(const std::string &str)
{
    return ranges::all_of(str, [](char chr) {
        // TODO: 全角空格之类更加特殊的字符判断
        return chr == ' ' || chr == '\t';
    });
}

std::vector<CHtmlTextExport::exported_text_entry> CHtmlTextExport::LoadText(const std::filesystem::path &filename)
{
    std::vector<exported_text_entry> result;

    std::ifstream stream(filename);
    std::string line;
    std::size_t line_no = 0;
    std::smatch matches;

    std::regex entry_regex(R"((0[xX][0-9a-fA-F]{8})=(.*))");

    if (!stream)
    {
        fmt::printf("打开输入文件 %s 失败。\n", filename.string());
        return result;
    }

    SkipUTF8Signature(stream);

    while (std::getline(stream, line))
    {
        bool original = false;

        ++line_no;

        line.erase(0, line.find_first_not_of(' '));
        line.erase(line.find_last_not_of(' ') + 1);

        if (line.empty())
        {
            continue;
        }

        if (line.front() == ';')
        {
            original = true;
            line.erase(line.begin());
        }

        if (std::regex_match(line, matches, entry_regex))
        {
            auto hash = std::stoul(matches.str(1), nullptr, 16);
            auto str = matches.str(2);

            //忽略空白文本
            if (IsBlankText(str))
            {
                continue;
            }

            if (result.empty() || result.back().hash != hash)
            {
                result.emplace_back();
                result.back().hash = hash;
            }

            if (original)
            {
                result.back().ori = str;
            }
            else
            {
                result.back().str = str;
            }
        }
        else
        {
            fmt::printf("%s: 第%u行无法识别。\n", filename.string(), line_no);
        }
    }

    return result;
}

std::vector<CHtmlTextExport::exported_text_entry> CHtmlTextExport::LoadJson(const std::filesystem::path &filename)
{
    std::vector<CHtmlTextExport::exported_text_entry> result;
    rapidjson::Document doc;

    std::ifstream stream(filename);

    if (!stream)
    {
        fmt::printf("打开文件失败。\n");
        return result;
    }

    std::string json(std::istreambuf_iterator<char>{stream}, std::istreambuf_iterator<char>{});

    if (doc.Parse(json.c_str()).HasParseError())
    {
        fmt::printf("json解析失败。\n");
        return result;
    }

    if (!doc.IsArray())
    {
        fmt::printf("json类型错误。\n");
        return result;
    }

    auto text_entries_array = doc.GetArray();

    for (auto &entry : text_entries_array)
    {
        auto obj = entry.GetObj();

        result.emplace_back(
            exported_text_entry{obj["hash"].GetUint(), obj["original"].GetString(), obj["translated"].GetString()});
    }

    return result;
}

std::vector<uchar> CHtmlTextExport::UnpackWhm(const path_type &input_file)
{
    BinaryFile file;
    std::vector<uchar> uncompressed_bytes;

    file.Open(input_file, "rb");

    if (file)
    {
        std::vector<uchar> compressed_bytes;
        rsc_header header;

        file.Seek(0, SEEK_END);
        auto compressed_size = file.Tell() - sizeof(header);
        compressed_bytes.resize(compressed_size);
        file.Seek(0, SEEK_SET);
        file.Read(header);

        auto virtual_size = header.flags.GetVirtualSize();
        auto physical_size = header.flags.GetPhysicalSize();
        uLongf uncompressed_size = virtual_size + physical_size;
        uncompressed_bytes.resize(uncompressed_size);
        file.Read(compressed_bytes.data(), compressed_size);

        uncompress(uncompressed_bytes.data(), &uncompressed_size, compressed_bytes.data(), compressed_size);
    }

    return uncompressed_bytes;
}

void CHtmlTextExport::ExtractNodeStrings(CHtmlNode *node, std::vector<uchar> &block,
                                         std::vector<exported_text_entry> &container, std::set<std::size_t> &hashes)
{
    if (node == nullptr)
        return;

    for (auto &element : node->m_children.get_span(block))
    {
        ExtractNodeStrings(element.locate(block), block, container, hashes);
    }

    switch (node->m_eNodeType)
    {
    case Node_HtmlNode:
    case Node_HtmlTableNode:
    case Node_HtmlTableElementNode: {
        break;
    }

    case Node_HtmlDataNode: {
        auto data_node = static_cast<CHtmlDataNode *>(node);

        TryAppendString(container, data_node->m_pData.locate(block), hashes);

        break;
    }

    default: {
        throw std::invalid_argument("Unknown node type.");
    }
    }
}

void CHtmlTextExport::TryAppendString(std::vector<exported_text_entry> &container, char *ptr,
                                      std::set<std::size_t> &hashes)
{
    //合法的文本类型:
    // 1) 包含EFIGS字母
    // 2) 非网址

    //判断一个字符是不是数字
    auto validate_digit_char = [](char chr) {
        auto ucr = *reinterpret_cast<uchar *>(&chr);

        return (ucr >= '0' && ucr <= '9');
    };

    //判断一个字符是不是英语字母
    auto validate_english_char = [](char chr) {
        auto ucr = *reinterpret_cast<uchar *>(&chr);

        return (ucr >= 'a' && ucr <= 'z') || (ucr >= 'A' && ucr <= 'Z');
    };

    //判断一个字符是不是EFIGS字母
    auto validate_efigs_char = [validate_english_char](char chr) {
        auto ucr = *reinterpret_cast<uchar *>(&chr);

        return validate_english_char(chr) || (ucr >= 0xC0);
    };

    //判断字符是不是游戏中url会出现的字符
    auto validate_url_char = [validate_digit_char, validate_english_char](char chr) {
        auto ucr = *reinterpret_cast<uchar *>(&chr);

        return validate_english_char(chr) || validate_digit_char(chr) || ucr == '.' || ucr == '%' || ucr == '@' ||
               ucr == '-' || ucr == '_';
    };

    //判断字符串是否为url
    auto validate_url = [validate_url_char](std::string_view view) {
        auto validate_char_result = ranges::all_of(view, validate_url_char);
        auto first_dot_pos = view.find('.');
        auto last_dot_pos = view.find_last_of('.');

        return !view.empty() &&                           //非空
               validate_char_result &&                    //所有字符合法
               first_dot_pos != std::string_view::npos && //最少包含一个'.'
               first_dot_pos != 0 &&                      //第一个字符不是'.'
               last_dot_pos != view.length() - 1;         //最后一个字符不是'.'
    };

    auto validate_string = [validate_efigs_char, validate_url](std::string_view view) {
        //包含EFIGS字母且不是网址
        return ranges::any_of(view, validate_efigs_char) && !validate_url(view);
    };

    if (ptr == nullptr)
    {
        return;
    }

    if (validate_string(std::string_view(ptr)))
    {
        auto hash = fnv_hash::hash_string(ptr, false);

        if (hashes.insert(hash).second)
        {
            container.emplace_back(exported_text_entry{hash, ptr});
        }
    }
}

std::vector<CHtmlTextExport::exported_text_entry> CHtmlTextExport::ExtractWhmStrings(
    const std::filesystem::path &filename, std::set<std::size_t> &hashes)
{
    std::vector<exported_text_entry> container;

    auto block = UnpackWhm(filename);

    if (!block.empty())
    {
        auto *p_doc = reinterpret_cast<CHtmlDocument *>(block.data());

        ExtractNodeStrings(p_doc->m_pBody.locate(block), block, container, hashes);
    }

    return container;
}

std::string CHtmlTextExport::Windows1252ToUtf8(const std::string &str)
{
    std::string result;

    for (auto chr : str)
    {
        utf8::append(*reinterpret_cast<uchar *>(&chr), std::back_inserter(result));
    }

    return result;
}
