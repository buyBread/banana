#include "treyarch/ngl/d3d9/geometry_stream.hh"
#include "treyarch/ngl/font/init.hh"
#include "treyarch/ngl/fx/init.hh"
#include "treyarch/ngl/geometry_shader/geometry_shader.hh"
#include "treyarch/ngl/init_list/init.hh"
#include "treyarch/ngl/init_list/init_list.hh"
#include "treyarch/ngl/material/material.hh"
#include "treyarch/ngl/shaders/registration.hh"

using namespace treyarch;

void ngl::dispatch_init_list() {
    for (init_list* item = references::init_list_head.read(); item; item = item->next) {
        if (item == &references::font_init_list.get()) {
            font_init();
            
            continue;
        }

        if (item == &references::default_shader.get()) {
            initialize_default_material((shader*)item);

            continue;
        }

        if (item == &fx::references::init_list.get()) {
            fx::init();

            continue;
        }

        if (item == &references::morph_geometry_shader_instance.get()) {
            ((geometry_shader*)item)->geometry_shader::register_item();

            continue;
        }

        if (item == &d3d9::geometry_stream::references::init_list.get()) {
            d3d9::geometry_stream::init();

            continue;
        }

        if (item == &references::no_op_init_list.get())
            continue;

        if (shaders::registration::try_register(item))
            continue;
    }
}
