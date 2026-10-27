#include "treyarch/mm/mm.hh"
#include "treyarch/soap/message_box_manager.hh"

using namespace treyarch;

// sub_9EEC20
soap::message_box_manager* soap::message_box_manager::inst() {
    if (!references::message_box_manager.read()) {
        auto* instance = (message_box_manager*)mm::allocate(sizeof(message_box_manager));

        if (instance) {
            instance->vtable  = &references::message_box_manager_vtable.get();
            instance->unk_004 = mm::allocate(1);
        }

        references::message_box_manager.write(instance);
    }

    return references::message_box_manager.read();
}
