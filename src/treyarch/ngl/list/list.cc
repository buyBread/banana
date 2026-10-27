#include <algorithm>

#include "treyarch/ngl/debug/debug.hh"
#include "treyarch/ngl/gpu/d3d9/device.hh"
#include "treyarch/ngl/lighting/lighting.hh"
#include "treyarch/ngl/list/list.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/ngl/timing/frame_timer.hh"
#include "treyarch/base/tl_system.hh"
#include "game/timing/hires_clock.hh"

using namespace treyarch;

// sub_588C40
void* ngl::list_alloc(u32 size, u32 alignment) {
    u32 aligned = ~(alignment - 1) & ((u32)references::list_work_pos.read() + alignment - 1);

    if (aligned + size <= references::list_work_size.read() + (u32)references::list_work.read()) {
        references::list_work_pos.write((u8*)(aligned + size));

        return (void*)aligned;
    }

    tl_fatal("Render list allocation overflow. Reserved = %d Requested = %d Free = %d.\n",
             references::list_work_size.read(),
             size,
             (u32)references::list_work.read() + references::list_work_size.read() - aligned);

    return nullptr;
}

// inlined @ sub_9E8900
void ngl::replace_list_work_buffer(u32 size) {
    if (references::list_work.read()) {
        tl_mem_free(references::list_work.read());

        references::list_work.write(nullptr);
    }

    if (size)
        references::list_work.write((u8*)tl_mem_alloc(size, 8, 0));

    references::list_work_size.write(size);
}

// sub_9DCDE0
ngl::scene* ngl::ngli_list_init() {
    timing::references::list_tick.write(timing::references::tick_state.get().tick_count);

    references::perf_info.get().list_submit_cycles =
        treyarch::timing::get_cpu_cycle();

    lighting::reset_context_registry();
    references::list_work_pos.write(references::list_work.read());

    lighting::references::default_light_context.write
        (lighting::create_light_context());

    if (references::sync_debug.get().dump_frame_log)
        references::debug.get().dump_frame_log = 0;

    if (references::sync_debug.get().dump_scene_file) {
        references::debug.get().dump_scene_file = 0;

        scene_dump_start();
    }

    references::sync_debug.get() = references::debug.get();
    references::build_scene.write(nullptr);

    list_begin_scene(scene_parameter_defaults);
    set_scene_name("NGL Root");
    set_scene_option_group_0(false, true, false);

    return set_scene_option_group_1(false, false, false);
}

// sub_9E7010
void ngl::ngli_list_send() {
    u64 current_cycles = treyarch::timing::get_cpu_cycle();

    performance_info &performance = references::perf_info.get();

    u64 elapsed_cycles = current_cycles - performance.list_submit_cycles;

    performance.list_send_milliseconds = (f32)
        treyarch::timing::cycles_to_milliseconds(elapsed_cycles);

    debug::reset_primitive_batches();

    references::debug.get().current_node = 0;

    if (references::build_scene.read() != references::root_build_scene.read())
        tl_fatal("nglListSend called while one or more scenes were still active (need to call nglListEndScene).\n");

    d3d9::list_send_batch();
}

bool ngl::opaque_compare::operator()(const render_sort_entry &first,
                                     const render_sort_entry &second) const {

    return first.sort_key.integer < second.sort_key.integer;
}

bool ngl::trans_compare::operator()(const render_sort_entry &first,
                                    const render_sort_entry &second) const {

    if (first.sort_key.floating > second.sort_key.floating)
        return true;
    if (first.sort_key.floating < second.sort_key.floating)
        return false;

    return (u32)first.node < (u32)second.node;
}

template <typename compare_type>
void ngl::sort_list(render_node* &head,
                    u32          count,
                    compare_type compare) {

    u8* saved_pos = references::list_work_pos.read();

    auto* entries = (render_sort_entry*)list_alloc(sizeof(render_sort_entry) * count, 16);

    render_sort_entry* entry = entries;

    for (render_node* node = head; node; node = node->next) {
        entry->node     = node;
        entry->sort_key = node->sort_key;

        ++entry;
    }

    /* note: this might produce issues!
             VC8 had Dinkumware STL, modern MSVC may have different sorting behavior(?) */
    std::sort(entries, entries + count, compare);

    render_node* previous = nullptr;

    entry = entries + count;

    while (count) {
        --entry;

        entry->node->next = previous;
        previous          = entry->node;

        --count;
    }

    head = previous;

    references::list_work_pos.write(saved_pos);
}

// sub_9DA2A0
template void ngl::sort_list<ngl::opaque_compare>(render_node* &, u32, opaque_compare);

// sub_9DA360
template void ngl::sort_list<ngl::trans_compare>(render_node* &, u32, trans_compare);

// sub_72C560
void ngl::begin_render_node(render_node* head) {
    debug_state &debug = references::debug.get();

    references::prev_render_node.write(nullptr);
    references::cur_render_node.write(head);

    render_node* node = head;

    while (node && debug.current_node < debug.test_node_start) {
        node = node->next;
        ++debug.current_node;

        references::cur_render_node.write(node);
    }

    if (debug.current_node > debug.test_node_end)
        references::cur_render_node.write(nullptr);
}

// sub_72C5B0
void ngl::advance_render_node() {
    debug_state &debug = references::debug.get();

    render_node* node = references::cur_render_node.read();

    references::prev_render_node.write(node);

    do {
        node = node->next;

        ++debug.current_node;

        references::cur_render_node.write(node);
    } while (node && debug.current_node < debug.test_node_start);

    if (debug.current_node > debug.test_node_end)
        references::cur_render_node.write(nullptr);
}

// inlined @ sub_9E97E0
void ngl::render_nodes(render_node* head) {
    for (begin_render_node(head); references::cur_render_node.read(); advance_render_node())
        dispatch_render(references::cur_render_node.read());
}
