#include "treyarch/ngl/lighting/light_node.hh"
#include "treyarch/ngl/lighting/references.hh"
#include "treyarch/ngl/list/arena.hh"

using namespace treyarch;

// sub_9E4E10
ngl::lighting::light_node* ngl::lighting::add_directional_light(      u32            category,
                                                                const math::vector4* direction,
                                                                const math::vector4* color) {

    auto* data = (directional_light_data*)list::allocate(sizeof(directional_light_data), 16);

    if (!data)
        return nullptr;

    data->direction = *direction;
    data->color.set_x(color->get_x());
    data->color.set_y(color->get_y());
    data->color.set_z(color->get_z());
    data->color.set_w(1.0f);

    auto* node = (light_node*)list::allocate(sizeof(light_node), 16);

    if (!node)
        return nullptr;

    node->type           = light_directional;
    node->node_data      = data;
    node->light_category = category;

    if (category & light_category_general) {
        light_context* context = references::current_context.read();

        node->next         = context->head.next;
        context->head.next = node;
    }

    return node;
}
