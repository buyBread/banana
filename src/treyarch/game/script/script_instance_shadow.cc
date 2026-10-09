#include "retail.hh"
#include "treyarch/game/script/script_instance_shadow.hh"
#include "treyarch/shared/memory/fixed_pool.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace references {
    util::memory_reference<memory::fixed_pool> script_instance_shadow_pool { 0x00F22210 };

    util::memory_reference<void*> script_instance_shadow_vtable { 0x00BDF394 };
}} // treyarch::references

using namespace treyarch;

// inlined @ sub_825600
script_instance_shadow::script_instance_shadow() {
    retail::sub_639100((u32*)this); // arch_base::arch_base
    vtable = (void**)&references::script_instance_shadow_vtable.get();
    m_inst = nullptr;
}

// sub_8235B0
void* script_instance_shadow::operator new(std::size_t) noexcept {
    return references::script_instance_shadow_pool.get().allocate();
}
