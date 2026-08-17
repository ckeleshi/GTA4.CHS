#include "gta_menu.h"
#include "font.h"

namespace gta_menu
{
void register_patches()
{
    // 密集调用GetStringWidthJump的一个函数
    // Esc菜单Header热区
    injector::WriteMemory(injector::aslr_ptr(0x483B10).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x483B2E).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x483B54).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x483B7A).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x483BA0).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x483BC6).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x483BEC).get(), true, true);

    // 密集调用GetStringWidthJump的另一个函数
    // Esc菜单Header间距
    injector::WriteMemory(injector::aslr_ptr(0x48561B).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x48563D).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x485663).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x485689).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x4856AF).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x4856D5).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x4856FB).get(), true, true);

    // //'Esc菜单Header热区'同一个函数里
    // // 视频编辑器菜单Header热区
    injector::MakeCALL(injector::aslr_ptr(0x4840BA).get(), CFont::GetStringWidthGetAllDetour);
    injector::MakeCALL(injector::aslr_ptr(0x4840DF).get(), CFont::GetStringWidthGetAllDetour);
    injector::MakeCALL(injector::aslr_ptr(0x484104).get(), CFont::GetStringWidthGetAllDetour);
    injector::MakeCALL(injector::aslr_ptr(0x484129).get(), CFont::GetStringWidthGetAllDetour);

    // //'Esc菜单Header间距'同一个函数里
    // // 视频编辑器菜单Header间距
    injector::WriteMemory(injector::aslr_ptr(0x4864AC).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x4864CE).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x4864F4).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x48651A).get(), true, true);
}
} // namespace gta_menu
