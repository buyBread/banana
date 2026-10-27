#include <new>

#include "game/light/deferred_lights.hh"
#include "game/light/references.hh"
#include "vc8/vector.hh"
#include "treyarch/mm/heap.hh"

namespace treyarch { namespace deferred_lights {
    // sub_7A77F0
    void queue_point(point_light_node* node) {
        auto* queue = references::deferred_point_lights.read();

        if (!queue) {
            queue = (vc8::vector<point_light_node*>*)memory::heap::allocate(sizeof(*queue));

            if (queue)
                new (queue) vc8::vector<point_light_node*>();

            references::deferred_point_lights.write(queue);
        }

        if (node)
            queue->push_back(node);
    }

    // sub_7A7870
    void queue_spot(spot_light_node* node) {
        auto* queue = references::deferred_spot_lights.read();

        if (!queue) {
            queue = (vc8::vector<spot_light_node*>*)memory::heap::allocate(sizeof(*queue));

            if (queue)
                new (queue) vc8::vector<spot_light_node*>();

            references::deferred_spot_lights.write(queue);
        }

        if (node)
            queue->push_back(node);
    }

    // sub_7A5130
    void drain() {
        auto* point_queue = references::deferred_point_lights.read();

        if (point_queue) {
            for (point_light_node** node = point_queue->begin(); node != point_queue->end(); ++node)
                references::light_manager.read()->remove_point(*node);

            point_queue->clear();
        }

        auto* spot_queue = references::deferred_spot_lights.read();

        if (spot_queue) {
            for (spot_light_node** node = spot_queue->begin(); node != spot_queue->end(); ++node)
                references::light_manager.read()->remove_spot(*node);

            spot_queue->clear();
        }
    }
}} // treyarch::deferred_lights
