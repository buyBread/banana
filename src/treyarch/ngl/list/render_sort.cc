#include <algorithm>

#include "treyarch/ngl/list/arena.hh"
#include "treyarch/ngl/list/render_sort.hh"

using namespace treyarch;

static bool compare_render_hash(const ngl::render_sort_entry &first,
                                const ngl::render_sort_entry &second) {

    return first.sort_key.integer < second.sort_key.integer;
}

static bool compare_render_distance(const ngl::render_sort_entry &first,
                                    const ngl::render_sort_entry &second) {

    if (first.sort_key.floating > second.sort_key.floating)
        return true;
    if (first.sort_key.floating < second.sort_key.floating)
        return false;

    return (u32)first.node < (u32)second.node;
}

template <typename compare_type>
static void sort_render_list(ngl::render_node* &head,
                             u32                count,
                             compare_type       compare) {

    ngl::list::arena_state &arena = ngl::list::references::arena.get();

    u8* saved_cursor = arena.cursor;

    auto* entries = (ngl::render_sort_entry*)
        ngl::list::allocate(sizeof(ngl::render_sort_entry) * count, 16);

    ngl::render_sort_entry* entry = entries;

    for (ngl::render_node* node = head; node; node = node->next) {
        entry->node     = node;
        entry->sort_key = node->sort_key;

        ++entry;
    }


    /* note: this might produce issues!
             VC8 had Dinkumware STL, modern MSVC may have different sorting behavior(?) */
    std::sort(entries, entries + count, compare);

    ngl::render_node* previous = nullptr;

    entry = entries + count;

    while (count) {
        --entry;

        entry->node->next = previous;
        previous          = entry->node;

        --count;
    }

    head = previous;

    arena.cursor = saved_cursor;
}

void ngl::list::sort_render_list_by_hash(render_node* &head, u32 count) {
    sort_render_list(head, count, compare_render_hash);
}

void ngl::list::sort_render_list_by_distance(render_node* &head, u32 count) {
    sort_render_list(head, count, compare_render_distance);
}
