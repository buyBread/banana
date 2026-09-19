#include <cmath>

#include "treyarch/game/light/deferred_lights.hh"
#include "treyarch/game/light/light_source.hh"
#include "treyarch/game/light/references.hh"
#include "treyarch/game/light/render_submission.hh"
#include "treyarch/ngl/lighting/light_data.hh"
#include "treyarch/ngl/lighting/light_node.hh"

namespace treyarch { namespace light_render_submission {
    using radius_method = f32 (__thiscall*)(light_source* self);

    f32 get_light_radius(light_source* source) {
        auto method = (radius_method)source->vtable[142];

        return method(source);
    }

    void submit_point_light(light_source* source) {
        ngl::lighting::point_light_data light;

        light.position   = source->my_abs_po->matrix.w;
        light.position.w = get_light_radius(source);

        light.color   = source->properties->diffuse_color;
        light.color.w = 1.0f;

        auto* result = references::light_manager.read()->add_point(light);

        deferred_lights::queue_point(result);
    }

    void submit_spot_light(light_source* source) {
        ngl::lighting::spot_light_data light;

        light.position = source->my_abs_po->matrix.w;
        light.position.w = get_light_radius(source);

        light.direction.x = source->my_abs_po->matrix.z.x;
        light.direction.y = source->my_abs_po->matrix.z.y;
        light.direction.z = source->my_abs_po->matrix.z.z;

        light.angles.x = (f32)std::cos((f64)source->properties->inner_angle * 0.01745329300562541);
        light.angles.y = (f32)std::cos((f64)source->properties->outer_angle * 0.01745329300562541);

        light.color = source->properties->diffuse_color;
        light.color.w = 1.0f;

        auto* result = references::light_manager.read()->add_spot(light);

        deferred_lights::queue_spot(result);
    }

    void submit_directional_light(light_source* source) {
        vector4 direction_source = source->my_abs_po->matrix.y;
        vector4 direction;
        vector4 color = source->properties->diffuse_color;

        direction_source.w = 0.0f;

        direction = -direction_source;
        direction.w = 0.0f;

        ngl::lighting::add_directional_light( ngl::lighting::light_category_general,
                                             &direction,
                                             &color);
    }

}} // treyarch::light_render_submission
