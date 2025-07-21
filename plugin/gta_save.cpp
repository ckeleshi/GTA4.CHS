#include "gta_save.h"
#include "gta_string.h"

namespace gta_save
{
void register_patches()
{
    // 在使用"MO_SLOT"的函数里面
    injector::MakeCALL(injector::aslr_ptr(0x789F9F).get(), gta_string::gtaExpandString);

    // 存档覆盖提示
    // 使用"SG_AUTO_SL_OVR2"的地方往前一点
    injector::MakeCALL(injector::aslr_ptr(0x82E6BA).get(), gta_string::gtaExpandString2);

    // 使用"SG_SAVE_OVRWRT2"的地方往前一点
    injector::MakeCALL(injector::aslr_ptr(0x82E79D).get(), gta_string::gtaExpandString2);

    // 使用"MO_BENCH_TEST"的函数里
    // 加载黑屏右下角的任务名
    injector::MakeCALL(injector::aslr_ptr(0x82D92F).get(), gta_string::gtaExpandString);

    // 在上方被替换位置的前一个调用里面
    injector::MakeCALL(injector::aslr_ptr(0x82C59C).get(), gta_string::gtaTruncateString);
}
} // namespace gta_save
