#include <doctest/doctest.h>

#include "mash/fixtures.hh"
#include "treyarch/shared/memory/memory.hh"

namespace mash = treyarch::mash;
using namespace tests::mash_support;

// mash_info_struct is the reader, the thing that walks an image and hands out pieces of it.
// it keeps a start address, a cursor and a size limit for each of the two streams.
// a read doesn't copy anything, it returns the address at the cursor and moves the cursor past the bytes that were asked for.
// alignment shows up a lot below.
// a four byte value usually has to start at an address divisible by four, so a read may skip a few padding bytes first.
TEST_SUITE("mash") {
    TEST_CASE("the reader splits an image at the header's shared offset and doesn't change its bytes") {
        image_t<128> image;

        // a 128 byte image; the header takes bytes 0 to 15, normal goes up to byte 80 and shared takes the rest.
        image.header(80);
        const auto original = image.bytes;

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128);

        CHECK(reader.mode == mash::UNMASH_MODE);
        CHECK(reader.state == mash::CONSTRUCTED);
        CHECK(reader.automash_crcs == nullptr);
        CHECK(reader.get_header() == image.at<mash::mash_info_struct::mash_header>(0));
        CHECK(reader.header_size() == 16);

        // both streams count from their own start; normal starts at the image's first byte, so its cursor begins past the header at 16.
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

    TEST_CASE("without a header, the whole span is normal and shared is empty") {
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

    TEST_CASE("the reader keeps whatever mode it's given and leaves its unknown word alone") {
        // the game only ever builds readers in UNMASH_MODE; the other modes belonged to the tools that wrote images.
        // bytes 12 to 15 of the reader are a field only the pc version has, and its constructor never writes to it.
        for (auto mode : {mash::UNKNOWN_MODE, mash::MASH_MODE, mash::UNMASH_MODE, mash::ALIGN_MODE}) {
            image_t<64> image;
            alignas(mash::mash_info_struct) u8 storage[sizeof(mash::mash_info_struct)];
            std::memset(storage, 0x5A, sizeof(storage));

            auto* reader = ::new (storage) mash::mash_info_struct(mode, image.bytes.data(), 64, false);

            CHECK(reader->mode == mode);
            CHECK(reader->state == mash::CONSTRUCTED);
            CHECK(reader->automash_crcs == nullptr);

            for (u32 offset = 12; offset < 16; ++offset)
                CHECK(storage[offset] == 0x5A);

            reader->~mash_info_struct();
        }
    }

    TEST_CASE("a shared offset at the very end leaves the shared stream empty") {
        image_t<64> image;
        image.header(64);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 64);

        CHECK(reader.get_buffer_size_max(mash::NORMAL_BUFFER) == 64);
        CHECK(reader.get_buffer_size_max(mash::SHARED_BUFFER) == 0);
        CHECK(reader.get_curr_buffer_pos(mash::SHARED_BUFFER) == image.bytes.data() + 64);
    }

    TEST_CASE("pointing a stream at other bytes resets that stream only") {
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

    TEST_CASE("alignment goes by the real address and only ever touches the skipped bytes") {
        // the image itself isn't promised to start on any particular boundary, so the cursor alone isn't enough.
        // for example, an image starting at an address ending in 1 needs three padding bytes before a four byte value, even at cursor 0.
        // these loops move both the image start and the cursor around, so either one can be the cause of the gap.
        // the skipped bytes can also get overwritten with 0xa1, a filler value the tools use for padding; that's the shred flag.
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
                            std::fill_n(expected.begin() + base_offset + start, padding, mash::alignment_padding);

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

    TEST_CASE("only the tool modes write filler into skipped bytes") {
        // a loading reader leaves padding alone; only the writing and layout modes fill it with 0xa1.
        for (auto mode : {mash::UNKNOWN_MODE, mash::MASH_MODE, mash::UNMASH_MODE, mash::ALIGN_MODE}) {
            image_t<64> image;
            image.bytes.fill(0x55);

            mash::mash_info_struct reader(mode, image.bytes.data(), 64, false);

            // a writing reader is expected to be fully set up before it moves its cursors.
            if (mode == mash::MASH_MODE)
                reader.state = mash::INITIALIZED;

            reader.advance_buffer(mash::NORMAL_BUFFER, 1);

            CHECK(reader.align_buffer(mash::NORMAL_BUFFER, 4) == 3);

            const u8 expected = mode == mash::MASH_MODE || mode == mash::ALIGN_MODE ?
                mash::alignment_padding : 0x55;

            CHECK(image.bytes[0] == 0x55);
            CHECK(image.bytes[1] == expected);
            CHECK(image.bytes[2] == expected);
            CHECK(image.bytes[3] == expected);
            CHECK(image.bytes[4] == 0x55);
            CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 0);
        }
    }

    TEST_CASE("deductive alignment skips 0xa1 filler, then rounds the cursor down to four") {
        // some reads don't know their alignment up front, so they find the next value by stepping over the 0xa1 filler instead.
        // after that, the cursor rounds down to a multiple of four, which can even move it backwards.
        // starting at 3 with no filler ends up at 0, for example.
        for (auto selected : {mash::NORMAL_BUFFER, mash::SHARED_BUFFER}) {
            for (u32 start = 0; start < 8; ++start) {
                for (u32 padding = 0; padding < 12; ++padding) {
                    CAPTURE(selected);
                    CAPTURE(start);
                    CAPTURE(padding);

                    image_t<64> image;
                    image.bytes.fill(0x55);
                    std::fill_n(image.bytes.begin() + start, padding, mash::alignment_padding);

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

    TEST_CASE("a read aligns first, then steps past its bytes, and the other stream stays put") {
        image_t<128> image;
        image.header(80);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128);
        reader.advance_buffer(mash::NORMAL_BUFFER, 1);

        // the cursor is at 17; aligning to 8 moves it to 24, and then the three bytes take it to 27.
        CHECK(reader.read_from_buffer(mash::NORMAL_BUFFER, 3, 8) == image.bytes.data() + 24);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 27);
        CHECK(reader.read_from_buffer(mash::SHARED_BUFFER, 0, 16) == image.bytes.data() + 80);
        CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 0);
        CHECK(reader.read_from_buffer(mash::NORMAL_BUFFER, 0, 4) == image.bytes.data() + 28);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 28);
    }

    TEST_CASE("asking for alignment 0 picks deductive alignment, even for an empty read") {
        // 0 doesn't mean "don't align"; it means "work it out from the filler".
        image_t<64> image;
        image.bytes.fill(0x55);
        std::fill_n(image.bytes.begin() + 1, 6, mash::alignment_padding);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 64, false);
        reader.advance_buffer(mash::NORMAL_BUFFER, 1);

        CHECK(reader.read_from_buffer(mash::NORMAL_BUFFER, 0, 0) == image.bytes.data() + 4);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 4);
        CHECK(reader.read_from_buffer(mash::NORMAL_BUFFER, 8, 0) == image.bytes.data() + 4);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 12);
    }

    TEST_CASE("typed reads take their size and alignment from the type") {
        image_t<64> image;
        image.put(4, u32{0x76543210});
        image.put(8, u16{0xBEEF});

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 64, false);
        reader.advance_buffer(mash::NORMAL_BUFFER, 1);

        // this overload copies the value out.
        u32 copied = 0;
        reader.read_from_buffer(mash::NORMAL_BUFFER, copied);

        CHECK(copied == 0x76543210);

        // this one hands back a pointer into the image; the null argument is only there to say which type to read.
        u16* borrowed = reader.read_from_buffer(mash::NORMAL_BUFFER, (const u16*)nullptr);

        CHECK(borrowed == image.at<u16>(8));
        CHECK(*borrowed == 0xBEEF);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 10);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "duplicating copies only the unread normal bytes, leaving the header and shared stream where they were") {

        // unmashing overwrites the image as it goes, so loading the same bytes twice needs a fresh copy first.
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

            // normal's cursor is at 24, so bytes 24 to 79 get copied into a new 64 byte aligned allocation.
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

            // the size limit stays at 80, even though the copy only holds 56 bytes.
            CHECK(reader.get_buffer_size_max(mash::NORMAL_BUFFER) == 80);
            CHECK(reader.get_buffer(mash::SHARED_BUFFER) == image.bytes.data() + 80);
            CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 4);
            CHECK(reader.get_buffer_size_max(mash::SHARED_BUFFER) == 48);
            CHECK(reader.get_header() == image.at<mash::mash_info_struct::mash_header>(0));

            *(u8*)copied = 0xFF;

            CHECK(image.bytes == original);
        }

        // the reader going away doesn't free the copy; whoever asked for it has to.
        CHECK(live_count(tests::plain_allocator) == 1);

        treyarch::memory::free(copied);

        CHECK(live_count(tests::plain_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "duplicating with nothing left to read still asks for an empty allocation") {

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
