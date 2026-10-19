#pragma once

#include "treyarch/amalga/mashable_resource_handler.hh"
#include "treyarch/amalga/resource_handler.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

// handlers that still forward to retail; each one moves out as its type gets owned (yay!)
namespace treyarch {
    class cut_scene;
    class sound_toc;
    class sound_index;

    namespace als {
        class animation_logic_system_shared;
    } // als

    namespace amalga {
        // todo: after NAL and ALS work
        template<> i32 mashable_resource_handler<als::animation_logic_system_shared>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<cut_scene>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<sound_toc>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<sound_index>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);

        struct ise_library_handler : resource_handler_base {
            i32 handle_resource(e_behavior           behavior,
                                resource_descriptor* descriptor,
                                limited_timer*       time_limit) override;
        };

        struct vertex_shader_handler : resource_handler_base {
            i32 handle_resource(e_behavior           behavior,
                                resource_descriptor* descriptor,
                                limited_timer*       time_limit) override;
        };

        struct pixel_shader_handler : resource_handler_base {
            i32 handle_resource(e_behavior           behavior,
                                resource_descriptor* descriptor,
                                limited_timer*       time_limit) override;
        };
    } // amalga
} // treyarch
