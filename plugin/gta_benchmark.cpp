#include "gta_benchmark.h"
#include "gta_string.h"

namespace gta_benchmark
{
void register_patches()
{
    // 全都在使用"Benchmark%02d%02d%02d%02d%02d%02d.txt"的函数里 (1.0.8.0 基址 0x477D00)
    // Benchmark输出文字转码
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0x46C).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0x8D3).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0x90F).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0x94B).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0x987).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0x9C3).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0x9FF).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0xA3B).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0xA77).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0xAB3).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0x137F).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0x1483).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0x19F3).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0x1A3A).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0x1A85).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0x1AC1).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0x1AFD).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0x1B39).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0x1B6A).get(), gta_string::gtaTruncateString2);
    injector::MakeCALL(injector::aslr_ptr(0x477D00 + 0x207C).get(), gta_string::gtaTruncateString2);
}
} // namespace gta_benchmark
