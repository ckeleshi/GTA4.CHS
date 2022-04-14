#pragma once
#include "../common/stdinc.h"

//在没有定义的情况下，手动访问结构体成员
class class_pointer
{
public:
    template <typename T>
    T* get_field(std::uintptr_t offset) const
    {
        return reinterpret_cast<T*>(reinterpret_cast<std::intptr_t> (this) + offset);
    }

    //TODO: call_member_function
    template <typename Ret, typename ...Arg>
    Ret call_member_function(std::uintptr_t func_addr, Arg ...args)
    {
        return reinterpret_cast<Ret(__thiscall*)(class_pointer*, Arg...)>(func_addr)(this, std::forward<Arg>(args)...);
    }

    //TODO: call_virtual_function
    template <typename Ret, typename ...Arg>
    Ret call_virtual_function(std::size_t index, Arg ...args)
    {
        auto func_addr = (*get_field<std::uintptr_t*>(0))[index];

        return call_member_function(func_addr, std::forward<Arg>(args)...);
    }
};
