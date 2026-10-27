#pragma once

namespace treyarch {
    struct point_light_node;
    struct spot_light_node;

    namespace deferred_lights {
        void queue_point(point_light_node* node);
        void queue_spot(spot_light_node* node);
        void drain();
    } // deferred_lights
} // treyarch
