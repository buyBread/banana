#include <d3d9.h>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/display.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"
#include "treyarch/ngl/d3d9/init.hh"
#include "treyarch/ngl/d3d9/internal_programs.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/d3d9/vertex_formats.hh"
#include "treyarch/ngl/d3d9/work_buffers.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/timing/frame_timer.hh"

using namespace treyarch;

ULONG ngl::d3d9::init() {
    references::d3d9.write(Direct3DCreate9(D3D_SDK_VERSION));

    select_default_display_mode();
    initialize_render_window();
    initialize_presentation_parameters();

    D3DDEVTYPE device_type = (D3DDEVTYPE)
        ((references::force_reference_device.read() != 0) + 1);

    references::d3d9.get()->CreateDevice( 0,
                                          device_type,
                                          ngl::references::render_window.read(),
                                          D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                         &references::presentation.get(),
                                         &references::device.get());

    provision_default_work_buffers();
    references::device.get()->GetDeviceCaps(&references::capabilities.get());
    initialize_sampler_filters();
    initialize_internal_vertex_formats();
    initialize_internal_programs();
    
    poison_bindings();
    reset_bindings();
    
    timing::restart_frame_timer();

    return initialize_framebuffers();
}
