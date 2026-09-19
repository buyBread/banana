#include <new>

#include "treyarch/game/light/deferred_lights.hh"
#include "treyarch/game/light/references.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "treyarch/shared/memory/heap.hh"

namespace treyarch { namespace deferred_lights {
    void queue_point(point_light_node* node) {
        auto* queue = references::deferred_point_lights.read();

        if (!queue) {
            queue = (dinkumware::vector<point_light_node*>*)memory::heap::allocate(sizeof(*queue));

            if (queue)
                new (queue) dinkumware::vector<point_light_node*>();

            references::deferred_point_lights.write(queue);
        }

        if (node)
            queue->push_back(node);
    }

    void queue_spot(spot_light_node* node) {
        auto* queue = references::deferred_spot_lights.read();

        if (!queue) {
            queue = (dinkumware::vector<spot_light_node*>*)memory::heap::allocate(sizeof(*queue));

            if (queue)
                new (queue) dinkumware::vector<spot_light_node*>();

            references::deferred_spot_lights.write(queue);
        }

        if (node)
            queue->push_back(node);
    }

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
