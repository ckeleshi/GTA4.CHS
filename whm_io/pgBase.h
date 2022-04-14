#pragma once

typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char uchar;

enum class ptr_element_type :uint
{
    Cpu_Type = 5,
    Gpu_Type = 6
};

template <typename T>
struct pgPtr
{
    uint o : 28;            //block中的位置
    ptr_element_type t : 4; //资源类型

    T* locate(std::vector<uchar>& block) const
    {
        if (block.empty() || t != ptr_element_type::Cpu_Type)
        {
            return nullptr;
        }
        else
        {
            return reinterpret_cast<T*>(block.data() + o);
        }
    }
};

//指向一个C String
struct pgString : pgPtr<char>
{

};

//对象数组
template <typename T>
struct pgObjectArray : pgPtr<T>
{
    ushort count; //有效元素个数(std::vector的size)
    ushort size;  //占据空间个数(std::vector的capacity)

    std::span<T> get_span(std::vector<uchar>& block) const
    {
        return { pgPtr<T>::locate(block), std::max(count,size) };
    }
};

//对象指针数组
template <typename T>
struct pgObjectPtrArray : pgObjectArray<pgPtr<T>>
{

};
