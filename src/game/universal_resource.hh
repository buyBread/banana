#pragma once

#include "game/amalga/resource_types.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace ngl {
        struct mesh;
        struct texture;
    } // ngl

    class adjustment_layers_library;
    class day_sky_color;
    class environment;
    class light_behavior;
    class meshmap;
    class night_sky_color;
    class outdoors;
    class quest_descriptions;
    class spawn_item_collection;
    class spawn_point_list_collection;
    class static_spawn_item_collection;
    class tilemap;

    // finds resources by name in the current resource context
    struct universal_resource {
        static ngl::mesh* get_mesh_by_name(string_hash name);

        static void* get(amalga::e_resource_type type, string_hash name);

        // only the types specialized below exist
        template<typename T>
        static T* get(string_hash name);
    };

    template<> ngl::texture*                 universal_resource::get<ngl::texture>(string_hash name);
    template<> ngl::mesh*                    universal_resource::get<ngl::mesh>(string_hash name);
    template<> tilemap*                      universal_resource::get<tilemap>(string_hash name);
    template<> environment*                  universal_resource::get<environment>(string_hash name);
    template<> light_behavior*               universal_resource::get<light_behavior>(string_hash name);
    template<> meshmap*                      universal_resource::get<meshmap>(string_hash name);
    template<> adjustment_layers_library*    universal_resource::get<adjustment_layers_library>(string_hash name);
    template<> outdoors*                     universal_resource::get<outdoors>(string_hash name);
    template<> day_sky_color*                universal_resource::get<day_sky_color>(string_hash name);
    template<> night_sky_color*              universal_resource::get<night_sky_color>(string_hash name);
    template<> quest_descriptions*           universal_resource::get<quest_descriptions>(string_hash name);
    template<> spawn_item_collection*        universal_resource::get<spawn_item_collection>(string_hash name);
    template<> spawn_point_list_collection*  universal_resource::get<spawn_point_list_collection>(string_hash name);
    template<> static_spawn_item_collection* universal_resource::get<static_spawn_item_collection>(string_hash name);
} // treyarch
