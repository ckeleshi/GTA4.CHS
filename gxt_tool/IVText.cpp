#include "IVText.h"
#include "../common/char8_t-remediation.h"
#include "../common/fnv_hash.h"
#include "../common/module_path.h"


void IVText::ProcessT2B(const PathType &inFolder, const PathType &outFolder)
{
    create_directories(outFolder);
    LoadTexts(inFolder);
    GenerateBinary(outFolder / "chinese.gxt");
}

void IVText::ProcessB2T(const PathType &inFile, const PathType &outFolder)
{
    create_directories(outFolder);
    LoadBinary(inFile);
    GenerateTexts(outFolder);
}

void IVText::ProcessCollect(const PathType &inFolder, const PathType &outFolder)
{
    std::set<char32_t> chars; //文本中出现的非原版字符集合

    create_directories(outFolder);

    ProcessTexts(inFolder, true,
                 [&chars](const std::string &filename, std::ifstream &stream) { CollectCharsFunc(stream, chars); });

    GenerateCollection(outFolder / "characters.txt", chars);
    GenerateTable(outFolder / "char_table.dat", chars);
}

bool IVText::IsNativeCharacter(char32_t character)
{
    if (character > 0xFFFF)
    {
        std::printf("Invalid code point: %u larger than 0xFFFF.\n", static_cast<unsigned>(character));
        return false;
    }

    return (character < 0x100 || character == L'™');
}

void IVText::ProcessTexts(const PathType &inFolder, bool recursive, const FuncType &func)
{
    std::vector<PathType> filenames;

    if (recursive)
    {
        std::filesystem::recursive_directory_iterator dir_it{inFolder};

        while (dir_it != std::filesystem::recursive_directory_iterator{})
        {
            PathType path = dir_it->path();

            if (path.extension() == ".txt")
            {
                filenames.push_back(path);
            }

            ++dir_it;
        }
    }
    else
    {
        std::filesystem::directory_iterator dir_it{inFolder};

        while (dir_it != std::filesystem::directory_iterator{})
        {
            PathType path = dir_it->path();

            if (path.extension() == ".txt")
            {
                filenames.push_back(path);
            }

            ++dir_it;
        }
    }

    for (auto &filename : filenames)
    {
        std::ifstream ifs(filename);

        if (!ifs)
        {
            continue;
        }

        func(filename.string(), ifs);
    }
}

void IVText::LoadTextFunc(const std::string &filename, std::ifstream &stream)
{
    std::regex table_regex(R"(\[([0-9a-zA-Z_]{1,7})\])");
    std::regex entry_regex(R"((0[xX][0-9a-fA-F]{8})=(.*))");

    std::match_results<const char *> matches;

    std::string line;

    auto table_iter = m_Data.end();

    size_t line_no = 0;

    SkipUTF8Signature(stream);

    while (std::getline(stream, line))
    {
        ++line_no;

        line.erase(0, line.find_first_not_of(' '));
        line.erase(line.find_last_not_of(' ') + 1);

        if (line.empty())
        {
            continue;
        }

        if (line.front() == '[' && regex_match(line.c_str(), matches, table_regex))
        {
            table_iter = m_Data.emplace(matches.str(1), std::vector<TextEntry>()).first;
            continue;
        }

        bool is_original = (line.front() == ';');
        bool match_result;

        if (is_original)
        {
            match_result = std::regex_match(line.c_str() + 1, matches, entry_regex);
        }
        else
        {
            match_result = std::regex_match(line.c_str(), matches, entry_regex);
        }

        if (match_result)
        {
            if (table_iter != m_Data.end())
            {
                auto &table_cont = table_iter->second;
                auto hash_string = matches.str(1);

                if (table_cont.empty() || table_cont.back().hash_string != hash_string)
                {
                    table_cont.emplace_back();
                }

                auto p_entry = &table_cont.back();

                p_entry->hash_string = hash_string;

                auto b_string = matches.str(2);

                if (is_original)
                {
                    p_entry->original = b_string;
                }
                else
                {
                    p_entry->translated = b_string;
                }

                if (!is_original && (ranges::count(b_string, '~') & 1) == 1)
                {
                    std::printf(U8("%s: 第%u行的\'~\'个数不是偶数!\n"), filename.c_str(), line_no);
                }
            }
            else
            {
                std::printf(U8("%s: 第%u行没有所属的表。\n"), filename.c_str(), line_no);
            }
        }
        else
        {
            std::printf(U8("%s: 第%u行无法识别。\n"), filename.c_str(), line_no);
        }
    }
}

void IVText::CollectCharsFunc(std::ifstream &stream, std::set<char32_t> &chars)
{
    std::vector<char> u8_buffer;
    std::vector<char32_t> u32_buffer;

    u8_buffer.assign(std::istreambuf_iterator(stream), std::istreambuf_iterator<char>());
    utf8::utf8to32(u8_buffer.begin(), u8_buffer.end(), std::back_inserter(u32_buffer));

    for (auto chr : u32_buffer)
    {
        if (!IsNativeCharacter(chr) && chr != 0x3000 && chr != 0xFEFF)
        {
            chars.insert(chr);
        }
    }
}

void IVText::LoadTexts(const PathType &inFolder)
{
    ProcessTexts(inFolder, false,
                 [this](const std::string &filename, std::ifstream &stream) { LoadTextFunc(filename, stream); });
}

void IVText::GenerateBinary(const PathType &output_binary) const
{
    BinaryFile file(output_binary, "wb");

    std::vector<uint8_t> buffer;
    long writePostion;

    GXTHeader gxtHeader;

    TableBlock tableBlock;
    KeyBlockOthers keyBlock;

    DataBlock dataBlock;

    TableEntry tableEntry;
    KeyEntry keyEntry;

    std::vector<TableEntry> tables;
    std::vector<KeyEntry> keys;
    std::vector<uint16_t> datas;

    if (!file)
    {
        std::printf(U8("创建输出文件 %s 失败。\n"), output_binary.string().c_str());
        return;
    }

    gxtHeader.Version = 4;
    gxtHeader.CharBits = 16;
    file.Write(gxtHeader);

    tableBlock.TABL[0] = 'T';
    tableBlock.TABL[1] = 'A';
    tableBlock.TABL[2] = 'B';
    tableBlock.TABL[3] = 'L';

    keyBlock.Body.TKEY[0] = 'T';
    keyBlock.Body.TKEY[1] = 'K';
    keyBlock.Body.TKEY[2] = 'E';
    keyBlock.Body.TKEY[3] = 'Y';

    dataBlock.TDAT[0] = 'T';
    dataBlock.TDAT[1] = 'D';
    dataBlock.TDAT[2] = 'A';
    dataBlock.TDAT[3] = 'T';

    tableBlock.Size = m_Data.size() * sizeof(TableEntry);
    file.Write(tableBlock);

    writePostion = 4 + 8 + tableBlock.Size;

    tables.clear();

    for (auto &table : m_Data)
    {
        keys.clear();
        datas.clear();

        strcpy(tableEntry.Name, table.first.c_str());
        tableEntry.Offset = static_cast<int>(writePostion);
        tables.push_back(tableEntry);

        strcpy(keyBlock.Name, table.first.c_str());
        keyBlock.Body.Size = table.second.size() * sizeof(KeyEntry);

        for (auto &entry : table.second)
        {
            if (entry.hash_string.empty() || entry.original.empty() || entry.translated.empty())
            {
                std::printf(U8("遇到缺失的文本项:\nhash_string: %s\noriginal: %s\ntranslated: %s\n\n"),
                            entry.hash_string.c_str(), entry.original.c_str(), entry.translated.c_str());
            }

            if (!CompareTokens(entry.original, entry.translated))
            {
                std::printf(U8("遇到Token与原文不一致的译文:\nhash_string: %s\noriginal: %s\ntranslated: %s\n\n"),
                            entry.hash_string.c_str(), entry.original.c_str(), entry.translated.c_str());
            }

            keyEntry.Hash = std::stoul(entry.hash_string, nullptr, 16);
            keyEntry.Offset = static_cast<std::int32_t>(datas.size() * 2);

            auto w_string_to_write = U8ToWide(entry.translated);
            ranges::copy(w_string_to_write, std::back_inserter(datas));

            keys.push_back(keyEntry);
        }

        dataBlock.Size = static_cast<std::int32_t>(datas.size() * 2);

        file.Seek(writePostion, SEEK_SET);

        if (table.first == "MAIN")
        {
            file.Write(keyBlock.Body);
        }
        else
        {
            file.Write(keyBlock);
        }

        file.WriteArray(keys);
        file.Write(dataBlock);
        file.WriteArray(datas);

        writePostion = file.Tell();
    }

    file.Seek(4 + 8, SEEK_SET);
    file.WriteArray(tables);
}

void IVText::GenerateCollection(const PathType &outFile, const std::set<char32_t> &chars)
{
    size_t count = 0;
    u8StringType u8_text;

    for (auto character : chars)
    {
        if (count == g_CharsPerLine)
        {
            u8_text += L'\n';
            count = 0;
        }

        utf8::append(character, std::back_inserter(u8_text));

        ++count;
    }

    std::ofstream stream(outFile, std::ios::trunc);

    std::copy_n(u8_text.c_str(), u8_text.size(), std::ostreambuf_iterator(stream));
}

void IVText::GenerateTable(const PathType &outFile, const std::set<char32_t> &chars)
{
    BinaryFile stream(outFile, "wb");
    stream.WriteArray2(ranges::to_vector(chars));
}

IVText::wStringType IVText::U8ToWide(const u8StringType &u8_string)
{
    wStringType result;

    utf8::utf8to16(u8_string.begin(), u8_string.end(), std::back_inserter(result));
    result.push_back(0);
    LiteralToGame(result);

    return result;
}

IVText::u8StringType IVText::WideToU8(const wStringType &wide_string)
{
    u8StringType result;

    if (!wide_string.empty())
    {
        utf8::utf16to8(wide_string.begin(), wide_string.end() - 1, std::back_inserter(result));
    }

    return result;
}

void IVText::FixCharacters(wStringType &wtext)
{
    // bad character in IV stock text: 0x85 0x92 0x94 0x96 0x97 0xA0
    // bad character in EFLC stock text: 0x93

    for (auto &character : wtext)
    {
        switch (character)
        {
        case 0x85:
            character = L' ';
            break;

        case 0x92:
        case 0x94:
            character = L'\'';
            break;

        case 0x93: // EFLC
            break;

        case 0x96:
            character = L'-';
            break;

        case 0x97:
        case 0xA0:
            character = L' ';
            break;

        default:
            break;
        }
    }
}

void IVText::LiteralToGame(wStringType &wtext)
{
    for (auto &character : wtext)
    {
        switch (character)
        {
        case L'™':
            character = 0x99;
            break;

        default:
            break;
        }
    }
}

void IVText::GameToLiteral(wStringType &wtext)
{
    for (auto &character : wtext)
    {
        switch (character)
        {
        case 0x99:
            character = L'™';
            break;

        default:
            break;
        }
    }
}

std::set<std::string> IVText::CollectTokens(const std::string &str)
{
    std::set<std::string> result;

    std::vector<std::string::size_type> wave_token_indexes, html_token_indexes;

    //收集token的起止点
    //明显是不支持嵌套尖括号的
    for (std::string::size_type index = 0; index < str.length(); ++index)
    {
        auto chr = str[index];

        if (chr == '~')
        {
            wave_token_indexes.push_back(index);
        }
        else if (chr == '<')
        {
            if (html_token_indexes.empty() || html_token_indexes.back() == '>')
            {
                html_token_indexes.push_back(index);
            }
        }
        else if (chr == '>')
        {
            if (!html_token_indexes.empty() && html_token_indexes.back() == '<')
            {
                html_token_indexes.push_back(index);
            }
        }
    }

    if ((wave_token_indexes.size() & 1) == 1 || (html_token_indexes.size() & 1) == 1)
    {
        return result;
    }

    //提取每个token
    for (std::vector<std::string::size_type>::size_type token_index = 0; token_index < wave_token_indexes.size() / 2;
         ++token_index)
    {
        result.emplace(str, wave_token_indexes[token_index * 2],
                       wave_token_indexes[token_index * 2 + 1] - wave_token_indexes[token_index * 2] + 1);
    }

    for (std::vector<std::string::size_type>::size_type token_index = 0; token_index < html_token_indexes.size() / 2;
         ++token_index)
    {
        result.emplace(str, html_token_indexes[token_index * 2],
                       html_token_indexes[token_index * 2 + 1] - html_token_indexes[token_index * 2] + 1);
    }

    return result;
}

bool IVText::CompareTokens(const std::string &s1, const std::string &s2)
{
    auto token1 = CollectTokens(s1);
    auto token2 = CollectTokens(s2);

    if (token1.size() != token2.size())
    {
        return false;
    }

    for (auto &token : token1)
    {
        if (!token2.contains(token))
        {
            return false;
        }
    }

    return true;
}

void IVText::LoadBinary(const PathType &inFile)
{
    GXTHeader gxtHeader;
    TableBlock tableBlock;
    KeyBlockOthers keyBlock;
    DataBlock tdatHeader;

    std::vector<TableEntry> tables;
    std::vector<KeyEntry> keys;
    std::vector<CharType> datas;

    m_Data.clear();

    auto tableIter = m_Data.end();

    BinaryFile file(inFile, "rb");

    if (!file)
    {
        std::printf(U8("打开输入文件 %s 失败。\n"), inFile.string().c_str());
        return;
    }

    file.Read(gxtHeader);

    file.Read(tableBlock);

    file.ReadArray(tableBlock.Size / sizeof(TableEntry), tables);

    for (TableEntry &table : tables)
    {
        tableIter = m_Data.emplace(table.Name, std::vector<TextEntry>()).first;

        file.Seek(table.Offset, SEEK_SET);

        if (strcmp(table.Name, "MAIN") != 0)
        {
            file.Read(keyBlock);
        }
        else
        {
            file.Read(keyBlock.Body);
        }

        file.ReadArray(keyBlock.Body.Size / sizeof(KeyEntry), keys);

        file.Read(tdatHeader);

        file.ReadArray(tdatHeader.Size / 2, datas);

        for (auto &key : keys)
        {
            wStringType w_string;
            TextEntry entry;
            char buffer[20];

            std::sprintf(buffer, "0x%08X", key.Hash);
            entry.hash_string = buffer;
            auto offset = key.Offset / 2;

            while (datas[offset] != 0)
            {
                w_string.push_back(datas[offset]);
                ++offset;
            }
            w_string.push_back(0);

            FixCharacters(w_string);
            GameToLiteral(w_string);
            entry.translated = WideToU8(w_string);

            tableIter->second.emplace_back(std::move(entry));
        }
    }
}

void IVText::GenerateTexts(const PathType &output_texts) const
{
    std::ofstream stream;
    std::string line;
    std::vector<char> buffer(4096);

    for (auto &table : m_Data)
    {
        stream.open(output_texts / (table.first + ".txt"), std::ios::trunc);

        if (!stream)
        {
            std::printf(U8("创建输出文件失败\n"));
        }

        stream.write("\xEF\xBB\xBF", 3);

        std::sprintf(buffer.data(), "[%s]\n", table.first.c_str());
        stream << buffer.data();

        for (auto &entry : table.second)
        {
            std::sprintf(buffer.data(), "%s=%s\n", entry.hash_string.c_str(), entry.translated.c_str());
            line = buffer.data();
            stream << ';' << line << line << '\n';
        }

        stream.close();
    }
}
