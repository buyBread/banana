#include "retail.hh"
#include "treyarch/game/cached_special_effect.hh"
#include "treyarch/game/wds/references.hh"

using namespace treyarch;

// inlined @ sub_6504E0
void fx_cache_ent::construct_mashed_class() {
    ent             = arch_base_vhandle();
    life_time       = -1.0f;
    alpha_fade_time = 0.0f;
}

// inlined @ sub_63E130
void fx_cache_ent::finalize(mash::allocation_scope) {
    if (arch_base* entity = ent.resolve())
        retail::sub_7FBE10((u32*)&references::g_world_ptr.read()->ent_mgr, (i32*)entity);

    ent = arch_base_vhandle();
}

// sub_6504E0
void fx_cache::construct_mashed_class() {
    self = nullptr;

    simple_list_vars[0] = nullptr;
    simple_list_vars[1] = nullptr;
    simple_list_vars[2] = nullptr;

    fx_name   .construct_mashed_class();
    cache_name.construct_mashed_class();
    cache     .construct_mashed_class();

    dynamic      = false;
    unk_20       = 0;
    cur_fx_index = 0;
}

// sub_64BC80
void fx_cache::destruct_mashed_class() {
    unk_20 = 0;

    fx_name   .destruct_mashed_class();
    cache_name.destruct_mashed_class();
    cache     .destruct_mashed_class();
}

// sub_650560
void fx_cache::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    fx_name   .unmash(mash_info, this, buffer);
    cache_name.unmash(mash_info, this, buffer);
    cache     .unmash(mash_info, this, buffer);
}
