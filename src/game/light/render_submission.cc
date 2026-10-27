#include <cmath>

#include "game/light/deferred_lights.hh"
#include "game/light/light_source.hh"
#include "game/light/references.hh"
#include "game/light/render_submission.hh"
#include "treyarch/ngl/lighting/light_data.hh"
#include "treyarch/ngl/lighting/light_node.hh"

namespace treyarch { namespace light_render_submission {
    using radius_method = f32 (__thiscall*)(light_source* self);

    // inlined into sub_9772D0; virtual slot 142
    f32 get_light_radius(light_source* source) {
        auto method = (radius_method)source->vtable[142];

        return method(source);
    }

    // inlined into sub_9772D0
    void submit_point_light(light_source* source) {
        ngl::lighting::point_light_data light;

        const color &diffuse = source->properties->diffuse_color;

        light.position = source->my_abs_po->get_matrix().w;
        light.position.set_w(get_light_radius(source));

        light.color = math::vector4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
        light.color.set_w(1.0f);

        auto* result = references::light_manager.read()->add_point(light);

        deferred_lights::queue_point(result);
    }

    // inlined into sub_9772D0
    void submit_spot_light(light_source* source) {
        ngl::lighting::spot_light_data light;

        const matrix4x4 &transform = source->my_abs_po->get_matrix();
        const color     &diffuse   = source->properties->diffuse_color;

        light.position = transform.w;
        light.position.set_w(get_light_radius(source));

        light.direction.set_x(transform.z.x);
        light.direction.set_y(transform.z.y);
        light.direction.set_z(transform.z.z);

        light.angles.set_x((f32)std::cos((f64)source->properties->inner_angle * 0.01745329300562541));
        light.angles.set_y((f32)std::cos((f64)source->properties->outer_angle * 0.01745329300562541));

        light.color = math::vector4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
        light.color.set_w(1.0f);

        auto* result = references::light_manager.read()->add_spot(light);

        deferred_lights::queue_spot(result);
    }

    // inlined into sub_9772D0
    void submit_directional_light(light_source* source) {
        const color &diffuse = source->properties->diffuse_color;

        math::vector4 direction_source = source->my_abs_po->get_matrix().y;
        math::vector4 direction;
        math::vector4 color(diffuse.r, diffuse.g, diffuse.b, diffuse.a);

        direction_source.set_w(0.0f);

        direction = -direction_source;
        direction.set_w(0.0f);

        ngl::lighting::add_directional_light( ngl::lighting::light_category_general,
                                             &direction,
                                             &color);
    }

}} // treyarch::light_render_submission
