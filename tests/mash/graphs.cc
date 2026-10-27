#include <doctest/doctest.h>

#include "mash/fixtures.hh"
#include "game/amalga/mashable_resource_handler.hh"
#include "game/amalga/resource_amalgatoc.hh"
#include "treyarch/mash/unmash.hh"

namespace mash   = treyarch::mash;
namespace amalga = treyarch::amalga;
using namespace tests::mash_support;

// everything from the other files comes together here, with real types from the game instead of stand-ins.
// each of these types decides for itself which bytes it reads next, through its own unmash method.
// that means an image only makes sense to the exact types that wrote it, so the layouts below follow those types field by field.
// once loaded, every pointer still leads somewhere inside the image; nothing gets its own allocation.
// amalga is the game's resource system, and amalga.toc is the table of contents it loads first, listing every pack the game has.
namespace tests { namespace mash_support {
    // a fake resource that writes down what it sees at each step of loading and unloading.
    // the handler sets self after construction, which is how the game hands a loaded resource its own address.
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

    // a resource handler expects a table of per-type records and a descriptor, which says where a resource's bytes are.
    class c_resource_fixture : public tests::c_memory_fixture {

    public:
        std::array<amalga::resource_type_record, (size_t)amalga::e_resource_type::count> types {};

        trace_t                     trace;
        amalga::resource_descriptor descriptor {};

        c_resource_fixture();

        // lays out a probe at 16, right after the header, and points the descriptor at the image.
        void put_probe(image_t<256> &image);

    private:
        c_reference_binding
            <std::array<amalga::resource_type_record, (size_t)amalga::e_resource_type::count>> m_types_binding;
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

tests::mash_support::c_resource_fixture::c_resource_fixture() : m_types_binding(amalga::references::resource_type_records, &types) {}

void tests::mash_support::c_resource_fixture::put_probe(image_t<256> &image) {
    image.header(128);

    // the probe's second field is its trace pointer.
    image.put(20, &trace);

    descriptor.type         = (amalga::e_resource_type)9;
    descriptor.raw_payload  = image.bytes.data();
    descriptor.payload_size = 256;
}

TEST_SUITE("mash") {
    TEST_CASE_FIXTURE(c_string_fixture,
                      "a small table of contents loads its name, its pack list and its memory map with all twelve partitions") {

        // this one lists a single pack and a single memory map, and the map has twelve partitions.
        // the names are short and easy to spot, so it's simple to check where each loaded pointer ended up.
        image_t<1024> image;
        image.header(512);

        // the root's plain fields; a version, some flags, which memory map to use and two buffer sizes.
        image.put(16,  u32{7});
        image.put(36,  u32{0x42});
        image.put(92,  i32{0});
        image.put(96,  u32{1234});
        image.put(100, u32{5678});

        // the root ends at 104 and its name's characters come next, terminating zero included.
        // alignment then puts the pack list's one pointer slot at 112, and loading points it at the pack that follows at 116.
        std::memcpy(image.bytes.data() + 104, "GAME", 5);
        image.put(116, u32{0x10203040});
        image.put(132, u32{0x50607080});
        image.put(140, u32{2000});
        image.put(144, u32{3000});

        // the pack ends at 188 and its name follows; the memory map list's pointer slot lands at 196 after alignment.
        // the map itself is at 200, and its name is a fixed size array inside its own bytes rather than a string after them.
        std::memcpy(image.bytes.data() + 188, "PACK", 5);
        std::memcpy(image.bytes.data() + 200, "MAIN", 5);

        for (u32 index = 0; index < 12; ++index) {
            image.put(220 + 20 * index,
                      amalga::resource_memory_map_partition { (amalga::e_resource_partition)index,
                                                              (amalga::e_partition_type)(index % 3),
                                                              { 100 + index, 200 + index },
                                                              3 + index });
        }

        // the shared stream holds four numbers, in the order loading asks for them.
        // the root's name length, the pack count, the pack's name length and the memory map count.
        // lengths don't count the terminating zero, so both four letter names take five bytes in the normal stream.
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
            CAPTURE(index);

            const auto &partition = map->memory_map_partitions[index];

            CHECK((u32)partition.partition_enum == index);
            CHECK((u32)partition.partition_type == index % 3);
            CHECK(partition.slot_size[0] == 100 + index);
            CHECK(partition.slot_size[1] == 200 + index);
            CHECK(partition.number_of_slots == 3 + index);
        }

        // both lists measure their image bytes from where they sit inside the root, at 52 and 72.
        CHECK(root->resource_buffer_size[0] == 1234);
        CHECK(root->resource_buffer_size[1] == 5678);
        CHECK(root->pack_entries.mash_image_offset == 196 - 52);
        CHECK(root->memory_maps.mash_image_offset == 460 - 72);
        CHECK(string_count == 2);
        CHECK(allocations.empty());

        // undo the two strings' construction before the image goes away; neither owns its characters, so nothing is freed.
        root->name.destruct_mashed_class();
        pack->name.destruct_mashed_class();

        CHECK(free_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "an empty table of contents still constructs its empty name, and allocates nothing") {

        image_t<256> image;
        image.header(192);

        auto* root = mash::unmash_in_place<amalga::resource_amalgatoc>(image.bytes.data(), 256);

        CHECK(root->name.empty());
        CHECK(root->name.c_str() == empty_string());
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
                      "a descriptor's optional extension only loads when its marker byte is 0xaf") {

        // an optional member gets a marker byte in the shared stream; 0xaf means it's there and anything else means it was null.
        for (u8 marker : {u8{0}, u8{1}, mash::member_class_mashed, u8{0xFF}}) {
            CAPTURE(marker);

            image_t<256> image;
            image.header(192);
            image.put(192, marker);

            auto* descriptor = mash::unmash_in_place<amalga::resource_descriptor>(image.bytes.data(), 256);

            if (marker == mash::member_class_mashed) {
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
                      "a descriptor's extension loads the pointer vector inside it") {

        image_t<512> image;
        image.header(256);

        // the marker is one byte, so the vector's count after it skips ahead to the next multiple of four.
        image.put(256, mash::member_class_mashed);
        image.put(260, u32{1});
        image.put(96,  u32{77});

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
                      "a resource directory reads its optional member's marker after its descriptor count") {

        // the directory's count comes first in the shared stream at 1536, so the marker is read from 1540.
        for (u8 marker : {u8{0}, mash::member_class_mashed}) {
            CAPTURE(marker);

            image_t<2048> image;
            image.header(1536);
            image.put(16, u32{0x1234});
            image.put(1540, marker);
            image.put(1000, u8{0x7C});

            auto* directory = mash::unmash_in_place<amalga::resource_directory>(image.bytes.data(), 2048);

            CHECK(directory->handle == 0);
            CHECK(directory->descriptors.size == 0);
            CHECK(directory->descriptors.data == nullptr);

            if (marker == mash::member_class_mashed) {
                REQUIRE(directory->unk_3c4 != nullptr);
                CHECK(directory->unk_3c4 == image.at<amalga::resource_directory_unk_3c4>(1000));
                CHECK(directory->unk_3c4->unk_00 == 0x7C);
            } else
                CHECK(directory->unk_3c4 == nullptr);

            CHECK(allocations.empty());
        }
    }

    TEST_CASE_FIXTURE(c_resource_fixture,
                      "a resource handler flags the image, loads it, hands it its address and unloads that same image") {

        image_t<256> image;
        put_probe(image);

        // raw_payload is where the resource's bytes start; adjusted_payload is the address the handler hands over through self.
        // giving the two different values shows that loading still goes by the bytes at raw_payload.
        descriptor.adjusted_payload = image.bytes.data() + 64;

        amalga::mashable_resource_handler<resource_probe_t> handler;

        CHECK(handler.handle_resource(amalga::resource_handler_base::load, &descriptor, nullptr) == 0);

        auto* root = image.at<resource_probe_t>(16);

        // in_use marks the image as already unmashed, since loading overwrites it and can't be done twice.
        // it's switched on before the probe's unmash runs, and the bytes around it stay untouched.
        CHECK(root->self == image.bytes.data() + 64);
        CHECK(root->unpublished_at_construction);
        CHECK(root->in_use_at_unmash == 1);
        CHECK(root->header->in_use == 1);
        CHECK(root->header->unk_0d == 0x1E);
        CHECK(root->header->unk_0e == 0x49);
        CHECK(root->header->unk_0f == 0x28);
        REQUIRE(trace.events.size() == 2);
        CHECK(trace.events[0].kind == event_unmash);
        CHECK(trace.events[1].kind == event_construct);

        // unloading finds the root through the image's header, not through the address handed out earlier, so changing it here makes no difference.
        // in_use is switched back off before the probe's teardown runs.
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

    TEST_CASE_FIXTURE(c_resource_fixture,
                      "the handler's static construct hands back the loaded root") {

        image_t<256> image;
        put_probe(image);

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
