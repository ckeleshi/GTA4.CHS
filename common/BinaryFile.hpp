#pragma once
#include <cstddef>
#include <filesystem>
#include <cstdio>
#include <type_traits>
#include <vector>

class BinaryFile
{
public:
    template <typename Elem, typename Ret>
    using tElemTrait = std::enable_if_t<std::is_trivially_copyable_v<Elem>, Ret>;

    BinaryFile() :m_pFile(nullptr) {}
    ~BinaryFile() { Close(); }
    BinaryFile(const BinaryFile&) = delete;
    BinaryFile& operator=(const BinaryFile&) = delete;

    BinaryFile(BinaryFile&& rv) noexcept
        :BinaryFile()
    {
        std::exchange(m_pFile, rv.m_pFile);
    }

    BinaryFile& operator=(BinaryFile&& rv) noexcept
    {
        Close();

        std::exchange(m_pFile, rv.m_pFile);

        return *this;
    }

    BinaryFile(const std::filesystem::path& filename, const char* mode)
        :BinaryFile()
    {
        Open(filename, mode);
    }

    bool Open(const std::filesystem::path& filename, const char* mode)
    {
        Close();

        m_pFile = std::fopen(filename.string().c_str(), mode);
        return (m_pFile != nullptr);
    }

    void Close()
    {
        if (m_pFile != nullptr)
        {
            std::fclose(m_pFile);
            m_pFile = nullptr;
        }
    }

    bool Opened() const
    {
        return m_pFile != nullptr;
    }

    operator bool() const
    {
        return Opened();
    }

    BinaryFile& Seek(long offset, int mode)
    {
        fseek(m_pFile, offset, mode);

        return *this;
    }

    long Tell() const
    {
        return ftell(m_pFile);
    }

    BinaryFile& Read(void* buffer, std::size_t size)
    {
        std::fread(buffer, size, 1, m_pFile);
        return *this;
    }

    template <typename T>
    tElemTrait<T, BinaryFile&> Read(T& object)
    {
        Read(&object, sizeof(object));
        return *this;
    }

    //读取到内置数组
    template <typename T, std::size_t C>
    tElemTrait<T, BinaryFile&> ReadArray(T(&arr)[C])
    {
        Read(arr, sizeof(T) * C);
        return *this;
    }

    //读取到std::array
    template <typename T, std::size_t C>
    tElemTrait<T, BinaryFile&> ReadArray(std::array<T, C>& objects)
    {
        Read(objects.data(), sizeof(T) * C);
        return *this;
    }

    //读取指定大小到vector
    template <typename T>
    tElemTrait<T, BinaryFile&> ReadArray(std::size_t count, std::vector<T>& objects)
    {
        objects.resize(count);
        Read(objects.data(), sizeof(T) * count);
        return *this;
    }

    //读取到已经分配好内存的vector
    template <typename T>
    tElemTrait<T, BinaryFile&> ReadArray(std::vector<T>& objects)
    {
        Read(objects.data(), sizeof(T) * objects.size());
        return *this;
    }

    //读取到span
    template <typename T>
    tElemTrait<T, BinaryFile&> ReadArray(std::span<T>& objects)
    {
        Read(objects.data(), sizeof(T) * objects.size());
        return *this;
    }

    template <typename T>
    tElemTrait<T, BinaryFile&> ReadArray2(std::vector<T>& objects)
    {
        std::size_t count;
        Read(count);
        ReadArray(count, objects);
        return *this;
    }

    BinaryFile& Write(const void* buffer, std::size_t size)
    {
        std::fwrite(buffer, size, 1, m_pFile);
        return *this;
    }

    template <typename T>
    tElemTrait<T, BinaryFile&> Write(const T& object)
    {
        Write(&object, sizeof(object));
        return *this;
    }

    //内置数组
    template <typename T, std::size_t C>
    tElemTrait<T, BinaryFile&> WriteArray(T(&arr)[C])
    {
        Write(arr, sizeof(T) * C);
        return *this;
    }

    //std::array
    template <typename T, std::size_t C>
    tElemTrait<T, BinaryFile&> WriteArray(const std::array<T, C>& arr)
    {
        Write(arr, sizeof(T) * C);
        return *this;
    }

    //std::vector
    template <typename T>
    tElemTrait<T, BinaryFile&> WriteArray(const std::vector<T>& objects)
    {
        Write(objects.data(), sizeof(T) * objects.size());
        return *this;
    }

    //span
    template <typename T>
    tElemTrait<T, BinaryFile&> WriteArray(const std::span<T>& objects)
    {
        Write(objects.data(), sizeof(T) * objects.size());
        return *this;
    }

    template <typename T, std::size_t C>
    tElemTrait<T, BinaryFile&> WriteArray2(T(&arr)[C])
    {
        Write(C);
        WriteArray(arr);
        return *this;
    }

    template <typename T, std::size_t C>
    tElemTrait<T, BinaryFile&> WriteArray2(const std::array<T, C>& arr)
    {
        Write(C);
        WriteArray(arr);
        return *this;
    }

    template <typename T>
    tElemTrait<T, BinaryFile&> WriteArray2(const std::vector<T>& objects)
    {
        Write(objects.size());
        WriteArray(objects);
        return *this;
    }

    template <typename T>
    tElemTrait<T, BinaryFile&> WriteArray2(const std::span<T>& objects)
    {
        Write(objects.size());
        WriteArray(objects);
        return *this;
    }

private:
    std::FILE* m_pFile;
};
