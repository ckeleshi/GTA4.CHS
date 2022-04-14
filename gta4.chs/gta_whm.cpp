#include "gta_whm.h"
#include "class_pointer.hpp"
#include "plugin.h"

namespace gta_whm
{
    struct html_data_node_replace_string
    {
        void operator()(injector::reg_pack& regs) const
        {
            auto p_class = reinterpret_cast<class_pointer*>(regs.ebx);
            regs.ebp = reinterpret_cast<std::uintptr_t>(plugin.whm_table.GetTranslated(
                *p_class->get_field<const uchar*>(0xD8)));
        }
    };

    void register_patches(batch_matching& batch_matcher)
    {
        //gta_html提到的用到CHtmlDataNode虚表的函数里
        //替换whm自带文本
        batch_matcher.register_step("8B AB D8 00 00 00", 1, [](const byte_pattern::result_type& addresses)
            {
                injector::MakeInline<html_data_node_replace_string>(addresses[0].i(), addresses[0].i(6));
            });
    }
}
