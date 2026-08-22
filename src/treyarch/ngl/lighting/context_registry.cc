#include <stdexcept>

#include "treyarch/ngl/lighting/context_registry.hh"
#include "treyarch/shared/memory/game_heap.hh"
#include "util/memory_reference.hh"

using namespace treyarch;

static util::memory_reference<ngl::lighting::context_registry*> light_context_registry { 0x00F532A4 };

void ngl::lighting::register_context(light_context* context) {
    context_registry* registry = light_context_registry.read();

    context_registry_node* head     = registry->head;
    context_registry_node* previous = head->previous;

    auto* node = (context_registry_node*)
        memory::game_heap::allocate_small_block(sizeof(context_registry_node));

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
    context_registry* registry = light_context_registry.read();
    
    context_registry_node* head = registry->head;
    context_registry_node* node = head->next;

    head->next     = head;
    head->previous = head;

    registry->size = 0;

    while (node != head) {
        context_registry_node* next = node->next;

        memory::game_heap::free_small_block(node);
        node = next;
    }
}
