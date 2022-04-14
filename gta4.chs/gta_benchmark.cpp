#include "gta_benchmark.h"
#include "gta_string.h"

namespace gta_benchmark
{
    void register_patches(batch_matching& batch_matcher)
    {
        //全都在使用"Benchmark%02d%02d%02d%02d%02d%02d.txt"的函数里
        //Benchmark输出文字转码
        batch_matcher.register_step("B8 68 2E 00 00 E8", 1, [](const byte_pattern::result_type& addresses)
            {
                injector::MakeCALL(addresses[0].i(0x04D9 - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x0926 - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x0962 - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x099E - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x09DA - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x0A16 - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x0A52 - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x0A8E - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x0ACA - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x0B06 - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x13C2 - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x14C2 - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x19DD - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x1A27 - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x1A7A - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x1AB6 - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x1AF2 - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x1B2E - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x1B63 - 6), gta_string::gtaTruncateString2);
                injector::MakeCALL(addresses[0].i(0x2041 - 6), gta_string::gtaTruncateString2);
            });
    }
}
