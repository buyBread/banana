#include "treyarch/nal/transport_list.hh"

using namespace treyarch;

// sub_9CD4F0
nal::transport* nal::transport_list::get_transport(u32 id) {
    for (transport* value = transports; value; value = value->next) {
        if (value->id == id)
            return value;
    }

    return nullptr;
}
