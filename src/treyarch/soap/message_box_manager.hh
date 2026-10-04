#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace soap {
    // the milestone's GT_SOAP_* text keys
    enum e_message_box : i32 {
        message_box_profile_sign_in_warning,
        message_box_storage_no_device_selected,
        message_box_storage_active_device_unavailable,
        message_box_storage_data_corrupt,
        message_box_storage_data_corrupt_confirm_delete,
        message_box_storage_data_corrupt_confirm_continue_no_save,
        message_box_storage_extension_corrupt,
        message_box_storage_savedata_exists
    };

    // what game::one_time_init_stuff builds through sub_97D790; the third label is always empty
    struct message_box_definition {
        wchar_t title[64];
        wchar_t text[256];
        wchar_t labels[3][32];
    };

    class message_box_manager;

    using message_box_manager_test = bool (__thiscall*)(message_box_manager* self);
    using message_box_manager_show = bool (__thiscall*)(message_box_manager* self, e_message_box box);

    struct message_box_manager_vtable {
        void*                    reserved_000[2]; // set_boxes, frame_advance
        message_box_manager_test is_box_showing;
        void*                    reserved_00c;    // the last box's result
        message_box_manager_show show;
    };

    class message_box_manager {

    public:
        message_box_manager_vtable* vtable;
        void*                       unk_004; // a one-byte allocation

        static message_box_manager* inst();

        bool is_box_showing() {
            return vtable->is_box_showing(this);
        }

        bool show(e_message_box box) {
            return vtable->show(this, box);
        }
    };

    namespace references {
        inline util::memory_reference<message_box_manager*> message_box_manager { 0x01123D10 };

        inline util::memory_reference<message_box_manager_vtable> message_box_manager_vtable { 0x00DBC288 };
    } // references

    ASSERT_SIZEOF  (message_box_definition,         0x340);
    ASSERT_OFFSETOF(message_box_definition, text,   0x080);
    ASSERT_OFFSETOF(message_box_definition, labels, 0x280);

    ASSERT_OFFSETOF(message_box_manager_vtable, is_box_showing, 0x08);
    ASSERT_OFFSETOF(message_box_manager_vtable, show,           0x10);

    ASSERT_SIZEOF(message_box_manager, 0x08);
}} // treyarch::soap
