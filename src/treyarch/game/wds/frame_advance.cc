#include "retail.hh"
#include "treyarch/app/app.hh"
#include "treyarch/chuck/vm/script_manager.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/game_meter_manager.hh"
#include "treyarch/game/input/input_mgr.hh"
#include "treyarch/game/trigger_manager.hh"
#include "treyarch/game/wds/ai/ai_core.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/game/wds/entity/interface/morph_interface.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"
#include "treyarch/game/zombie_manager.hh"
#include "util/memory_reference.hh"

namespace treyarch {
    namespace references {
        util::memory_reference<u8> unk_00fc49d0 { 0x00FC49D0 };
        util::memory_reference<u8> unk_010fc53d { 0x010FC53D }; // never read
        util::memory_reference<u8> unk_010fc53e { 0x010FC53E };

        util::memory_reference<u32> unk_010fc550 { 0x010FC550 }; // never read

        // lazily allocated by sub_58B490; while set, the hero is unparented and given this relative po
        util::memory_reference<po*> unk_010fc580 { 0x010FC580 };

        // created by app::app through sub_428D40; the chuck stored-entity natives use it
        util::memory_reference<void*> unk_010f9bf0 { 0x010F9BF0 };
    } // references

    namespace helpers {
        // inlined; retail reads the world through g_world_ptr even where `this` is at hand
        f32 dilate_world_time(f32 time_inc) {
            f32 dilation = references::g_world_ptr.read()->time_mgr.get_time_dilation_factor();

            return (f32)((f64)dilation * (f64)time_inc);
        }

        // inlined; an entity with a time interface runs on its own clock
        f32 dilate_entity_time(const entity* ent, f32 time_inc) {
            f32 dilation = ent->has_ifc(entity_ifc_time) ?
                ent->time_ifc()->calc_time_dilation() :
                references::g_world_ptr.read()->time_mgr.get_time_dilation_factor();

            return (f32)((f64)dilation * (f64)time_inc);
        }

        // inlined into sub_978CF0
        void apply_hero_rel_po_override(entity* hero) {
            retail::sub_640910((u32*)hero);                                       // entity_base::clear_parent
            retail::sub_626BE0((i32)hero, (u64*)references::unk_010fc580.read()); // entity_base::set_rel_po
        }
    } // helpers
} // treyarch

using namespace treyarch;

// sub_978CF0
void world_dynamics_system::frame_advance(f32 time_inc) {
    using namespace helpers;

    references::unk_010fc53e.write(0);
    references::unk_010fc53d.write(1);

    retail::sub_968530((f32*)&time_mgr, time_inc); // wds_time_manager::frame_advance
    retail::sub_7FEC90((u32*)&ent_mgr, time_inc);
    retail::sub_96F4E0(this, time_inc); // world_dynamics_system::update_ai_and_visibility_proximity_maps_for_moved_entities
    retail::sub_7851D0();
    retail::sub_A1AE50((u32*)&chuck::vm::script_manager::get(), time_inc, 0); // script_manager::run
    retail::sub_763520(time_inc); // pedestrians

    if (zombie_manager* zombies = references::zombies.read())
        retail::sub_76C610((i32)zombies, time_inc);

    references::unk_010fc550.write(0);
    retail::sub_5ECB60(time_inc); // als registry
    references::unk_010fc53e.write(1);
    retail::sub_5E7840(time_inc); // aim_mech_inode registry
    time_interface::frame_advance_all_time_interfaces(time_inc);

    if (playing_scene_anims.size())
        retail::sub_976FB0((i32)this, dilate_world_time(time_inc)); // world_dynamics_system::update_scene_anims

    if (references::game.read()->get_current_view_camera())
        retail::sub_975970(1, (u32*)references::game.read()->get_current_view_camera());

    references::unk_00fc49d0.write(0);
    retail::sub_43D8C0(time_inc); // ai_path::frame_advance_all_ai_paths
    retail::sub_5A7520(time_inc);
    retail::sub_5A7970(time_inc); // traffic; reads the chuck enable_traffic switch
    retail::sub_666850();

    if (hero_ptr->player_ifc()->is_enabled()) {
        f32 hero_time_inc = dilate_entity_time(hero_ptr, time_inc);

        hero_ptr->player_ifc()->frame_advance(hero_time_inc);
        retail::sub_9338B0((u32*)references::game_meter_manager.read(), time_inc);
    }
    else {
        f32 hero_time_inc = dilate_entity_time(hero_ptr, time_inc);

        hero_ptr->player_ifc()->frame_advance_disabled(hero_time_inc);
    }

    ai_core::frame_advance_all_core_ais(time_inc);
    retail::sub_65C2A0(dilate_world_time(time_inc)); // ise_interface registry
    retail::sub_625F70(time_inc); // simple_rotators::frame_advance_rotators
    retail::sub_638890(time_inc); // physical_interface::frame_advance_all_phys_interfaces

    if (references::unk_010fc580.read())
        apply_hero_rel_po_override(hero_ptr);

    retail::sub_819110(time_inc);
    retail::sub_637130(time_inc); // standing_interface::frame_advance_all_standing_interfaces

    f32 dilated_time_inc = dilate_world_time(time_inc);

    retail::sub_A6FCC0((u32*)retail::sub_A6FBE0(), dilated_time_inc);
    retail::sub_5DD1F0();
    retail::sub_7A9810(time_inc);
    retail::sub_646F50(time_inc);
    retail::sub_823300(dilate_world_time(time_inc)); // rigid body physics
    misc_entity_updates(time_inc);
    retail::sub_79A3F0(dilate_world_time(time_inc));
    retail::sub_63C580(time_inc);
    retail::sub_682A30(time_inc); // damage_interface::frame_advance_all_damage_ifc
    morph_interface::frame_advance_all_morph_ifcs(time_inc);

    // game::handle_cameras may rewrite time_inc for everything below
    retail::sub_97C060((i32)references::game.read(), (i32)references::input_manager.read(), &time_inc);

    f32 hero_time_inc = dilate_entity_time(hero_ptr, time_inc);

    hero_ptr->player_ifc()->unk_094(hero_time_inc);

    retail::sub_63CC40(dilate_world_time(time_inc)); // tentacle_interface registry
    retail::sub_4BC100(time_inc); // ai_tentacle_info::frame_advance_all_tentacles
    retail::sub_4BC290();         // ai_tentacle_info::update_spline_on_all_tentacles
    retail::sub_639E20(time_inc); // polytube::frame_advance_all_polytubes
    retail::sub_61DDA0(time_inc); // web_wall registry
    retail::sub_6014C0(time_inc); // sound_interface registry

    if (hero_ptr && hero_ptr->has_ifc(entity_ifc_combo)) {
        combo_interface* combo = hero_ptr->combo_ifc();

        combo->frame_advance(dilate_world_time(time_inc));
    }

    dilated_time_inc = dilate_world_time(time_inc);

    retail::sub_6664B0(retail::sub_6370C0(), dilated_time_inc);
    retail::sub_9676F0(this, time_inc); // dynamic rtree update
    retail::sub_77F1F0(0, 0);
    retail::sub_801660(time_inc);
    retail::sub_756130(references::trigger_manager.read()); // trigger_manager::update
    retail::sub_77B960(the_terrain, time_inc);              // terrain::frame_advance
    retail::sub_7474E0(); // ped spawn search batches
    retail::sub_94C790((i32)references::unk_010f9bf0.read(), time_inc);
    retail::sub_477CA0(); // avoidance info buffer

    if (references::unk_010fc580.read())
        apply_hero_rel_po_override(hero_ptr);
}

// sub_95C510
void world_dynamics_system::misc_entity_updates(f32 time_inc) {
    retail::sub_7998B0(4);        // line_info::frame_advance
    retail::sub_632C80(time_inc); // weapon_interface::frame_advance_all_weapon_interfaces
    retail::sub_64BB30(time_inc); // gun_beam_cache::frame_advance_all_gun_beam_caches
    retail::sub_657CD0(time_inc); // gun_weapon_interface::frame_advance_all_gun_weapons
    retail::sub_61D570(time_inc); // grenade::frame_advance_all_grenades
    retail::sub_681120(time_inc); // manip_obj_interface::frame_advance_all_manip_objs
    retail::sub_736180(time_inc); // beam::frame_advance_all_beams
    retail::sub_646F20(time_inc);
}
