#include "treyarch/ngl/fx/render_node.hh"
#include "treyarch/ngl/list/render_node.hh"

#ifdef DEBUG
    #include <unordered_set>

    #include "banana/logging.hh"
#endif

using namespace treyarch;

void ngl::render_node::render() {
    if (vtable == (void*)&fx::references::render_node_vtable.get()) {
        fx::render((fx::render_node*)this);

        return;
    }

    /*
        fallback
    */

    using render_function = void(__thiscall*)(render_node*);

#ifndef DEBUG
    ((render_function*)vtable)[2](this);
#else
    static std::unordered_set<void*> seen;
    static void* last = nullptr; // kill performance on debug builds slightly less

    render_function fn = ((render_function*)vtable)[2];

    auto* address = (void*)fn;

    if (address != last) {
        last = address;

        if (seen.insert(address).second)
            banana::log.wrn("unowned render node: {:p}", address);
    }

    fn(this);
#endif
}
