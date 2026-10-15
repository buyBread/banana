#include <doctest/doctest.h>

#include "mash/fixtures.hh"
#include "treyarch/shared/memory/memory.hh"

namespace mash = treyarch::mash;
using namespace tests::mash_support;

// a mash image stores connected c++ objects as bytes; unmashing repairs their pointers after those bytes are loaded.
// mash_info_struct reads the loaded image; it tracks where the next read starts in each stream.
// a read can return a pointer directly into that memory rather than copy the bytes into a new object.
// reserving a body means moving the cursor past its bytes, not allocating more memory.
// later traversal tests use those reads to turn saved relationships into usable pointers.
TEST_SUITE("mash") {
    TEST_CASE("reader splits a headered image without changing its bytes") {
        image_t<128> image;

        // a 128-byte image has a 16-byte header, normal data up to byte 80 and shared data from byte 80 onward.
        image.header(80);
        const auto original = image.bytes;

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128);

        CHECK(reader.mode == mash::UNMASH_MODE);
        CHECK(reader.state == mash::CONSTRUCTED);
        CHECK(reader.automash_crcs == nullptr);
        CHECK(reader.get_header() == image.at<mash::mash_info_struct::mash_header>(0));
        CHECK(reader.header_size() == 16);

        // normal_used counts from the image beginning, while shared_used counts from the shared stream's beginning.
        // the initial values are therefore 16 and 0 even though neither stream has read an object yet.
        CHECK(reader.get_buffer(mash::NORMAL_BUFFER) == image.bytes.data());
        CHECK(reader.get_curr_buffer_pos(mash::NORMAL_BUFFER) == image.bytes.data() + 16);
        CHECK(reader.get_buffer_size_max(mash::NORMAL_BUFFER) == 80);
        CHECK(reader.get_buffer(mash::SHARED_BUFFER) == image.bytes.data() + 80);
        CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 0);
        CHECK(reader.get_buffer_size_max(mash::SHARED_BUFFER) == 48);
        CHECK(image.bytes == original);

        const mash::mash_info_struct &constant = reader;

        CHECK(constant.get_header() == reader.get_header());
    }

    TEST_CASE("headerless readers expose the whole span as normal storage") {
        for (i32 size : {0, 1, 16, 64}) {
            CAPTURE(size);

            image_t<64> image;

            mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), size, false);

            CHECK(reader.get_header() == nullptr);
            CHECK(reader.header_size() == 0);
            CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 0);
            CHECK(reader.get_buffer_size_max(mash::NORMAL_BUFFER) == size);
            CHECK(reader.get_buffer(mash::SHARED_BUFFER) == image.bytes.data() + size);
            CHECK(reader.get_buffer_size_max(mash::SHARED_BUFFER) == 0);
        }
    }

    TEST_CASE("reader construction preserves the unresolved word and supplied mode") {
        for (auto mode : {mash::UNKNOWN_MODE, mash::MASH_MODE, mash::UNMASH_MODE, mash::ALIGN_MODE}) {
            image_t<64> image;
            alignas(mash::mash_info_struct) u8 storage[sizeof(mash::mash_info_struct)];
            std::memset(storage, 0x5A, sizeof(storage));
            auto* reader = ::new (storage)
                mash::mash_info_struct(mode, image.bytes.data(), 64, false);

            CHECK(reader->mode == mode);
            CHECK(reader->state == mash::CONSTRUCTED);
            CHECK(reader->automash_crcs == nullptr);
            for (u32 offset = 12; offset < 16; ++offset)
                CHECK(storage[offset] == 0x5A);

            reader->~mash_info_struct();
        }
    }

    TEST_CASE("a header can leave the shared stream empty") {
        image_t<64> image;
        image.header(64);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 64);

        CHECK(reader.get_buffer_size_max(mash::NORMAL_BUFFER) == 64);
        CHECK(reader.get_buffer_size_max(mash::SHARED_BUFFER) == 0);
        CHECK(reader.get_curr_buffer_pos(mash::SHARED_BUFFER) == image.bytes.data() + 64);
    }

    TEST_CASE("replacing a stream resets only that stream") {
        for (auto selected : {mash::NORMAL_BUFFER, mash::SHARED_BUFFER}) {
            image_t<128> image;
            image.header(80);
            image_t<64> replacement;

            mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128);
            reader.advance_buffer(mash::NORMAL_BUFFER, 4);
            reader.advance_buffer(mash::SHARED_BUFFER, 8);
            const auto other = selected == mash::NORMAL_BUFFER ?
                mash::SHARED_BUFFER : mash::NORMAL_BUFFER;

            u8* other_base = reader.get_buffer(other);
            i32 other_used = reader.get_buffer_size_used(other);
            i32 other_max  = reader.get_buffer_size_max(other);

            reader.set_buffer(selected, replacement.bytes.data(), 32);

            CHECK(reader.get_buffer(selected) == replacement.bytes.data());
            CHECK(reader.get_buffer_size_max(selected) == 32);
            CHECK(reader.get_buffer_size_used(selected) == 0);
            CHECK(reader.get_buffer(other) == other_base);
            CHECK(reader.get_buffer_size_used(other) == other_used);
            CHECK(reader.get_buffer_size_max(other) == other_max);
            CHECK(reader.get_header() == image.at<mash::mash_info_struct::mash_header>(0));

            reader.set_buffer(selected, nullptr, 0);

            CHECK(reader.get_buffer(selected) == nullptr);
            CHECK(reader.get_buffer_size_max(selected) == 0);
            CHECK(reader.get_buffer_size_used(selected) == 0);
        }
    }

    TEST_CASE("alignment uses absolute addresses and changes only skipped padding") {
        // alignment concerns the actual address, not just the number of bytes read so far.
        // for example, a base ending in 1 needs three skipped bytes before a four-byte-aligned value can start.
        // these loops vary both the base address and the cursor so either one can cause that gap.
        for (u32 base_offset = 0; base_offset < 16; ++base_offset) {
            for (u32 start = 0; start < 16; ++start) {
                for (u32 alignment : {1u, 2u, 4u, 8u, 16u, 32u, 64u}) {
                    for (bool shred : {false, true}) {
                        CAPTURE(base_offset);
                        CAPTURE(start);
                        CAPTURE(alignment);
                        CAPTURE(shred);

                        image_t<192> image;
                        image.bytes.fill(0x7E);
                        auto      expected = image.bytes;
                        const u32 padding  = (alignment - (base_offset + start) % alignment) % alignment;

                        if (shred)
                            std::fill_n(expected.begin() + base_offset + start, padding, 0xA1);

                        u32 used = start;

                        CHECK(mash::mash_info_struct::align_buffer(image.bytes.data() + base_offset,
                                                                  &used,
                                                                  alignment,
                                                                  shred) == (i32)padding);
                        CHECK(used == start + padding);
                        CHECK(((size_t)(image.bytes.data() + base_offset + used) % alignment) == 0);
                        CHECK(image.bytes == expected);
                    }
                }
            }
        }
    }

    TEST_CASE("only mash and align modes overwrite skipped bytes") {
        for (auto mode : {mash::UNKNOWN_MODE, mash::MASH_MODE, mash::UNMASH_MODE, mash::ALIGN_MODE}) {
            image_t<64> image;
            image.bytes.fill(0x55);

            mash::mash_info_struct reader(mode, image.bytes.data(), 64, false);

            if (mode == mash::MASH_MODE)
                reader.state = mash::INITIALIZED;

            reader.advance_buffer(mash::NORMAL_BUFFER, 1);

            CHECK(reader.align_buffer(mash::NORMAL_BUFFER, 4) == 3);

            const u8 expected = mode == mash::MASH_MODE || mode == mash::ALIGN_MODE ? 0xA1 : 0x55;

            CHECK(image.bytes[0] == 0x55);
            CHECK(image.bytes[1] == expected);
            CHECK(image.bytes[2] == expected);
            CHECK(image.bytes[3] == expected);
            CHECK(image.bytes[4] == 0x55);
            CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 0);
        }
    }

    TEST_CASE("deductive alignment scans padding and rounds the relative cursor down") {
        // some reads discover the next position from padding bytes rather than a supplied alignment.
        // this path skips consecutive 0xa1 bytes, then rounds the stream offset down to a multiple of four.
        // that last step can move backward; starting at 3 with no padding ends at 0, for example.
        for (auto selected : {mash::NORMAL_BUFFER, mash::SHARED_BUFFER}) {
            for (u32 start = 0; start < 8; ++start) {
                for (u32 padding = 0; padding < 12; ++padding) {
                    CAPTURE(selected);
                    CAPTURE(start);
                    CAPTURE(padding);

                    image_t<64> image;
                    image.bytes.fill(0x55);
                    std::fill_n(image.bytes.begin() + start, padding, 0xA1);
                    const auto original = image.bytes;

                    mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 64, false);
                    reader.set_buffer(selected, image.bytes.data(), 64);
                    reader.advance_buffer(selected, start);
                    reader.deductive_align_buffer(selected);

                    CHECK(reader.get_buffer_size_used(selected) == (i32)((start + padding) / 4 * 4));
                    CHECK(image.bytes == original);
                }
            }
        }
    }

    TEST_CASE("reads align before reserving bytes and leave the other cursor alone") {
        image_t<128> image;
        image.header(80);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128);
        reader.advance_buffer(mash::NORMAL_BUFFER, 1);

        CHECK(reader.read_from_buffer(mash::NORMAL_BUFFER, 3, 8) == image.bytes.data() + 24);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 27);
        CHECK(reader.read_from_buffer(mash::SHARED_BUFFER, 0, 16) == image.bytes.data() + 80);
        CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 0);
        CHECK(reader.read_from_buffer(mash::NORMAL_BUFFER, 0, 4) == image.bytes.data() + 28);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 28);
    }

    TEST_CASE("zero read alignment selects deduction even for a zero sized read") {
        // zero selects padding detection; it does not ask the reader to leave alignment alone.
        image_t<64> image;
        image.bytes.fill(0x55);
        std::fill_n(image.bytes.begin() + 1, 6, 0xA1);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 64, false);
        reader.advance_buffer(mash::NORMAL_BUFFER, 1);

        CHECK(reader.read_from_buffer(mash::NORMAL_BUFFER, 0, 0) == image.bytes.data() + 4);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 4);
        CHECK(reader.read_from_buffer(mash::NORMAL_BUFFER, 8, 0) == image.bytes.data() + 4);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 12);
    }

    TEST_CASE("typed reads use the value type alignment and size") {
        image_t<64> image;
        image.put(4, u32{0x76543210});
        image.put(8, u16{0xBEEF});

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 64, false);
        reader.advance_buffer(mash::NORMAL_BUFFER, 1);
        u32 copied = 0;
        reader.read_from_buffer(mash::NORMAL_BUFFER, copied);

        CHECK(copied == 0x76543210);

        u16* borrowed = reader.read_from_buffer(mash::NORMAL_BUFFER, (const u16*)nullptr);

        CHECK(borrowed == image.at<u16>(8));
        CHECK(*borrowed == 0xBEEF);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 10);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "duplicating normal bytes preserves header and shared state") {

        image_t<128> image;
        for (u32 index = 0; index < image.bytes.size(); ++index)
            image.bytes[index] = (u8)index;
        image.header(80);
        const auto original = image.bytes;
        void*      copied   = nullptr;
        {
            mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128);
            reader.advance_buffer(mash::NORMAL_BUFFER, 8);
            reader.advance_buffer(mash::SHARED_BUFFER, 4);

            // only the unread normal bytes are copied; the header and shared stream still refer to the old image.
            reader.dupe_buffers_for_unmash_copies();
            copied = reader.get_buffer(mash::NORMAL_BUFFER);

            REQUIRE(allocations.size() == 1);
            CHECK(allocations[0].size == 56);
            CHECK(allocations[0].alignment == 64);
            CHECK(allocations[0].flags == 0);
            CHECK(allocations[0].allocator == tests::plain_allocator);
            CHECK((size_t)copied % 64 == 0);
            CHECK(std::memcmp(copied, image.bytes.data() + 24, 56) == 0);
            CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 0);

            // the recorded maximum remains the old value even though the new allocation contains only 56 bytes.
            CHECK(reader.get_buffer_size_max(mash::NORMAL_BUFFER) == 80);
            CHECK(reader.get_buffer(mash::SHARED_BUFFER) == image.bytes.data() + 80);
            CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 4);
            CHECK(reader.get_buffer_size_max(mash::SHARED_BUFFER) == 48);
            CHECK(reader.get_header() == image.at<mash::mash_info_struct::mash_header>(0));

            *(u8*)copied = 0xFF;

            CHECK(image.bytes == original);
        }
        // destroying the reader does not release this copy; its caller still has to free it.
        CHECK(live_count(tests::plain_allocator) == 1);

        treyarch::memory::free(copied);

        CHECK(live_count(tests::plain_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "an empty unread normal span still requests an aligned zero byte copy") {

        image_t<64> image;

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 16, false);
        reader.advance_buffer(mash::NORMAL_BUFFER, 16);
        reader.dupe_buffers_for_unmash_copies();

        REQUIRE(allocations.size() == 1);
        CHECK(allocations[0].size == 0);
        CHECK(allocations[0].alignment == 64);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 0);
        CHECK(reader.get_buffer_size_max(mash::NORMAL_BUFFER) == 16);
    }
}
