#pragma once

namespace treyarch {
    class light_source;

    namespace light_render_submission {
        void submit_point_light(light_source* source);
        void submit_spot_light(light_source* source);
        void submit_directional_light(light_source* source);
    } // light_render_submission
} // treyarch
