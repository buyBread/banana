#include "treyarch/nal/anim.hh"
#include "treyarch/nal/nal.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

// inlined @ sub_9D1790
void nal::anim::link_transport_list() {
    if (transport_list)
        return;

    transport_list = get_skeleton(&transport_list_name);

    if (!transport_list)
        memory::report("NAL2 error: TransportList not found in the directory. Please report to animation programmer.");
}

// sub_9CD4A0
nal::transport_data* nal::anim::get_transport_data(u32 id) {
    for (nal::transport_data* value = transport_data; value; value = value->next) {
        if (value->id == id)
            return value;
    }

    return nullptr;
}

// sub_9CD4C0
bool nal::anim::has_transport_data(u32 id) {
    for (nal::transport_data* value = transport_data; value; value = value->next) {
        if (value->id == id)
            return true;
    }

    return false;
}
