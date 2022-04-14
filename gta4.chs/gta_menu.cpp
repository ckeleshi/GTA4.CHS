#include "gta_menu.h"

namespace gta_menu
{
    void register_patches(batch_matching& batch_matcher)
    {
        //密集调用GetStringWidthJump的一个函数
        //Esc菜单Header热区
        batch_matcher.register_step("D9 5C 24 4C F3 0F 10 44 24 4C F3 0F 58 44 24 2C", 1, [](const byte_pattern::result_type& addresses)
            {
                injector::WriteMemory(addresses[0].i(0xAF4B - 0xAF62 + 1), true, true);
                injector::WriteMemory(addresses[0].i(0xAF7A - 0xAF62 + 1), true, true);
                injector::WriteMemory(addresses[0].i(0xAFB0 - 0xAF62 + 1), true, true);
                injector::WriteMemory(addresses[0].i(0xAFE6 - 0xAF62 + 1), true, true);
                injector::WriteMemory(addresses[0].i(0xB01C - 0xAF62 + 1), true, true);
                injector::WriteMemory(addresses[0].i(0xB052 - 0xAF62 + 1), true, true);
                injector::WriteMemory(addresses[0].i(0xB088 - 0xAF62 + 1), true, true);
            });

        //密集调用GetStringWidthJump的另一个函数
        //Esc菜单Header间距
        batch_matcher.register_step("B9 ? ? ? ? F3 0F 11 44 24 54 E8", 1, [](const byte_pattern::result_type& addresses)
            {
                injector::WriteMemory(addresses[0].i(0x860 - 0x867 + 1), true, true);
                injector::WriteMemory(addresses[0].i(0x895 - 0x867 + 1), true, true);
                injector::WriteMemory(addresses[0].i(0x8CB - 0x867 + 1), true, true);
                injector::WriteMemory(addresses[0].i(0x901 - 0x867 + 1), true, true);
                injector::WriteMemory(addresses[0].i(0x937 - 0x867 + 1), true, true);
                injector::WriteMemory(addresses[0].i(0x96D - 0x867 + 1), true, true);
                injector::WriteMemory(addresses[0].i(0x9A3 - 0x867 + 1), true, true);
            });

        //'Esc菜单Header热区'同一个函数里
        //视频编辑器菜单Header热区
        batch_matcher.register_step("6A 00 68 ? ? ? ? F3 0F 58 44 24 20 F3 0F 11 44 24 4C", 1, [](const byte_pattern::result_type& addresses)
            {
                injector::WriteMemory(addresses[0].i(0x595 - 0x5C4 + 1), true, true);
                injector::WriteMemory(addresses[0].i(0x5C4 - 0x5C4 + 1), true, true);
                injector::WriteMemory(addresses[0].i(0x5FA - 0x5C4 + 1), true, true);
                injector::WriteMemory(addresses[0].i(0x630 - 0x5C4 + 1), true, true);
            });

        //'Esc菜单Header间距'同一个函数里
        //视频编辑器菜单Header间距
        batch_matcher.register_step("6A 00 68 ? ? ? ? B9 ? ? ? ? F3 0F 11 44 24 20", 1, [](const byte_pattern::result_type& addresses)
            {
                injector::WriteMemory(addresses[0].i(0x7E8 - 0x7E8 + 1), true, true);
                injector::WriteMemory(addresses[0].i(0x81D - 0x7E8 + 1), true, true);
            });
    }
}
