#include <cstring>

#include "treyarch/ngl/lighting/context.hh"
#include "treyarch/ngl/lighting/context_registry.hh"
#include "treyarch/ngl/list/arena.hh"
#include "treyarch/ngl/scene/scene.hh"

using namespace treyarch;

ngl::lighting::light_context* ngl::lighting::create_context() {
    auto* context = (light_context*)list::allocate(sizeof(light_context), 16);

    if (!context)
        return nullptr;

    std::memset(context, 0, sizeof(light_context));

    context->head.next           = &context->head;
    context->projector_head.next = &context->projector_head;
    context->ambient[0]          = 1.0f;
    context->ambient[1]          = 1.0f;
    context->ambient[2]          = 1.0f;
    context->ambient[3]          = 1.0f;

    references::current_context.write(context);
    register_context(context);

    return context;
}

ngl::lighting::light_context* ngl::lighting::select_context(light_context* context) {
    light_context* previous = references::current_context.read();

    references::current_context.write(context);
    
    return previous;
}

ngl::scene* ngl::lighting::set_scene_context(light_context* context,
                                              scene*        value) {

    value->light_context = context;
    return value;
}
