#include "gta_whm.h"
#include "class_pointer.hpp"
#include "plugin.h"

namespace gta_whm
{
struct html_data_node_replace_string
{
    void operator()(injector::reg_pack &regs) const
    {
        auto p_class = reinterpret_cast<class_pointer *>(regs.esi);
        regs.ebp =
            reinterpret_cast<std::uintptr_t>(plugin.whm_table.GetTranslated(*p_class->get_field<const uchar *>(0xD8)));
    }
};

void register_patches()
{
    // gta_html提到的用到CHtmlDataNode虚表的函数里
    // 替换whm自带文本
    injector::MakeInline<html_data_node_replace_string>(injector::aslr_ptr(0x4B41EA).get(),
                                                        injector::aslr_ptr(0x4B41EA + 6).get());
}
} // namespace gta_whm
