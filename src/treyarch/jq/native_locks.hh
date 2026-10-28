#pragma once

#include <array>

#include "game/mutex.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

// locks of systems that still live in retail, all set up by jobqueue_init;
// each one moves to its system's references once that system is owned (yay!)
namespace treyarch { namespace jq { namespace references {
    // ai
    inline util::memory_reference<engine_recursive_lock> unk_00fc2058 { 0x00FC2058 }; // combat dodge and block checks
    inline util::memory_reference<engine_recursive_lock> unk_00fc23e0 { 0x00FC23E0 }; // combat_state
    inline util::memory_reference<engine_recursive_lock> unk_00fc3c60 { 0x00FC3C60 }; // ai paths (quad_path_inode, creating and destroying paths)
    inline util::memory_reference<engine_recursive_lock> unk_00fc4968 { 0x00FC4968 }; // std_fear_inode, flee and cower states
    inline util::memory_reference<engine_recursive_lock> unk_00fc4a38 { 0x00FC4A38 }; // ai car list (ai_car_inode, evil car list)
    inline util::memory_reference<engine_recursive_lock> unk_00fc4a70 { 0x00FC4A70 }; // ai_action_nugget ids
    inline util::memory_reference<engine_recursive_lock> unk_00fc4a98 { 0x00FC4A98 }; // ai_item_holder_inode
    inline util::memory_reference<engine_recursive_lock> unk_00fc4b68 { 0x00FC4B68 }; // ai_tentacle_engine
    inline util::memory_reference<engine_recursive_lock> unk_00fc4ff0 { 0x00FC4FF0 }; // ai_tentacle_info
    inline util::memory_reference<engine_recursive_lock> unk_00fc5028 { 0x00FC5028 }; // ai_civilian_agressor_inode
    inline util::memory_reference<engine_recursive_lock> unk_00fc50c8 { 0x00FC50C8 }; // hospitals (medical emergencies)
    inline util::memory_reference<engine_recursive_lock> unk_00fc57e0 { 0x00FC57E0 }; // info_node registries
    inline util::memory_reference<engine_recursive_lock> unk_00fc59a8 { 0x00FC59A8 }; // combat states (sub_4CC650, sub_4CDEC0)
    inline util::memory_reference<engine_recursive_lock> unk_00fd3ef8 { 0x00FD3EF8 }; // voice_box_inode
    inline util::memory_reference<engine_recursive_lock> unk_01003138 { 0x01003138 }; // getting in and out of cars
    inline util::memory_reference<engine_recursive_lock> unk_01030b90 { 0x01030B90 }; // ped states (wander, hold_abductee)

    // ai, taken as reader/writer locks
    inline util::memory_reference<engine_recursive_lock> unk_00fc5098 { 0x00FC5098 }; // civilian victim searches
    inline util::memory_reference<engine_recursive_lock> unk_00fc60e0 { 0x00FC60E0 }; // ai cover markers

    // als
    inline util::memory_reference<engine_recursive_lock> unk_00fd8458 { 0x00FD8458 }; // animation_logic_system, vtable slot 24
    inline util::memory_reference<engine_recursive_lock> unk_00fd8478 { 0x00FD8478 }; // animation_logic_system, vtable slot 24

    // entities and their interfaces
    inline util::memory_reference<engine_recursive_lock> unk_00ffecc8 { 0x00FFECC8 }; // gun_weapon_interface firing
    inline util::memory_reference<engine_recursive_lock> unk_00ffece0 { 0x00FFECE0 }; // entity parent links
    inline util::memory_reference<engine_recursive_lock> unk_00ffed00 { 0x00FFED00 }; // damage_interface registry
    inline util::memory_reference<engine_recursive_lock> unk_00ffed20 { 0x00FFED20 }; // ise_interface registry
    inline util::memory_reference<engine_recursive_lock> unk_00ffedc0 { 0x00FFEDC0 }; // weapon_interface
    inline util::memory_reference<engine_recursive_lock> unk_00ffede8 { 0x00FFEDE8 }; // standing_interface
    inline util::memory_reference<engine_recursive_lock> unk_010030f0 { 0x010030F0 }; // grenade
    inline util::memory_reference<engine_recursive_lock> unk_01003148 { 0x01003148 }; // sub_63C580, sub_641650, sub_6463A0
    inline util::memory_reference<engine_recursive_lock> unk_010031c8 { 0x010031C8 }; // sub_605340, sub_641B00
    inline util::memory_reference<engine_recursive_lock> unk_010031d8 { 0x010031D8 }; // switch_obj_interface
    inline util::memory_reference<engine_recursive_lock> unk_01003240 { 0x01003240 }; // hiding entities and clearing areas
    inline util::memory_reference<engine_recursive_lock> unk_01003260 { 0x01003260 }; // grenade and thrown_weapon_interface
    inline util::memory_reference<engine_recursive_lock> unk_01003398 { 0x01003398 }; // gun_weapon_interface
    inline util::memory_reference<engine_recursive_lock> unk_01003428 { 0x01003428 }; // biped physics and physical_interface
    inline util::memory_reference<engine_recursive_lock> unk_01003478 { 0x01003478 }; // physical_interface prop physics and pendulums
    inline util::memory_reference<engine_recursive_lock> unk_01003498 { 0x01003498 }; // manip_obj_interface
    inline util::memory_reference<engine_recursive_lock> unk_010039a8 { 0x010039A8 }; // thrown_weapon_interface
    inline util::memory_reference<engine_recursive_lock> unk_01003610 { 0x01003610 }; // sub_61DAE0, sub_633570, sub_6506A0
    inline util::memory_reference<engine_recursive_lock> unk_01003a20 { 0x01003A20 }; // tentacle_interface registry
    inline util::memory_reference<engine_recursive_lock> unk_01003ac8 { 0x01003AC8 }; // physical_interface_driver_basic registry

    // entity visit slots (sub_6020A0, sub_602210)
    inline util::memory_reference<engine_recursive_lock> unk_01003458 { 0x01003458 }; // taken first when sub_6020A0's flag is set
    inline util::memory_reference<engine_recursive_lock> unk_01003998 { 0x01003998 }; // taken while picking a slot

    inline util::memory_reference<std::array<engine_recursive_lock, 5>> unk_00ffed68 { 0x00FFED68 }; // one per slot

    // world queries
    inline util::memory_reference<engine_recursive_lock> unk_00fc46d8 { 0x00FC46D8 }; // subdivision_visitor
    inline util::memory_reference<engine_recursive_lock> unk_010f66d0 { 0x010F66D0 }; // colmesh visitor and entity filters
    inline util::memory_reference<engine_recursive_lock> unk_01075e00 { 0x01075E00 }; // sub_789960
    inline util::memory_reference<engine_recursive_lock> unk_01075e20 { 0x01075E20 }; // region visitors
    inline util::memory_reference<engine_recursive_lock> unk_01075fd0 { 0x01075FD0 }; // sub_785150, sub_8FEE60
    inline util::memory_reference<engine_recursive_lock> unk_01076aa8 { 0x01076AA8 }; // entity handle set (sub_7A1760, sub_7AA850)

    // local collision query slots (sub_778ED0, sub_779070)
    inline util::memory_reference<engine_recursive_lock> unk_01073f58 { 0x01073F58 }; // taken while picking a slot
    inline util::memory_reference<engine_recursive_lock> unk_01074068 { 0x01074068 }; // taken first when sub_778ED0's flag is set

    inline util::memory_reference<std::array<engine_recursive_lock, 6>> unk_01076a38 { 0x01076A38 }; // one per slot

    // world objects
    inline util::memory_reference<engine_recursive_lock> unk_01031d60 { 0x01031D60 }; // beam
    inline util::memory_reference<engine_recursive_lock> unk_01075df0 { 0x01075DF0 }; // particle_generator
    inline util::memory_reference<engine_recursive_lock> unk_010769e8 { 0x010769E8 }; // line_info

    // triggers
    inline util::memory_reference<engine_recursive_lock> unk_0102ff70 { 0x0102FF70 }; // box and sphere triggers, trigger lookups
    inline util::memory_reference<engine_recursive_lock> unk_01030030 { 0x01030030 }; // trigger manager update
    inline util::memory_reference<engine_recursive_lock> unk_01030b30 { 0x01030B30 }; // trigger manager update

    // peds
    inline util::memory_reference<engine_recursive_lock> unk_0102ff80 { 0x0102FF80 }; // sub_751C10
    inline util::memory_reference<engine_recursive_lock> unk_01030040 { 0x01030040 }; // spectacle points of interest
    inline util::memory_reference<engine_recursive_lock> unk_01030098 { 0x01030098 }; // ped spawn point search (sub_7482A0)
    inline util::memory_reference<engine_recursive_lock> unk_01032020 { 0x01032020 }; // ped spawn points near a point

    // rendering
    inline util::memory_reference<engine_recursive_lock> unk_010876c0 { 0x010876C0 }; // speedtree
    inline util::memory_reference<engine_recursive_lock> unk_01087c88 { 0x01087C88 }; // speedtree
    inline util::memory_reference<engine_recursive_lock> unk_01087e18 { 0x01087E18 }; // speedtree
    inline util::memory_reference<engine_recursive_lock> unk_011106e0 { 0x011106E0 }; // render list allocation

    // sound
    inline util::memory_reference<engine_recursive_lock> unk_010f8b78 { 0x010F8B78 }; // sound bank requests and loaded checks

    // chuck natives
    inline util::memory_reference<engine_recursive_lock> unk_010f76f0 { 0x010F76F0 }; // get_global_data_mutex
    inline util::memory_reference<engine_recursive_lock> unk_010f7748 { 0x010F7748 }; // ai machine changes, combat dodge and block checks

    inline util::memory_reference<u32> unk_010f7758 { 0x010F7758 }; // zeroed right after the lock above, nothing else touches it

    // no users
    inline util::memory_reference<engine_recursive_lock> unk_01076a98 { 0x01076A98 };

    // heap locks; milestone names, matched by the order of its asserts
    inline util::memory_reference<engine_recursive_lock*> debug_message_send_mutex          { 0x00FFDA50 }; // no users
    inline util::memory_reference<engine_recursive_lock*> entity_id_manager_allocation_lock { 0x00FFEAB8 };
}}} // treyarch::jq::references
