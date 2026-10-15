#include <doctest/doctest.h>

#include "mash/fixtures.hh"
#include "treyarch/amalga/mashable_resource_handler.hh"
#include "treyarch/amalga/resource_amalgatoc.hh"
#include "treyarch/shared/mash/unmash.hh"

namespace mash   = treyarch::mash;
namespace amalga = treyarch::amalga;
using namespace tests::mash_support;

// a resource image brings together the reader, strings and collections tested in the other files.
// its root object's unmash method follows its members into more objects, forming the connected structure used by the game.
// these tests build small examples of the actual resource types, so their own methods decide which bytes are read next.
// the resulting pointers still lead into the image; loading the resource does not give every member its own allocation.
namespace tests { namespace mash_support {
    // the handler writes self after construction; the callbacks record what is visible at each stage.
    struct resource_probe_t {
        u8*                                  self;
        trace_t*                             trace;
        mash::mash_info_struct::mash_header* header;
        u32                                  in_use_at_unmash;
        u32                                  in_use_at_teardown;
        bool                                 unpublished_at_construction;

        void unmash(mash::mash_info_struct* reader,
                    void*                   parent,
                    mash::buffer_type       buffer);

        void construct_mashed_class();
        void destruct_mashed_class();
    };
}}

void tests::mash_support::resource_probe_t::unmash(mash::mash_info_struct* reader,
                                                   void*                   parent,
                                                   mash::buffer_type       buffer) {

    header           = reader->get_header();
    in_use_at_unmash = header->in_use;
    trace->push(event_unmash, 1, this, parent, buffer, reader);
}

void tests::mash_support::resource_probe_t::construct_mashed_class() {
    unpublished_at_construction = self == nullptr;
    trace->push(event_construct, 1, this);
}

void tests::mash_support::resource_probe_t::destruct_mashed_class() {
    in_use_at_teardown = header->in_use;
    trace->push(event_destruct_mashed, 1, this);
}

TEST_SUITE("mash") {
    TEST_CASE_FIXTURE(c_string_fixture,
                      "a synthetic TOC reconstructs strings pointer tables and partition arrays") {

        // the table of contents describes packs and the memory maps used to load them.
        // this example has one pack, one map and twelve partitions; the names make their loaded pointers easy to check.
        image_t<1024> image;
        image.header(512);
        image.put(16,  u32{7});
        image.put(36,  u32{0x42});
        image.put(92,  i32{0});
        image.put(96,  u32{1234});
        image.put(100, u32{5678});

        // the root body ends at byte 104; its name's characters come next, including the terminating zero.
        // alignment leaves the next four-byte pointer slot at 112; unmashing will make it point at the pack at 116.
        std::memcpy(image.bytes.data() + 104, "GAME", 5);
        image.put(116, u32{0x10203040});
        image.put(132, u32{0x50607080});
        image.put(140, u32{2000});
        image.put(144, u32{3000});

        // the pack body ends at 188 and its name follows; the next pointer slot is at 196 after alignment.
        // that slot leads to the map at 200, whose fixed-size name lives inside its body rather than after it.
        std::memcpy(image.bytes.data() + 188, "PACK", 5);
        std::memcpy(image.bytes.data() + 200, "MAIN", 5);

        for (u32 index = 0; index < 12; ++index) {
            image.put(220 + 20 * index,
                      amalga::resource_memory_map_partition { (amalga::e_resource_partition)index,
                                                              (amalga::e_partition_type)(index % 3),
                                                              { 100 + index, 200 + index },
                                                              3 + index });
        }

        // shared values follow the order of the member visits; root name length, pack count, pack name length, map count.
        // each length excludes the terminating zero; both names have four letters and take five character bytes.
        // the characters and object bodies stay in the normal stream while these four reads advance the shared cursor.
        image.put(512, i32{4});
        image.put(516, u32{1});
        image.put(520, i32{4});
        image.put(524, u32{1});

        auto* root = mash::unmash_in_place<amalga::resource_amalgatoc>(image.bytes.data(), 1024);

        CHECK(root == image.at<amalga::resource_amalgatoc>(16));
        CHECK(root->versions.pack_version == 7);
        CHECK(root->flags == 0x42);
        CHECK(root->name == "GAME");
        CHECK(root->name.m_data == image.at<char>(104));
        CHECK(root->name.m_source_slab == source_slab);
        REQUIRE(root->pack_entries.size == 1);
        CHECK(root->pack_entries.capacity == 1);
        REQUIRE(root->pack_entries.data == image.at<amalga::resource_amalgatoc_pack_entry*>(112));

        auto* pack = root->pack_entries.data[0];

        REQUIRE(pack == image.at<amalga::resource_amalgatoc_pack_entry>(116));
        CHECK(pack->name == "PACK");
        CHECK(pack->name.m_data == image.at<char>(188));
        CHECK(pack->name_hash.source_hash_code == 0x10203040);
        CHECK(pack->group_hash.source_hash_code == 0x50607080);
        CHECK(pack->data_size[0] == 2000);
        CHECK(pack->data_size[1] == 3000);
        REQUIRE(root->memory_maps.size == 1);
        REQUIRE(root->memory_maps.data == image.at<amalga::resource_memory_map*>(196));

        auto* map = root->memory_maps.data[0];

        REQUIRE(map == image.at<amalga::resource_memory_map>(200));
        CHECK(doctest::String(map->map_name_str) == "MAIN");
        REQUIRE(map->memory_map_partitions == image.at<amalga::resource_memory_map_partition>(220));
        for (u32 index = 0; index < 12; ++index) {
            const auto &partition = map->memory_map_partitions[index];

            CHECK((u32)partition.partition_enum == index);
            CHECK((u32)partition.partition_type == index % 3);
            CHECK(partition.slot_size[0] == 100 + index);
            CHECK(partition.slot_size[1] == 200 + index);
            CHECK(partition.number_of_slots == 3 + index);
        }

        CHECK(root->resource_buffer_size[0] == 1234);
        CHECK(root->resource_buffer_size[1] == 5678);
        CHECK(root->pack_entries.mash_image_offset == 196 - 52);
        CHECK(root->memory_maps.mash_image_offset == 460 - 72);
        CHECK(string_count == 2);
        CHECK(allocations.empty());

        // balance the two string construction hooks before the image goes away; neither owns its character bytes.
        root->name.destruct_mashed_class();
        pack->name.destruct_mashed_class();

        CHECK(free_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "an empty TOC constructs its empty name without allocating container storage") {

        image_t<256> image;
        image.header(192);
        auto* root = mash::unmash_in_place<amalga::resource_amalgatoc>(image.bytes.data(), 256);

        CHECK(root->name.empty());
        CHECK(root->name.c_str() == empty_storage.data() + 16);
        CHECK(root->pack_entries.size == 0);
        CHECK(root->pack_entries.data == nullptr);
        CHECK(root->memory_maps.size == 0);
        CHECK(root->memory_maps.data == nullptr);
        CHECK(root->pack_entries.mash_image_offset == 52);
        CHECK(root->memory_maps.mash_image_offset == 32);
        CHECK(string_count == 1);
        CHECK(allocations.empty());

        root->name.destruct_mashed_class();
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "optional descriptor extensions accept only the present marker") {

        for (u8 marker : {u8{0}, u8{1}, u8{0xAF}, u8{0xFF}}) {
            CAPTURE(marker);

            image_t<256> image;
            image.header(192);
            image.put(192, marker);
            auto* descriptor = mash::unmash_in_place<amalga::resource_descriptor>(image.bytes.data(), 256);

            if (marker == 0xAF) {
                REQUIRE(descriptor->extension != nullptr);
                CHECK(descriptor->extension == image.at<amalga::resource_descriptor_extension>(60));
                CHECK(descriptor->extension->unk_0c.size == 0);
                CHECK(descriptor->extension->unk_0c.data == nullptr);
                CHECK(descriptor->extension->unk_0c.mash_image_offset == 20);
            } else
                CHECK(descriptor->extension == nullptr);

            CHECK(allocations.empty());
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "descriptor extensions reconstruct their nested pointer vector") {

        image_t<512> image;
        image.header(256);

        // presence is one byte, while the following vector count starts at the next four-byte boundary.
        image.put(256, u8{0xAF});
        image.put(260, u32{1});
        image.put(96, u32{77});
        auto* descriptor = mash::unmash_in_place<amalga::resource_descriptor>(image.bytes.data(), 512);

        REQUIRE(descriptor->extension != nullptr);

        const auto &entries = descriptor->extension->unk_0c;

        CHECK(entries.size == 1);
        CHECK(entries.data == image.at<amalga::resource_descriptor_extension_entry*>(92));
        CHECK(entries.data[0] == image.at<amalga::resource_descriptor_extension_entry>(96));
        CHECK(entries.data[0]->unk_00[0] == 77);
        CHECK(entries.mash_image_offset == 132 - 72);
        CHECK(allocations.empty());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "directory traversal reads its optional byte after the descriptor count") {

        for (u8 marker : {u8{0}, u8{0xAF}}) {
            image_t<2048> image;
            image.header(1536);
            image.put(16, u32{0x1234});
            image.put(1540, marker);
            image.put(1000, u8{0x7C});
            auto* directory = mash::unmash_in_place<amalga::resource_directory>(image.bytes.data(), 2048);

            CHECK(directory->handle == 0);
            CHECK(directory->descriptors.size == 0);
            CHECK(directory->descriptors.data == nullptr);
            
            if (marker == 0xAF) {
                REQUIRE(directory->unk_3c4 != nullptr);
                CHECK(directory->unk_3c4 == image.at<amalga::resource_directory_unk_3c4>(1000));
                CHECK(directory->unk_3c4->unk_00 == 0x7C);
            } else
                CHECK(directory->unk_3c4 == nullptr);

            CHECK(allocations.empty());
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "mashable handlers mark reconstruct publish and tear down the same image") {

        std::array<amalga::resource_type_record, (size_t)amalga::e_resource_type::count> types {};
        c_reference_binding binding(amalga::references::resource_type_records, &types);
        image_t<256> image;
        image.header(128);

        trace_t trace;
        image.put(20, &trace);
        amalga::resource_descriptor descriptor {};
        descriptor.type         = (amalga::e_resource_type)9;
        descriptor.raw_payload  = image.bytes.data();
        descriptor.payload_size = 256;

        // raw_payload points at the loaded bytes; adjusted_payload is the address published through the resource's self field.
        // give the latter a different address to show that construction still visits the root at byte 16.
        descriptor.adjusted_payload = image.bytes.data() + 64;
        amalga::mashable_resource_handler<resource_probe_t> handler;

        CHECK(handler.handle_resource(amalga::resource_handler_base::load, &descriptor, nullptr) == 0);

        auto* root = image.at<resource_probe_t>(16);

        CHECK(root->self == image.bytes.data() + 64);
        CHECK(root->unpublished_at_construction);
        CHECK(root->in_use_at_unmash == 1);
        CHECK(root->header->in_use == 1);
        CHECK(root->header->unknown_0d == 0x1E);
        CHECK(root->header->unknown_0e == 0x49);
        CHECK(root->header->unknown_0f == 0x28);
        REQUIRE(trace.events.size() == 2);
        CHECK(trace.events[0].kind == event_unmash);
        CHECK(trace.events[1].kind == event_construct);

        // unload finds the root from the raw image header, even after this cached pointer has changed.
        // the flag marks the image as already unmashed; it is set before loading callbacks and cleared before cleanup.
        descriptor.adjusted_payload = image.bytes.data() + 96;

        CHECK(handler.handle_resource(amalga::resource_handler_base::unload, &descriptor, nullptr) == 0);
        CHECK(root->header->in_use == 0);
        CHECK(root->in_use_at_teardown == 0);
        REQUIRE(trace.events.size() == 3);
        CHECK(trace.events[2].kind == event_destruct_mashed);
        CHECK(trace.events[2].object == root);
        CHECK(allocations.empty());
        CHECK(frees.empty());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "static handler construction returns the reconstructed root") {

        std::array<amalga::resource_type_record, (size_t)amalga::e_resource_type::count> types {};
        c_reference_binding binding(amalga::references::resource_type_records, &types);
        image_t<256> image;
        image.header(128);

        trace_t trace;
        image.put(20, &trace);
        amalga::resource_descriptor descriptor {};
        descriptor.type             = (amalga::e_resource_type)9;
        descriptor.raw_payload      = image.bytes.data();
        descriptor.payload_size     = 256;
        descriptor.adjusted_payload = image.bytes.data() + 16;

        using handler_t = amalga::mashable_resource_handler<resource_probe_t>;
        auto* root      = handler_t::construct(&descriptor);

        REQUIRE(root == image.at<resource_probe_t>(16));
        CHECK(root->self == image.bytes.data() + 16);
        CHECK(root->in_use_at_unmash == 1);

        handler_t::destruct(&descriptor);

        CHECK(root->in_use_at_teardown == 0);
        CHECK(trace.events.size() == 3);
        CHECK(allocations.empty());
    }
}
