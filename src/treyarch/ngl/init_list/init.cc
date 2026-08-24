#include "treyarch/ngl/font/init.hh"
#include "treyarch/ngl/fx/init.hh"
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

        if (shaders::registration::try_register(item))
            continue;

        item->register_item();
    }
}
