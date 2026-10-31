#include "retail.hh"
#include "game/app.hh"
#include "game/default_values.hh"
#include "game/wds/ai/ai_flag_store.hh"
#include "game/wds/ai/ai_root_inode.hh"
#include "game/wds/ai/param_block.hh"

using namespace treyarch;

// sub_4F8CB0
void ai_flag_store::initialize() { // pack mode leaves it uninitialized
    for (i32 word = 33; word >= 0; --word)
        bits[word] = 0;

    if (references::pack_mode.read())
        return;

    bits[0] &= ~0x3u;
    bits[0] |= 0x1Cu;
    bits[0] &= 0xFFFFF81Fu;

    auto defaults = (default_values*)retail::sub_4E2550((u32*)owner, 0); // ai_root_inode::get_generic_actor_defaults

    apply_params(&defaults->my_param_block);
    apply_params(owner->var_block ? owner->var_block : &owner->my_param_block);
}

// sub_4F8BF0
void ai_flag_store::apply_params(param_block* params) { // every int param named after a flag sets or clears it
    vc8::hash_map<u32, i32>* flag_name_map = get_ai_flag_name_map();

    if (mash::vector<param_base>* param_array = params->param_array) {
        for (param_base** param = param_array->begin(); param != param_array->end(); ++param) {
            if ((*param)->my_type != param_type_int)
                continue;

            i32 flag = find_ai_flag(flag_name_map, (*param)->get_name().source_hash_code, 0);

            if (flag == -1)
                continue;

            if ((*param)->get_data_int())
                bits[flag >> 5] |= 1u << (flag & 31);
            else
                bits[flag >> 5] &= ~(1u << (flag & 31));
        }
    }

    initialized = true;
}

// sub_4F8F10
void ai_flag_store::set(i32 flag, bool value) {
    engine_lock_scope scope(&lock);

    if (!initialized)
        initialize();

    if (value)
        bits[flag >> 5] |= 1u << (flag & 31);
    else
        bits[flag >> 5] &= ~(1u << (flag & 31));
}

// sub_4F8FA0
bool ai_flag_store::get(i32 flag) {
    engine_lock_scope scope(&lock);

    if (!initialized)
        initialize();

    return (bits[flag >> 5] & (1u << (flag & 31))) != 0;
}

// sub_42A700
vc8::hash_map<u32, i32>* treyarch::get_ai_flag_name_map() {
    u32 &guard = references::ai_flag_name_map_guard.get();

    if (!(guard & 1)) {
        guard |= 1;

        retail::sub_530AC0((i32)&references::ai_flag_name_map.get()); // inserts every flag's name
        retail::sub_ADE5FD(retail::sub_B67630); // the game's CRT atexit
    }

    return &references::ai_flag_name_map.get();
}

// sub_528660
i32 treyarch::find_ai_flag(const vc8::hash_map<u32, i32>* flag_name_map, u32 name_hash, i32 unused) {
    auto* entry = flag_name_map->find(name_hash);

    return entry == flag_name_map->head() ? -1 : entry->value.value;
}
