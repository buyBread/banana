#pragma once

#include <type_traits>

#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace mash {
    class mash_info_struct {

public:
        struct mash_header {
            u32 master_marker;
            u32 unknown_04;
            u32 shared_offset;
            u8  in_use;
            u8  unknown_0d;
            u8  unknown_0e;
            u8  unknown_0f;
        };

        io_mode      mode;
        mash_state   state;
        mash_header* header;
        u32          unknown_0c; // retail's constructor doesn't touch this PC-only word
        u8*          mash_image_ptr[MAX_BUFFER_TYPES];
        u32          buffer_size_used[MAX_BUFFER_TYPES];
        u32          buffer_size_max[MAX_BUFFER_TYPES];
        void*        automash_crcs;

        mash_info_struct(io_mode mode, u8* buffer, i32 buffer_max, bool has_header = true);
       ~mash_info_struct() = default;

        static constexpr u32 get_master_crc()        { return 0x12345678; }
        static constexpr u32 get_mash_image_sentry() { return 0x6D617368; }

              mash_header* get_header()       { return header; }
        const mash_header* get_header() const { return header; }

        void dupe_buffers_for_unmash_copies();
        void set_buffer(buffer_type buffer, u8* buffer_ram, i32 buffer_size);

        i32 get_buffer_size_used(buffer_type buffer) const {
            return (i32)buffer_size_used[buffer];
        }

        i32 get_buffer_size_max(buffer_type buffer) const {
            return (i32)buffer_size_max[buffer];
        }

        u8* get_curr_buffer_pos(buffer_type buffer) const {
            return mash_image_ptr[buffer] + buffer_size_used[buffer];
        }

        u8* get_buffer(buffer_type buffer) const {
            return mash_image_ptr[buffer];
        }

               i32 align_buffer(buffer_type buffer, i32 alignment_required);
        static i32 align_buffer(u8* buffer_ptr, u32* buffer_ptr_used, i32 alignment_required, bool shred_padding);

        void deductive_align_buffer(buffer_type buffer);
        void advance_buffer(buffer_type buffer, i32 byte_count);

        i32 header_size() const;
        void* read_from_buffer( buffer_type buffer, i32 read_size, i32 alignment_required);

        template <typename T>
        T* read_from_buffer(buffer_type buffer, const T*) {
            return (T*)read_from_buffer(buffer, (i32)sizeof(T), (i32)alignof(T));
        }

        template <typename T>
        void read_from_buffer(buffer_type buffer, T &value) {
            value = *(T*)read_from_buffer(buffer, (i32)sizeof(T), (i32)alignof(T));
        }

        template <typename T>
        void unmash_class(T*          &class_ptr,
                          void*        containing_class_ptr = nullptr,
                          buffer_type  buffer               = NORMAL_BUFFER) {

            if constexpr (std::is_polymorphic_v<T>) {
                // alignment 0 means the exact type isn't known until its serialized key becomes a vtable
                class_ptr = (T*)read_from_buffer(buffer, (i32)sizeof(T), 0);

                mash_virtual_base::fixup_vtable(class_ptr);

                // sizeof(T) only covered the declared base; the fixed vtable gives us the real tail size
                advance_buffer(buffer,
                               class_ptr->get_mash_sizeof() - (i32)sizeof(T));
            } else {
                class_ptr = (T*)read_from_buffer(
                    buffer,
                    (i32)sizeof(T),
                    (i32)alignof(T));
            }

            class_ptr->unmash(this, containing_class_ptr, buffer);
        }

        template <typename T>
        void unmash_class_in_place(T           &class_ref,
                                   void*        containing_class_ptr = nullptr,
                                   buffer_type  buffer               = NORMAL_BUFFER) {

            if constexpr (std::is_polymorphic_v<T>)
                mash_virtual_base::fixup_vtable(&class_ref);

            class_ref.unmash(this, containing_class_ptr, buffer);
        }

        template <typename T>
        static void construct_class(T* &class_ptr) {
            if constexpr (std::is_polymorphic_v<T>)
                class_ptr = (T*)mash_virtual_base::construct_class_helper(class_ptr);
            else
                class_ptr->construct_mashed_class();
        }
    };

    ASSERT_SIZEOF  (mash_info_struct::mash_header,                0x10);
    ASSERT_OFFSETOF(mash_info_struct::mash_header, master_marker, 0x00);
    ASSERT_OFFSETOF(mash_info_struct::mash_header, unknown_04,    0x04);
    ASSERT_OFFSETOF(mash_info_struct::mash_header, shared_offset, 0x08);
    ASSERT_OFFSETOF(mash_info_struct::mash_header, in_use,        0x0C);

    ASSERT_SIZEOF  (mash_info_struct,                   0x2C);
    ASSERT_OFFSETOF(mash_info_struct, mode,             0x00);
    ASSERT_OFFSETOF(mash_info_struct, state,            0x04);
    ASSERT_OFFSETOF(mash_info_struct, header,           0x08);
    ASSERT_OFFSETOF(mash_info_struct, unknown_0c,       0x0C);
    ASSERT_OFFSETOF(mash_info_struct, mash_image_ptr,   0x10);
    ASSERT_OFFSETOF(mash_info_struct, buffer_size_used, 0x18);
    ASSERT_OFFSETOF(mash_info_struct, buffer_size_max,  0x20);
    ASSERT_OFFSETOF(mash_info_struct, automash_crcs,    0x28);
}} // treyarch::mash
