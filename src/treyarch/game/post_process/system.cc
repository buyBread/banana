#include <new>

#include "retail.hh"
#include "treyarch/game/post_process/post_process.hh"
#include "treyarch/game/shader_resource_manager.hh"
#include "treyarch/ngl/display.hh"
#include "treyarch/ngl/timing/frame_timer.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

u32 post_process::get_blend_time() {
    return (u32)(i64)((f64)ngl::get_vblank_milliseconds() *
                      (f64)ngl::timing::references::list_tick.read());
}

// sub_745B90
post_process::system::system() {
    const luminosity_parameters          default_luminosity          { 2.0f, 32.0f };
    const blur_parameters                default_blur                { 1, 3, 0 };
    const poisson_disc_blur_parameters   default_poisson_disc_blur   { 1, 3.0f };
    const radial_blur_parameters         default_radial_blur         { 4, 0.0f, 0.0f, 0.1f, 0.0f };
    const depth_of_field_blur_parameters default_depth_of_field_blur { 0.5f, 0.0f, 0.0f, 3, nullptr };
    const vignette_parameters            default_vignette            { { 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, 0 };
    const noise_parameters               default_noise               { { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f } };
    const color_adjust_parameters        default_color_adjust        { { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f } };

    luminosity.current    = default_luminosity;
    luminosity.previous   = default_luminosity;
    luminosity.target     = default_luminosity;
    luminosity.start_time = 0;
    luminosity.end_time   = 0;

    blur.current    = default_blur;
    blur.previous   = default_blur;
    blur.target     = default_blur;
    blur.start_time = 0;
    blur.end_time   = 0;

    poisson_disc_blur.current    = default_poisson_disc_blur;
    poisson_disc_blur.previous   = default_poisson_disc_blur;
    poisson_disc_blur.target     = default_poisson_disc_blur;
    poisson_disc_blur.start_time = 0;
    poisson_disc_blur.end_time   = 0;

    adaptation_rate.current    = 2.0f;
    adaptation_rate.previous   = 2.0f;
    adaptation_rate.target     = 2.0f;
    adaptation_rate.start_time = 0;
    adaptation_rate.end_time   = 0;

    // sub_7349D0
    {
        const bloom_parameters default_bloom { { 1.0f, 1.0f, 0.0f, 0.255f, 31.0f, 0.99f, 0.5f, 0.0f, 0.0f,
                                                 0.0f, 0.0f, 1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f } };

        bloom.current    = default_bloom;
        bloom.previous   = default_bloom;
        bloom.target     = default_bloom;
        bloom.start_time = 0;
        bloom.end_time   = 0;
    }

    radial_blur.current    = default_radial_blur;
    radial_blur.previous   = default_radial_blur;
    radial_blur.target     = default_radial_blur;
    radial_blur.start_time = 0;
    radial_blur.end_time   = 0;

    depth_of_field_blur.current    = default_depth_of_field_blur;
    depth_of_field_blur.previous   = default_depth_of_field_blur;
    depth_of_field_blur.target     = default_depth_of_field_blur;
    depth_of_field_blur.start_time = 0;
    depth_of_field_blur.end_time   = 0;

    spherize.current    = 0.0f;
    spherize.previous   = 0.0f;
    spherize.target     = 0.0f;
    spherize.start_time = 0;
    spherize.end_time   = 0;

    vignette.current    = default_vignette;
    vignette.previous   = default_vignette;
    vignette.target     = default_vignette;
    vignette.start_time = 0;
    vignette.end_time   = 0;

    noise.current    = default_noise;
    noise.previous   = default_noise;
    noise.target     = default_noise;
    noise.start_time = 0;
    noise.end_time   = 0;

    color_adjust.current    = default_color_adjust;
    color_adjust.previous   = default_color_adjust;
    color_adjust.target     = default_color_adjust;
    color_adjust.start_time = 0;
    color_adjust.end_time   = 0;

    last_update_time        = 0;
    update_delta            = 0;
    blitter                 = nullptr;
    hdr_targets             = nullptr;
    ldr_targets             = nullptr;
    average_luminosity      = nullptr;
    adapted_luminosity      = nullptr;
    secondary_ldr_targets_0 = nullptr;
    secondary_ldr_targets_1 = nullptr;
    adapted_luminosity_next = nullptr;

    down_sample                       = nullptr;
    luminosity_filter                 = nullptr;
    gaussian_blur_3x3_filter          = nullptr;
    gaussian_blur_7x7_filter          = nullptr;
    gaussian_blur_15x15_filter        = nullptr;
    poisson_disc_blur_filter          = nullptr;
    luminosity_range_avg_filter       = nullptr;
    luminosity_range_avg_accum_filter = nullptr;
    adapt_filter                      = nullptr;
    blend_filter                      = nullptr;
    bloom_filter                      = nullptr;
    radial_blur_filter                = nullptr;
    depth_of_field_blur_filter        = nullptr;
    depth_of_field_filter             = nullptr;
    spherize_filter                   = nullptr;
    vignette_filter                   = nullptr;
    noise_filter                      = nullptr;
    color_adjust_filter               = nullptr;
    queue                             = nullptr;
    damage_texture                    = nullptr;

    void* allocation = memory::heap::allocate(sizeof(treyarch::blitter));
    blitter = allocation ? new (allocation) treyarch::blitter() : nullptr;

    if (!blitter)
        return;

    allocation  = memory::heap::allocate(sizeof(mipmap_texture_array));
    hdr_targets = allocation ? new (allocation) mipmap_texture_array() : nullptr;
    allocation  = memory::heap::allocate(sizeof(mipmap_texture_array));
    ldr_targets = allocation ? new (allocation) mipmap_texture_array() : nullptr;

    if (!hdr_targets || !ldr_targets)
        return;

    allocation              = memory::heap::allocate(sizeof(mipmap_texture_array));
    secondary_ldr_targets_0 = allocation ? new (allocation) mipmap_texture_array() : nullptr;
    allocation              = memory::heap::allocate(sizeof(mipmap_texture_array));
    secondary_ldr_targets_1 = allocation ? new (allocation) mipmap_texture_array() : nullptr;

    if (!secondary_ldr_targets_0 || !secondary_ldr_targets_1)
        return;

    allocation        = memory::heap::allocate(sizeof(filter));
    luminosity_filter = allocation ? new (allocation) filter(blitter) : nullptr;

    if (!luminosity_filter)
        return;

    allocation  = memory::heap::allocate(sizeof(down_sample_filter));
    down_sample = allocation ? new (allocation) down_sample_filter(blitter) : nullptr;

    if (!down_sample)
        return;

    // allocated in this order, each one only once the previous succeeded
    filter** filters[] { &gaussian_blur_3x3_filter,
                         &gaussian_blur_7x7_filter,
                         &gaussian_blur_15x15_filter,
                         &poisson_disc_blur_filter,
                         &luminosity_range_avg_filter,
                         &luminosity_range_avg_accum_filter,
                         &adapt_filter,
                         &blend_filter,
                         &bloom_filter,
                         &radial_blur_filter,
                         &depth_of_field_blur_filter,
                         &depth_of_field_filter,
                         &spherize_filter,
                         &vignette_filter,
                         &noise_filter,
                         &color_adjust_filter };

    for (filter** slot : filters) {
        allocation = memory::heap::allocate(sizeof(filter));
        *slot      = allocation ? new (allocation) filter(blitter) : nullptr;

        if (!*slot)
            return;
    }

    queue = (filter_queue*)memory::heap::allocate(sizeof(filter_queue));

    if (queue) {
        queue->head       = nullptr;
        queue->tail       = nullptr;
        queue->count      = 0;
        queue->free_nodes = nullptr;
    }

    if (!queue)
        return;

    luminosity_filter->setup                 = (void*)retail::sub_730120;
    gaussian_blur_3x3_filter->setup          = (void*)retail::sub_73C840;
    gaussian_blur_7x7_filter->setup          = (void*)retail::sub_73C840;
    gaussian_blur_15x15_filter->setup        = (void*)retail::sub_73C840;
    poisson_disc_blur_filter->setup          = (void*)retail::sub_73CEC0;
    luminosity_range_avg_filter->setup       = (void*)retail::sub_73D140;
    luminosity_range_avg_accum_filter->setup = (void*)retail::sub_73D140;
    adapt_filter->setup                      = (void*)retail::sub_7301E0;
    blend_filter->setup                      = (void*)retail::sub_730290;
    bloom_filter->setup                      = (void*)retail::sub_730330;
    radial_blur_filter->setup                = (void*)retail::sub_7305C0;
    depth_of_field_blur_filter->setup        = (void*)retail::sub_73D250;
    depth_of_field_filter->setup             = (void*)retail::sub_7306B0;
    spherize_filter->setup                   = (void*)retail::sub_730710;
    vignette_filter->setup                   = (void*)retail::sub_730790;
    noise_filter->setup                      = (void*)retail::sub_7308D0;
    color_adjust_filter->setup               = (void*)retail::sub_730CC0;

    set_luminosity(nullptr, 0);
    set_blur(nullptr, 0);
    set_poisson_disc_blur(nullptr, 0);
    adaptation_rate.set(2.0f, 0);
    set_bloom(nullptr, 0);
    set_radial_blur(nullptr, 0);
    set_depth_of_field_blur(nullptr, 0);
    spherize.set(0.0f, 0);
    set_vignette(nullptr, 0);
    set_noise(nullptr, 0);
    set_color_adjust(nullptr, 0);
}

// sub_762E00
u32 post_process::system::create_render_targets(u8* work_buffer) {
    u8* cursor = work_buffer;

    if (!ldr_targets->create(work_buffer, 512, 256, 0, D3DFMT_A8R8G8B8, D3DFMT_A8R8G8B8, 0))
        return 0;

    u32 hdr_levels = hdr_targets->create(work_buffer,
                                         512,
                                         256,
                                         0,
                                         D3DFMT_A16B16G16R16F,
                                         D3DFMT_A16B16G16R16F,
                                         0);
    if (!hdr_levels)
        return 0;

    if (work_buffer)
        cursor = work_buffer + hdr_levels;

    if (!secondary_ldr_targets_1->create(cursor, 512, 256, 0, D3DFMT_A8R8G8B8, D3DFMT_A8R8G8B8, 0) ||
        !secondary_ldr_targets_0->create(cursor, 512, 256, 0, D3DFMT_A8R8G8B8, D3DFMT_A8R8G8B8, 0) ||
        !create_noise_texture(noise_texture, 256, 256))

        return 0;

    void* allocation   = memory::heap::allocate(sizeof(surface_texture));
    average_luminosity = allocation ? new (allocation) surface_texture() : nullptr;

    if (!average_luminosity->create(1, 1, 1, D3DFMT_A16B16G16R16F, D3DFMT_A16B16G16R16F, 0))
        return 0;

    average_luminosity->clear(0);

    allocation         = memory::heap::allocate(sizeof(surface_texture));
    adapted_luminosity = allocation ? new (allocation) surface_texture() : nullptr;

    if (!adapted_luminosity->create(1, 1, 1, D3DFMT_R32F, D3DFMT_R32F, 0))
        return 0;

    adapted_luminosity->clear(0);

    allocation              = memory::heap::allocate(sizeof(surface_texture));
    adapted_luminosity_next = allocation ? new (allocation) surface_texture() : nullptr;

    if (!adapted_luminosity_next->create(1, 1, 1, D3DFMT_R32F, D3DFMT_R32F, 0))
        return 0;

    adapted_luminosity_next->clear(0);

    if (!blitter || !blitter->initialize())
        return 0;

    return bind_filter_programs() ? hdr_levels : 0;
}

// sub_75ED90
bool post_process::system::bind_filter_programs() {
    shader_resource_manager* programs = treyarch::references::shader_resource_manager.read();

    struct filter_program {
        filter*     value;
        const char* name;
    };

    const filter_program bindings[] { { luminosity_filter,                 "luminosity"                 },
                                      { gaussian_blur_3x3_filter,          "gaussian_blur_3x3"          },
                                      { gaussian_blur_7x7_filter,          "gaussian_blur_7x7"          },
                                      { gaussian_blur_15x15_filter,        "gaussian_blur_15x15"        },
                                      { poisson_disc_blur_filter,          "poisson_disc_blur"          },
                                      { luminosity_range_avg_filter,       "luminosity_range_avg"       },
                                      { luminosity_range_avg_accum_filter, "luminosity_range_avg_accum" },
                                      { adapt_filter,                      "adapt"                      },
                                      { blend_filter,                      "blend"                      },
                                      { bloom_filter,                      "bloom"                      },
                                      { radial_blur_filter,                "radial_blur"                },
                                      { depth_of_field_blur_filter,        "depth_of_field_blur"        },
                                      { depth_of_field_filter,             "depth_of_field"             },
                                      { spherize_filter,                   "spherize"                   },
                                      { vignette_filter,                   "vignette"                   },
                                      { noise_filter,                      "noise"                      },
                                      { color_adjust_filter,               "color_adjust"               } };

    for (const filter_program &binding : bindings) {
        IDirect3DPixelShader9** program = programs->find_pixel_program(binding.name);

        if (!program)
            return false;

        binding.value->program = *program;
    }

    if (!blitter->texture_program)
        return false;

    down_sample->program = *blitter->texture_program;

    return true;
}

// sub_75EB00
void post_process::system::release() {
    if (blitter) {
        blitter->release();
        blitter = nullptr;
    }

    if (hdr_targets) {
        hdr_targets->destroy();
        hdr_targets = nullptr;
    }

    if (ldr_targets) {
        ldr_targets->destroy();
        ldr_targets = nullptr;
    }

    if (average_luminosity) {
        average_luminosity->~surface_texture();
        memory::heap::free(average_luminosity);
        average_luminosity = nullptr;
    }

    if (adapted_luminosity) {
        adapted_luminosity->~surface_texture();
        memory::heap::free(adapted_luminosity);
        adapted_luminosity = nullptr;
    }

    if (secondary_ldr_targets_0) {
        secondary_ldr_targets_0->destroy();
        secondary_ldr_targets_0 = nullptr;
    }

    if (secondary_ldr_targets_1) {
        secondary_ldr_targets_1->destroy();
        secondary_ldr_targets_1 = nullptr;
    }

    if (adapted_luminosity_next) {
        adapted_luminosity_next->~surface_texture();
        memory::heap::free(adapted_luminosity_next);
        adapted_luminosity_next = nullptr;
    }

    if (noise_texture.resource && !noise_texture.resource->Release())
        noise_texture.resource = nullptr;

    if (damage_texture)
        damage_texture = nullptr;

    // retail's order, not the declaration order
    filter** filters[] { &luminosity_filter,
                         &luminosity_range_avg_filter,
                         &luminosity_range_avg_accum_filter,
                         &adapt_filter,
                         &blend_filter,
                         &bloom_filter,
                         &radial_blur_filter,
                         &depth_of_field_blur_filter,
                         &depth_of_field_filter,
                         &spherize_filter,
                         &vignette_filter,
                         &noise_filter,
                         &color_adjust_filter,
                         (filter**)&down_sample,
                         &gaussian_blur_3x3_filter,
                         &gaussian_blur_7x7_filter,
                         &gaussian_blur_15x15_filter,
                         &poisson_disc_blur_filter };

    for (filter** slot : filters) {
        if (*slot) {
            (*slot)->destroy();
            *slot = nullptr;
        }
    }

    if (queue) {
        filter_queue* released = queue;
        released->clear();
        memory::heap::free(released);
        queue = nullptr;
    }
}

// sub_72F5D0
void post_process::system::set_luminosity(const luminosity_parameters* value, u32 duration) {
    const luminosity_parameters defaults { 2.0f, 32.0f };

    luminosity.set(value ? *value : defaults, duration);
}

// sub_72F6B0
void post_process::system::set_blur(const blur_parameters* value, u32 duration) {
    const blur_parameters defaults { 1, 3, 0 };

    blur.previous = blur.current;
    blur.target   = value ? *value : defaults;

    if (blur.target.radius > 7)
        blur.target.radius = 7;

    if (blur.target.iterations > 8)
        blur.target.iterations = 8;

    blur.start(duration);
}

// sub_72F7C0
void post_process::system::set_poisson_disc_blur(const poisson_disc_blur_parameters* value, u32 duration) {
    const poisson_disc_blur_parameters defaults { 1, 3.0f };

    poisson_disc_blur.previous = poisson_disc_blur.current;
    poisson_disc_blur.target   = value ? *value : defaults;

    if (poisson_disc_blur.target.iterations > 8)
        poisson_disc_blur.target.iterations = 8;

    poisson_disc_blur.start(duration);
}

// sub_72F8B0
void post_process::system::set_bloom(const bloom_parameters* value, u32 duration) {
    const bloom_parameters defaults { { 1.0f, 1.0f, 0.0f, 0.255f, 31.0f, 0.99f, 0.5f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f } };

    bloom.set(value ? *value : defaults, duration);
}

// sub_72FA00
void post_process::system::set_radial_blur(const radial_blur_parameters* value, u32 duration) {
    const radial_blur_parameters defaults { 4, 0.0f, 0.0f, 0.1f, 0.0f };

    radial_blur.set(value ? *value : defaults, duration);
}

// sub_72FB30
void post_process::system::set_depth_of_field_blur(const depth_of_field_blur_parameters* value, u32 duration) {
    const depth_of_field_blur_parameters defaults { 0.5f, 0.0f, 0.0f, 3, nullptr };

    depth_of_field_blur.set(value ? *value : defaults, duration);
}

// sub_72FC50
void post_process::system::set_vignette(const vignette_parameters* value, u32 duration) {
    const vignette_parameters defaults { { 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, 0 };

    vignette.set(value ? *value : defaults, duration);
}

// sub_72FE10
void post_process::system::set_noise(const noise_parameters* value, u32 duration) {
    const noise_parameters defaults { { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f } };

    noise.set(value ? *value : defaults, duration);
}

// sub_72FF40
void post_process::system::set_color_adjust(const color_adjust_parameters* value, u32 duration) {
    const color_adjust_parameters defaults { { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f } };

    color_adjust.set(value ? *value : defaults, duration);
}

// sub_73BD50
void post_process::apply_default_parameters() {
    system* value = references::device_resources.get().system;

    if (!value)
        return;

    const blur_parameters                blur                { 1, 3, 0 };
    const poisson_disc_blur_parameters   poisson_disc_blur   { 0, 3.0f };
    const radial_blur_parameters         radial_blur         { 4, 0.0f, 0.0f, 0.1f, 0.0f };
    const depth_of_field_blur_parameters depth_of_field_blur { 10.0f, 0.0f, 0.0f, 3, nullptr };
    const vignette_parameters            vignette            { { 0.6f, 0.8f, 2.0f, 2.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.0f, 0.0f }, 0 };
    const noise_parameters               noise               { { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f } };

    const bloom_parameters bloom { { 4.0f, 1.0f, 1.0f, 0.3f, 31.0f, 1.0f, 0.5f, 0.0f,
                                     (f32)((f64)references::device_resources.get().unk_12c + 0.0),
                                     0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f } };

    value->luminosity.set({ 1.0f, 32.0f }, 0);
    value->set_blur(&blur, 0);
    value->set_poisson_disc_blur(&poisson_disc_blur, 0);
    value->adaptation_rate.set(1.0f, 0);
    value->set_bloom(&bloom, 0);
    value->set_radial_blur(&radial_blur, 0);
    value->set_depth_of_field_blur(&depth_of_field_blur, 0);
    value->spherize.set(0.0f, 0);
    value->set_vignette(&vignette, 0);
    value->set_noise(&noise, 0);
}
