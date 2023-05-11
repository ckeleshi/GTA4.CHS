#pragma once
#include "../common/common.h"
#include "../common/stdinc.h"

struct IVTextTableSorting
{
    // 保证'MAIN'是第一个
    bool operator()(const std::string &lhs, const std::string &rhs) const
    {
        // 确保两个'MAIN'比较结果是false
        if (rhs == "MAIN")
        {
            return false;
        }
        else if (lhs == "MAIN")
        {
            return true;
        }
        else
        {
            return lhs < rhs;
        }
    }
};

struct GXTHeader
{
    std::uint16_t Version;  // 4
    std::uint16_t CharBits; // 8 or 16
};

struct TableEntry
{
    char Name[8];
    std::int32_t Offset;
};

struct TableBlock
{
    char TABL[4];
    std::int32_t Size;
    // TableEntry Entries[Size / 12];
};

struct KeyEntry
{
    std::int32_t Offset;
    std::uint32_t Hash;
};

struct KeyBlockMAIN
{
    char TKEY[4];
    std::int32_t Size;
};

struct KeyBlockOthers
{
    char Name[8];
    KeyBlockMAIN Body;
};

struct DataBlock
{
    char TDAT[4];
    std::int32_t Size;
    // std::uint16_t Entries[Size / 2];
};

class IVText
{
  public:
    typedef std::filesystem::path PathType;                                     // 路径类型
    typedef GTAChar CharType;                                                   // 宽字符(GXT)类型
    typedef std::uint32_t HashType;                                             // GXT key类型
    typedef std::string u8StringType;                                           // 转码用
    typedef std::vector<GTAChar> wStringType;                                   // GXT字符串类型(手动补0)
    typedef std::function<void(const std::string &, std::ifstream &)> FuncType; // 文本func类型

    struct TextEntry
    {
        uint32 hash;
        u8StringType original, translated;
    };

    void ProcessT2B(const PathType &inFolder, const PathType &outFolder);
    void ProcessB2T(const PathType &inFile, const PathType &outFolder);
    void ProcessCollect(const PathType &inFolder, const PathType &outFolder);

    void ProcessT2J(const PathType &inFolder);
    void ProcessJ2B(const PathType &inFolder, const PathType &outFolder);
    void ProcessB2J(const PathType &inFile, const PathType &outFolder);

  private:
    static bool IsNativeCharacter(char32_t character);

    // 用func处理文件夹中每个文件
    static void ProcessTexts(const PathType &inFolder, bool recursive, const FuncType &func);
    static void ProcessJsons(const PathType &inFolder, bool recursive, const FuncType &func);

    // func: 加载译文
    void LoadTextFunc(const std::string &filename, std::ifstream &stream);
    void LoadJsonFunc(const std::string &filename, std::ifstream &stream);

    // func: 收集文本中需要做成字库的字符
    static void CollectCharsFunc(std::ifstream &stream, std::set<char32_t> &chars);
    // 生成字库位置数据和字符集
    static void GenerateCollection(const PathType &outFile, const std::set<char32_t> &chars);
    static void GenerateTable(const PathType &outFile, const std::set<char32_t> &chars);

    void LoadBinary(const PathType &inFile);
    void LoadTexts(const PathType &inFolder);
    void LoadJsons(const PathType &inFolder);

    void GenerateTexts(const PathType &output_texts) const;
    void GenerateJsons(const PathType &output_texts) const;
    void GenerateBinary(const PathType &output_binary) const;

    // 文本/GXT形式互转
    static wStringType U8ToWide(const u8StringType &u8_string);
    static u8StringType WideToU8(const wStringType &wide_string);

    // 修正原版GXT中的混乱字符
    static void FixCharacters(wStringType &wtext);

    // 标准编码转到游戏编码
    static void LiteralToGame(wStringType &wtext);

    // 游戏编码转到标准编码
    static void GameToLiteral(wStringType &wtext);

    // 收集'~~'和'<>'形式的Token供比较
    static std::set<std::string> CollectTokens(const std::string &str);
    static bool CompareTokens(const std::string &s1, const std::string &s2);

    std::map<std::string, std::vector<TextEntry>, IVTextTableSorting> m_Data;
};
