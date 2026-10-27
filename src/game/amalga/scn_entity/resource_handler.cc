#include <new>

#include "retail.hh"
#include "game/amalga/resource_directory.hh"
#include "game/amalga/scn_entity/resource_handler.hh"
#include "game/amalga/worldly_pack_slot.hh"
#include "game/arch_base.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/entity/scene_entity_container.hh"
#include "game/wds/references.hh"
#include "game/wds/region.hh"
#include "treyarch/mm/heap.hh"

using namespace treyarch;

// sub_76E770
i32 amalga::scene_entity_container_resource_handler::handle_resource(e_behavior           behavior,
                                                                     resource_descriptor* descriptor,
                                                                     limited_timer*       time_limit) {

    world_dynamics_system* world = treyarch::references::g_world_ptr.read();

    // wds_entity_manager: push the slot as resource context, taking both resource locks
    retail::sub_8014C0((u32*)&world->ent_mgr, (i32)pack_slot);

    engine_lock_scope lock(&world->ent_mgr.lock);

    i32 more_to_do = 1;

    if (behavior == load) {
        scene_entity_container* container;

        if (entity_index || mash_info)
            container = (scene_entity_container*)descriptor->calculate_adjusted_payload();
        else {
            u8* payload = descriptor->raw_payload;

            void* allocation = memory::heap::allocate(sizeof(mash::mash_info_struct));

            mash_info = allocation ?
                new (allocation) mash::mash_info_struct(mash::UNMASH_MODE, payload, (i32)descriptor->payload_size, true) : nullptr;

            container = nullptr;

            mash_info->unmash_class(container, nullptr, mash::NORMAL_BUFFER);

            container->construct_mashed_class();
        }

        region* slot_region = nullptr;

        if (world->the_terrain) {
            string_hash name = pack_slot->pack_name.hash;

            slot_region = (region*)retail::sub_7A1990((u32*)world->the_terrain, (i32)name.source_hash_code);
        }

        while (!(time_limit && time_limit->out_of_time()) && entity_index < (i32)container->entity_mashes.size) {
            // scene_entity_container::unmash_entity
            entity* ent = (entity*)retail::sub_639660((u32*)container, (i32)pack_slot, (i32)mash_info, entity_index);

            u32 flags = ent->ext_flags;

            ent->unk_03a = 0;
            ent->ext_flags = flags; // stored back unchanged

            retail::sub_6123B0((u32*)ent, (i32)(flags & 0xF), (i32)0xCBADF00D);

            worldly_entity_instances* instances = ((worldly_pack_slot*)pack_slot)->entity_instances;

            if (!instances->instances)
                instances->instances = (void*)retail::sub_76E190((u32*)world->ent_mgr.unk_10);

            retail::sub_8043A0((i32)&world->ent_mgr, (i32)instances->instances, (i32*)ent);

            if (slot_region && slot_region->unk_000)
                retail::sub_7ABE70((i32)slot_region, (i32)ent);

            if (ent->is_an_actor() && !retail::sub_6026D0((u32*)ent))
                retail::sub_7FBF20((u32*)&world->ent_mgr, (i32*)ent);

            ++entity_index;
        }

        if (entity_index == (i32)container->entity_mashes.size) {
            mash::mash_info_struct* finished = mash_info;

            entity_index = 0;

            if (finished) {
                finished->~mash_info_struct();

                memory::heap::free(finished);
            }

            mash_info  = nullptr;
            more_to_do = 0;
        }
    } else
        more_to_do = 0;

    // and pop it, releasing them
    retail::sub_7F9B90((u32*)&world->ent_mgr);

    return more_to_do;
}
