#include <cstring>

#include "treyarch/game/light/light_manager.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

light_manager::light_manager() : reserved_000{},
                                 point_count(0), point_head(nullptr), point_tail(nullptr),
                                 spot_count(0), spot_head(nullptr), spot_tail(nullptr) {}

point_light_node* light_manager::add_point(const ngl::lighting::point_light_data &data) {
    point_light_node* node = (point_light_node*)memory::heap::allocate(sizeof(point_light_node));

    if (node) {
        std::memcpy(&node->data, &data, sizeof(data));
        node->next = nullptr;

        if (point_tail)
            point_tail->next = node;
        else
            point_head = node;

        ++point_count;
        point_tail = node;
    }

    return node;
}

spot_light_node* light_manager::add_spot(const ngl::lighting::spot_light_data &data) {
    spot_light_node* node = (spot_light_node*)memory::heap::allocate(sizeof(spot_light_node));

    if (node) {
        std::memcpy(&node->data, &data, sizeof(data));
        node->next = nullptr;

        if (spot_tail)
            spot_tail->next = node;
        else
            spot_head = node;

        ++spot_count;
        spot_tail = node;
    }

    return node;
}

bool light_manager::remove_point(point_light_node* node) {
    point_light_node* previous = nullptr;

    for (point_light_node* current = point_head; current; current = current->next) {
        if (current != node) {
            previous = current;
            continue;
        }

        if (current == point_head)
            point_head = current->next;
        if (current == point_tail)
            point_tail = previous;
        if (previous)
            previous->next = current->next;

        memory::heap::free(current);
        --point_count;
        return true;
    }

    return false;
}

bool light_manager::remove_spot(spot_light_node* node) {
    spot_light_node* previous = nullptr;

    for (spot_light_node* current = spot_head; current; current = current->next) {
        if (current != node) {
            previous = current;
            continue;
        }

        if (current == spot_head)
            spot_head = current->next;
        if (current == spot_tail)
            spot_tail = previous;
        if (previous)
            previous->next = current->next;

        memory::heap::free(current);
        --spot_count;
        return true;
    }

    return false;
}
