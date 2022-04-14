#pragma once
#include "BinaryFile.hpp"
#include <vector>
#include <optional>
#include <stdexcept>

class MemoryFile
{
public:
    typedef std::vector<unsigned char> buffer_type;

    //从物理文件构造
    MemoryFile(const std::filesystem::path& filename)
    {
        Load(filename);
    }

    //从缓冲区构造
    MemoryFile(const buffer_type& buffer)
    {
        Create(buffer);
    }

    //从缓冲区构造
    MemoryFile(buffer_type&& buffer)
    {
        Create(std::move(buffer));
    }

    //从缓冲区创建
    void Create(const buffer_type& buffer)
    {
        m_bytes_opt = buffer;
        m_file_pointer = 0;
    }

    //从缓冲区创建
    void Create(buffer_type&& buffer)
    {
        m_bytes_opt = std::move(buffer);
        m_file_pointer = 0;
    }

    //创建指定大小的文件
    void Create(long size)
    {
        if (!m_bytes_opt.has_value())
            m_bytes_opt.emplace(size);

        m_bytes_opt->resize(size);
        m_file_pointer = 0;
    }

    //读物理文件
    bool Load(const std::filesystem::path& filename)
    {
        BinaryFile binary(filename.string().c_str(), "rb");

        if (binary)
        {
            binary.Seek(0, SEEK_END);
            auto size = binary.Tell();
            limit_offset(size);
            binary.Seek(0, SEEK_SET);
            buffer_type buffer;
            buffer.reserve(s_max_file_size);
            buffer.resize(size);
            binary.Read(buffer.data(), size);
            m_bytes_opt.emplace(std::move(buffer));
            m_file_pointer = 0;
        }

        return binary;
    }

    //写物理文件
    void Dump(const std::filesystem::path& filename)
    {
        if (!Opened())
        {
            return;
        }

        BinaryFile file(filename, "rb");

        if (!file)
            return;

        file.Write(m_bytes_opt->data(), m_bytes_opt->size());
    }

    void Close()
    {
        m_bytes_opt.reset();
    }

    bool Opened() const
    {
        return m_bytes_opt.has_value();
    }

    operator bool() const
    {
        return Opened();
    }

    MemoryFile& Seek(long offset, int mode)
    {
        switch (mode)
        {
        case SEEK_SET:
            m_file_pointer = offset;
            break;

        case SEEK_END:
            m_file_pointer = m_bytes_opt->size() + offset;
            break;

        case SEEK_CUR:
            m_file_pointer += offset;
            break;

        default:
            throw std::invalid_argument("Invalid seek method.");
            break;
        }

        //Seek之后文件有可能变大
        try_expand(m_file_pointer);

        return *this;
    }

    long Tell() const
    {
        return m_file_pointer;
    }

    MemoryFile& Read(void* buffer, std::size_t size)
    {
        limit_offset(m_file_pointer + size);
        std::memcpy(buffer, m_bytes_opt->data() + size, size);
        return *this;
    }

    template <typename T>
    std::enable_if_t<std::is_trivially_copyable_v<T>, MemoryFile&> Read(T& object)
    {
        Read(&object, sizeof(object));
        return *this;
    }

    template <typename T>
    std::enable_if_t<std::is_trivially_copyable_v<T>, MemoryFile&> ReadArray(std::size_t count, std::vector<T>& objects)
    {
        objects.resize(count);
        Read(objects.data(), sizeof(T) * count);
        return *this;
    }

    MemoryFile& Write(const void* buffer, std::size_t size)
    {
        //写文件有可能导致文件变大
        try_expand(m_file_pointer + size);
        std::memcpy(m_bytes_opt->data() + m_file_pointer, buffer, size);
        return *this;
    }

    template <typename T>
    std::enable_if_t<std::is_trivially_copyable_v<T>, MemoryFile&> Write(const T& object)
    {
        Write(&object, sizeof(object));
        return *this;
    }

    template <typename T>
    std::enable_if_t<std::is_trivially_copyable_v<T>, MemoryFile&> WriteArray(const std::vector<T>& objects)
    {
        Write(objects.data(), sizeof(T) * objects.size());
        return *this;
    }

private:
    static constexpr long s_max_file_size = 104857600; //100MB

    void limit_offset(long offset) const
    {
        if (offset < 0 || offset > s_max_file_size)
            throw std::out_of_range("Invalid file offset.");
    }

    void try_expand(long size)
    {
        limit_offset(size);

        if (static_cast<long>(m_bytes_opt->size()) < size)
            m_bytes_opt->resize(size);
    }

    std::optional<std::vector<unsigned char>> m_bytes_opt;
    long m_file_pointer = 0;
};
