#include "retail.hh"
#include "treyarch/game/localized_string_table.hh"

using namespace treyarch;

// sub_6AB9E0
i32 treyarch::find_global_text(string_hash id) { // the id's index in the localizer, or -1
    if (references::global_text_order.read() == -1)
        retail::sub_692BB0();

    return retail::sub_692C90(id.source_hash_code,
                              (i32)&references::global_text_order.get(),
                              (i32)&references::global_text_hashes.get(),
                              0,
                              3525);
}
