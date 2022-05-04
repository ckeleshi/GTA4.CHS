#pragma once
#include <cstdint>
#include <xbyak/xbyak.h>

template <typename Fn> const uint8_t *asm_gen(Fn func) {
    using namespace Xbyak::util;

    static Xbyak::CodeGenerator code_gen;

    code_gen.reset();
    func(code_gen);

    return code_gen.getCode();
}
