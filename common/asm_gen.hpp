#pragma once
#include "stdinc.h"

template <typename Fn> const uint8 *asm_gen(Fn func) {
    using namespace Xbyak::util;

    static Xbyak::CodeGenerator code_gen;

    code_gen.reset();
    func(code_gen);

    return code_gen.getCode();
}
