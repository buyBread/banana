#include "retail.hh"
#include "game/amalga/resource_types.hh"
#include "game/frontend/igo/igo_3d_gta_widget.hh"
#include "game/resource_key.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"

using namespace treyarch;

// sub_6A0E40
igo_3d_gta_widget::igo_3d_gta_widget() {
    vtable = &references::igo_3d_gta_widget_vtable.get();

    resource_key key;
    retail::sub_739B10((u32*)&key, "target_ring", (i32)amalga::e_resource_type::entity);

    auto ring = (entity*)retail::sub_804740((u32*)&references::g_world_ptr.read()->ent_mgr,
                                            (i32)key.get_hash().source_hash_code,
                                            (i32*)1); // wds_entity_manager::spawn_entity_autoname

    if (ring) {
        ring->set_visible(false, false);
        target_ring = ring->my_handle;
    }

    render_scale = 1.0f;
    active       = false;
    unk_19       = 0;
    unk_20       = 0;
    unk_21       = 0;
    pos_offset   = vector3d(0.0f, 0.0f, 0.0f);
}

// sub_6A0EE0
void igo_3d_gta_widget::set_color(vector3d color) {
    auto* ring = (entity*)target_ring.resolve();

    if (!ring)
        return;

    color32 render_color;

    render_color.set_red_float  (color.x);
    render_color.set_green_float(color.y);
    render_color.set_blue_float (color.z);
    render_color.a = 0xFF;

    ring->set_render_color(render_color);
}
