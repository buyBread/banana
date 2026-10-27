#include <algorithm>

#include "retail.hh"
#include "game/WinMain.hh"
#include "game/amalga/resource_manager.hh"
#include "game/app.hh"
#include "game/als/als.hh"
#include "game/als/als_jq.hh"
#include "game/animation/animation_controller_jq.hh"
#include "game/animation/animation_pose_jq.hh"
#include "game/event/event_manager.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/render/references.hh"
#include "game/wds/subdivision_visitor.hh"
#include "treyarch/jq/jq.hh"
#include "treyarch/jq/native_locks.hh"
#include "game/arch_base_vhandle.hh"
#include "treyarch/mm/mm.hh"
#include "game/mutex.hh"

using namespace treyarch;

// sub_4291C0
void jq::jobqueue_init() {
    treyarch::references::master_clock_is_up.write(1);

    const i32 worker_count = std::max<i32>(treyarch::references::processor_count.read() - 1, 1);

    retail::sub_A140E0();             // jq_init
    retail::sub_A14040(1024);         // jq_set_batch_pool_size
    retail::sub_A14430(256);          // jq_set_batch_group_pool_size
    retail::sub_A14150(worker_count); // jq_start
    retail::sub_A6C830();             // InMultiThreadedCode's locks

    references::unk_01075fd0.get().reset();
    references::unk_00fd8478.get().reset();
    als::references::force_update_lock.get().reset();
    references::unk_00fd8458.get().reset();
    references::unk_010f8b78.get().reset();
    references::unk_011106e0.get().reset();
    references::unk_010f66d0.get().reset();
    references::unk_010f76f0.get().reset();
    references::unk_01075e00.get().reset();
    als::references::als_list_lock.get().reset();
    references::unk_01076aa8.get().reset();
    references::unk_00fc46d8.get().reset();

    auto* arch_handle_manager_lock = (engine_recursive_lock*)mm::allocate(sizeof(engine_recursive_lock));

    if (arch_handle_manager_lock)
        arch_handle_manager_lock->reset();

    treyarch::references::g_arch_handle_manager_lock.write(arch_handle_manager_lock);
    arch_handle_manager_lock->reset();

    references::unk_00fc4a70.get().reset();
    references::unk_00fc60e0.get().reset();
    references::unk_00fc60e0.get().writer_depth = 0;
    references::unk_00fc5f60.get().reset();
    references::unk_00fc4a98.get().reset();
    references::unk_00fc3c60.get().reset();
    references::unk_00fc4a38.get().reset();
    references::unk_00fc5028.get().reset();
    references::unk_00fc5098.get().reset();
    references::unk_00fc5098.get().writer_depth = 0;
    references::unk_00fc50c8.get().reset();
    references::unk_00fc2058.get().reset();
    references::unk_00fc23e0.get().reset();
    references::unk_00fc4968.get().reset();
    references::unk_010031d8.get().reset();
    references::unk_00fc4ff0.get().reset();
    references::unk_00fc4b68.get().reset();
    references::unk_00fd3ef8.get().reset();
    treyarch::references::ai_core_list_lock.get().reset();
    references::unk_00fc59a8.get().reset();
    references::unk_00fc57e0.get().reset();
    references::unk_01031d60.get().reset();
    references::unk_01003610.get().reset();
    references::unk_00ffed00.get().reset();
    references::unk_01003a20.get().reset();
    references::unk_00ffed20.get().reset();
    references::unk_01003240.get().reset();
    references::unk_01003998.get().reset();
    references::unk_01003458.get().reset();
    references::unk_00ffece0.get().reset();
    treyarch::references::event_manager_lock.get().reset();
    references::unk_01003398.get().reset();
    references::unk_00ffecc8.get().reset();
    references::unk_010769e8.get().reset();
    references::unk_01003498.get().reset();
    references::unk_01075df0.get().reset();
    references::unk_01030b90.get().reset();
    references::unk_01003ac8.get().reset();
    references::unk_01003428.get().reset();
    references::unk_01003478.get().reset();
    references::unk_00ffedd8.get().reset();
    treyarch::references::subdivision_query_lock.get().reset();
    treyarch::references::render_lock.get().reset();
    references::unk_01076a98.get().reset();
    references::unk_01075e20.get().reset();
    amalga::resource_manager::references::unk_010300a8.get().reset();
    references::unk_010f7748.get().reset();
    references::unk_010f7758.write(0);
    references::unk_010039a8.get().reset();
    references::unk_010030f0.get().reset();
    references::unk_01003260.get().reset();
    references::unk_0102ff70.get().reset();
    references::unk_00ffedc0.get().reset();
    references::unk_01030040.get().reset();
    references::unk_010031c8.get().reset();
    references::unk_01003138.get().reset();
    references::unk_00ffede8.get().reset();
    references::unk_01032020.get().reset();
    references::unk_01030098.get().reset();
    references::unk_0102ff80.get().reset();
    references::unk_01073f58.get().reset();
    references::unk_01074068.get().reset();
    references::unk_01030030.get().reset();
    references::unk_01030b30.get().reset();
    references::unk_010876c0.get().reset();
    references::unk_01087e18.get().reset();
    references::unk_01087c88.get().reset();
    references::unk_01003148.get().reset();

    for (engine_recursive_lock &lock : references::unk_00ffed68.get())
        lock.reset();

    for (engine_recursive_lock &lock : references::unk_01076a38.get())
        lock.reset();

    auto* debug_message_send_mutex = (engine_recursive_lock*)mm::allocate(sizeof(engine_recursive_lock));

    if (debug_message_send_mutex)
        debug_message_send_mutex->reset();

    references::debug_message_send_mutex.write(debug_message_send_mutex);
    debug_message_send_mutex->reset();

    auto* entity_id_manager_allocation_lock = (engine_recursive_lock*)mm::allocate(sizeof(engine_recursive_lock));

    if (entity_id_manager_allocation_lock)
        entity_id_manager_allocation_lock->reset();

    references::entity_id_manager_allocation_lock.write(entity_id_manager_allocation_lock);
    entity_id_manager_allocation_lock->reset();

    als::animation_event_signal_jq::init();
    als::multi_animation_event_signal_jq::init();
    als::pre_animation_event_signal_jq::init();
    als::multi_pre_animation_event_signal_jq::init();
    animation_controller_jq::init();
    multi_animation_controller_jq::init();
    animation_signal_jq::init();
    multi_animation_signal_jq::init();
    animation_pose_jq::init();
    als::als_jq::init();
    als::als_mocomp_jq::init();
    als::multi_als_mocomp_jq::init();
    retail::sub_4CE360(); // ai_inode_determiner_jq
    retail::sub_4CACB0(); // ai_inode_jq
    retail::sub_4CADE0(); // post_ai_inode_jq
    retail::sub_4E0D10(); // ai_core_jq
    retail::sub_4CD7A0(); // pre_ai_core_jq
    retail::sub_677660(); // standing_jq
    retail::sub_638C20(); // simple_rotators_jq
    retail::sub_66A300(); // damage_interface_jq
    retail::sub_822B70(); // prop_collisions_jq
    retail::sub_822EC0(); // ragdoll_collisions_jq
    retail::sub_683620(); // physical_interface_jq
    retail::sub_75F090(); // ped_jq
    retail::sub_746BB0(); // vendor_jq
    retail::sub_758220(); // fake_ped_jq
    retail::sub_747090(); // ped_spawn_pos_search_jq
    retail::sub_763330(); // ped_spawn_quad_search_jq
    retail::sub_75E020(); // trigger_enter_jq
    retail::sub_481050(); // ai_avoidance_jq
    retail::sub_96F480(); // entity_sphere_region_helper_jq
    retail::sub_905570(); // game_sound_jq
    retail::sub_5A6200(); // vehicle_lod_jq
    retail::sub_79EBC0(); // particle_frame_advance_jq

    references::unnamed_queue_group   .write(retail::sub_A144D0()); // jq_alloc_batch_group
    references::unnamed_queue_finished.write(1);
    references::unnamed_queue_active  .write(0);
    references::unnamed_queue_callback.write((batch_callback)retail::sub_5E3400);
}
