#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/work_buffers.hh"
#include "treyarch/ngl/list/arena.hh"
#include "treyarch/ngl/list/init.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/scene/render_sort.hh"

using namespace treyarch;

void __cdecl ngl::set_buffer_size(e_buffer_type buffer,
                                  u32           size,
                                  bool          allow_resize,
                                  bool          send_list) {
                                    
    (void)allow_resize;

    scene* current_scene = references::current_scene.read();

    bool restart_list = current_scene != nullptr;

    if (restart_list) {
        render_sort(current_scene);

        if (send_list)
            list_send();
    }

    if (d3d9::references::device.read())
        d3d9::wait_for_rendering();

    switch (buffer) {
        case buffer_list_work:
            list::replace_storage(size);
            break;
        case buffer_scratch_index:
            d3d9::replace_scratch_index_buffers(size);
            break;
        case buffer_scratch_vertex:
            d3d9::replace_scratch_vertex_buffers(size);
            break;
        case buffer_platform_work:
            d3d9::replace_platform_work_buffer(size);
            break;
        default:
            break;
    }

    if (restart_list)
        list_init();
}
