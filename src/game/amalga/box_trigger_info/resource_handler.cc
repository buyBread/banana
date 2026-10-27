#include "retail.hh"
#include "game/amalga/box_trigger_info/resource_handler.hh"
#include "game/amalga/mashable_resource_handler.hh"
#include "game/amalga/resource_directory.hh"
#include "game/amalga/worldly_pack_slot.hh"
#include "game/arch_base.hh"
#include "game/box_trigger_info.hh"
#include "game/geometry/subdivision.hh"
#include "game/wds/references.hh"
#include "treyarch/mash/string.hh"
#include "game/math/references.hh"

using namespace treyarch;

// sub_764F30
i32 amalga::box_trigger_resource_handler::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer*) {
    if (behavior == load) {
        box_trigger_info* info = mashable_resource_handler<box_trigger_info>::construct(descriptor);

        subdivision_node_large_obb box;

        // subdivision_node_large_obb::init
        retail::sub_778C00((char*)&box,
                           (i32)references::unk_01030eac.read().source_hash_code,
                           0,
                           0,
                           (f32*)&treyarch::references::unk_0102ff40.get(),
                           (f32*)&info->x_axis,
                           (f32*)&info->y_axis,
                           (f32*)&info->z_axis);

        mash::string trigger_id(info->ent_id.m_data);

        // wds_entity_manager::create_and_add_box_trigger
        info->trig = (arch_base*)retail::sub_7DF620(&treyarch::references::g_world_ptr.read()->ent_mgr,
                                                    &trigger_id,
                                                    (f32*)&info->pos,
                                                    &box);

        return 0;
    }

    box_trigger_info* info = (box_trigger_info*)descriptor->calculate_adjusted_payload();

    // slot 0
    if (info->trig)
        ((void (__thiscall*)(arch_base*, u32))info->trig->vtable[0])(info->trig, 1);

    info->trig = nullptr;

    mashable_resource_handler<box_trigger_info>::destruct(descriptor);

    return 0;
}
