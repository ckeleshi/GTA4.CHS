#include "gta_phone.h"
#include "gta_string.h"

namespace gta_phone
{
void register_patches()
{
    // 用到"Unknown1"的地方往后一个调用的函数里面
    // TBoGT重玩任务选择界面
    injector::MakeCALL(injector::aslr_ptr(0x8AE33F).get(), gta_string::gtaExpandString3);

    // Native: DISPLAY_TEXT_WITH_LITERAL_SUBSTRING(1FCB5241)里面第二个调用strncpy的地方
    injector::MakeCALL(injector::aslr_ptr(0xB56B8E).get(), gta_string::gtaUTF8strncpy);
    injector::MakeNOP(injector::aslr_ptr(0xB56BA2).get(), 5);
}
} // namespace gta_phone
