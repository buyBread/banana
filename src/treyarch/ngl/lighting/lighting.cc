#include <cstring>
#include <stdexcept>

#include "treyarch/ngl/lighting/lighting.hh"
#include "treyarch/ngl/list/list.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/mm/heap.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace lighting { namespace references {
    util::memory_reference<lighting::context_registry*> context_registry { 0x00F532A4 };
}}}} // treyarch::ngl::lighting::references

using namespace treyarch;

// sub_9E50D0
ngl::lighting::light_context* ngl::lighting::create_light_context() {
    auto* context = (light_context*)list_alloc(sizeof(light_context), 16);

    if (!context)
        return nullptr;

    std::memset(context, 0, sizeof(light_context));

    context->head.next           = &context->head;
    context->projector_head.next = &context->projector_head;
    context->ambient[0]          = 1.0f;
    context->ambient[1]          = 1.0f;
    context->ambient[2]          = 1.0f;
    context->ambient[3]          = 1.0f;

    references::build_light_context.write(context);
    register_context(context);

    return context;
}

// sub_9E4CC0
ngl::lighting::light_context* ngl::lighting::select_build_light_context(light_context* context) {
    light_context* previous = references::build_light_context.read();

    references::build_light_context.write(context);
    
    return previous;
}

// sub_9E4CB0
ngl::scene* ngl::lighting::set_light_context(light_context* context,
                                              scene*        value) {

    value->light_context = context;
    return value;
}

void ngl::lighting::register_context(light_context* context) {
    context_registry* registry = references::context_registry.read();

    context_registry_node* head     = registry->head;
    context_registry_node* previous = head->previous;

    auto* node = (context_registry_node*)
        memory::heap::allocate_small_block(sizeof(context_registry_node));

    node->next     = head;
    node->previous = previous;
    node->value    = context;

    if (registry->size == 0x3FFFFFFF)
        throw std::length_error("list<T> too long");

    ++registry->size;
    
    head->previous = node;
    previous->next = node;
}

void ngl::lighting::reset_context_registry() {
    container::clear_legacy_list(references::context_registry.read());
}

// sub_9E4E10
ngl::lighting::light_node* ngl::lighting::list_add_light(      u32            category,
                                                         const math::vector4* direction,
                                                         const math::vector4* color) {

    auto* data = (directional_light_data*)list_alloc(sizeof(directional_light_data), 16);

    if (!data)
        return nullptr;

    data->direction = *direction;
    data->color.set_x(color->get_x());
    data->color.set_y(color->get_y());
    data->color.set_z(color->get_z());
    data->color.set_w(1.0f);

    auto* node = (light_node*)list_alloc(sizeof(light_node), 16);

    if (!node)
        return nullptr;

    node->type           = light_directional;
    node->node_data      = data;
    node->light_category = category;

    if (category & light_category_general) {
        light_context* context = references::build_light_context.read();

        node->next         = context->head.next;
        context->head.next = node;
    }

    return node;
}
