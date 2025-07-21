#include "gta_menu.h"
#include "font.h"

namespace gta_menu
{
void register_patches()
{
    // 密集调用GetStringWidthJump的一个函数
    // Esc菜单Header热区
    injector::WriteMemory(injector::aslr_ptr(0x4134F0).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x41350E).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x413534).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x41355A).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x413580).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x4135A6).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x4135CC).get(), true, true);

    // 密集调用GetStringWidthJump的另一个函数
    // Esc菜单Header间距
    injector::WriteMemory(injector::aslr_ptr(0x41558B).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x4155AB).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x4155D3).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x4155F9).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x41561F).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x415645).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x41566B).get(), true, true);

    //'Esc菜单Header热区'同一个函数里
    // 视频编辑器菜单Header热区
    injector::MakeCALL(injector::aslr_ptr(0x413A9A).get(), CFont::GetStringWidthGetAllDetour);
    injector::MakeCALL(injector::aslr_ptr(0x413ABF).get(), CFont::GetStringWidthGetAllDetour);
    injector::MakeCALL(injector::aslr_ptr(0x413AE4).get(), CFont::GetStringWidthGetAllDetour);
    injector::MakeCALL(injector::aslr_ptr(0x413B09).get(), CFont::GetStringWidthGetAllDetour);

    //'Esc菜单Header间距'同一个函数里
    // 视频编辑器菜单Header间距
    injector::WriteMemory(injector::aslr_ptr(0x41641C).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x41643E).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x416464).get(), true, true);
    injector::WriteMemory(injector::aslr_ptr(0x41648A).get(), true, true);
}
} // namespace gta_menu
