#include <algorithm>
#include <cstdlib>
#include <vector>
#include <doctest/doctest.h>

#include "mash/fixtures.hh"
#include "treyarch/mash/avl_tree.hh"
#include "treyarch/mash/unmash.hh"
#include "treyarch/mm/mm.hh"

namespace mash = treyarch::mash;
using namespace tests::mash_support;

// an avl tree is a sorted tree of nodes that keeps itself balanced.
// each node has up to two children; everything down its left side is smaller than it and everything down its right side is bigger.
// balanced means the two sides of any node never differ in height by more than one level, which keeps the tree from getting deep.
// when adding a node breaks that rule, the tree fixes it with a rotation, which lifts a child up into its parent's place.
// the image doesn't save the tree's shape, only its nodes one after another.
// loading adds them back one at a time, so the shape is rebuilt from scratch, rotations and all.
// each saved node is followed by its element, and a marker byte in the shared stream says whether that element is there at all.
namespace tests { namespace mash_support {
    using record_tree = mash::avl_tree<record_t>;
    using record_node = mash::avl_node<record_t>;

    // the tree sorts its elements with <, and records sort by id.
    bool operator<(const record_t &left, const record_t &right) {
        return left.id < right.id;
    }

    // where put_tree places the tree and each saved node along with its element; a node is twenty bytes and a record is eight.
    constexpr u32 tree_offset = 16;

    constexpr u32 node_offset(u32 index) {
        return 32 + 28 * index;
    }

    constexpr u32 element_offset(u32 index) {
        return node_offset(index) + 20;
    }

    // lays out a saved tree of records with these ids, in this order.
    // the tree's body comes right after the header, then each node and its record, and the shared stream holds the count and one marker per node.
    template<size_t size>
    void put_tree(      image_t<size>    &image,
                        u32               shared_offset,
                  const std::vector<u32> &ids,
                        trace_t          &trace) {

        image.header(shared_offset);

        // loading never sets destruct_contents, so it comes straight from the saved bytes.
        image.put(tree_offset + 12, true);
        image.put(shared_offset, (u32)ids.size());

        for (u32 index = 0; index < ids.size(); ++index) {
            // only the element pointer gets junk here; the links stay zeroed.
            // adding a node fills in its links but never clears them, so saved nodes have to come with empty ones.
            image.put(node_offset(index), u32{0xDEAD0000 + index});
            put_record(image, element_offset(index), ids[index], trace);
            image.put(shared_offset + 4 + index, mash::member_class_mashed);
        }
    }

    // checks one node and everything under it, then hands back its height; an empty spot counts as height -1.
    i32 check_subtree(const record_node*      node,
                      const record_node*      parent,
                            std::vector<u32> &ids) {

        if (!node)
            return -1;

        CHECK(node->parent == parent);
        REQUIRE(node->element != nullptr);

        // going left, then the node itself, then right visits everything in sorted order.
        i32 left_height = check_subtree(node->left, node, ids);
        ids.push_back(node->element->id);
        i32 right_height = check_subtree(node->right, node, ids);

        CHECK(node->height == 1 + std::max(left_height, right_height));
        CHECK(std::abs(left_height - right_height) <= 1);

        return node->height;
    }

    // checks every link, height and the balance rule, then makes sure the tree's own iterator visits the same ids in the same order.
    void check_tree(const record_tree &tree, const std::vector<u32> &expected_ids) {
        std::vector<u32> walked_ids;
        check_subtree(tree.root_node, nullptr, walked_ids);

        CHECK(walked_ids == expected_ids);
        CHECK(tree.size == expected_ids.size());

        std::vector<u32> iterated_ids;

        for (auto it = tree.begin(); it.node; it.iterate())
            iterated_ids.push_back(it.node->element->id);

        CHECK(iterated_ids == expected_ids);
    }

    // a node and record made on the heap, like a tree built at runtime would have.
    record_node* make_owned_node(u32 id, trace_t &trace) {
        auto* element = ::new (treyarch::mm::allocate(sizeof(record_t))) record_t{id, &trace};
        auto* node    = (record_node*)treyarch::mm::allocate(sizeof(record_node));

        *node = record_node{element, nullptr, nullptr, nullptr, 0, {}};

        return node;
    }

    // hands the tree three owned nodes, with 2 on top and 1 and 3 under it.
    void put_owned_nodes(record_tree &tree, trace_t &trace) {
        record_node* root = make_owned_node(2, trace);

        root->left          = make_owned_node(1, trace);
        root->right         = make_owned_node(3, trace);
        root->left->parent  = root;
        root->right->parent = root;
        root->height        = 1;

        tree.root_node = root;
        tree.size      = 3;
    }
}}

TEST_SUITE("mash") {
    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "loading a tree reads each node and its element, then links the nodes up in sorted order") {

        image_t<256> image;
        trace_t      trace;
        put_tree(image, 192, {2, 1, 3}, trace);

        auto* tree = mash::unmash_in_place<record_tree>(image.bytes.data(), 256);

        REQUIRE(tree == image.at<record_tree>(tree_offset));
        CHECK(tree->size == 3);
        CHECK(tree->destruct_contents);

        // the first saved node became the root; the other two sorted to either side of it.
        auto* root = image.at<record_node>(node_offset(0));

        REQUIRE(tree->root_node == root);
        CHECK(root->left == image.at<record_node>(node_offset(1)));
        CHECK(root->right == image.at<record_node>(node_offset(2)));

        for (u32 index = 0; index < 3; ++index) {
            CAPTURE(index);

            CHECK(image.at<record_node>(node_offset(index))->element == image.at<record_t>(element_offset(index)));
        }

        check_tree(*tree, {1, 2, 3});

        // the tree's image bytes reach up to the end of the last record.
        CHECK(tree->mash_image_offset == element_offset(2) + 8 - tree_offset);

        // elements are unmashed in saved order with their node as the parent; construction then goes in sorted order.
        REQUIRE(trace.events.size() == 6);

        for (u32 index = 0; index < 3; ++index) {
            CAPTURE(index);

            CHECK(trace.events[index].kind == event_unmash);
            CHECK(trace.events[index].object == image.at<record_t>(element_offset(index)));
            CHECK(trace.events[index].parent == image.at<record_node>(node_offset(index)));
            CHECK(trace.events[3 + index].kind == event_construct);
            CHECK(trace.events[3 + index].id == index + 1);
        }

        // teardown clears out each node's children before the node itself, so 1 and 3 go before 2.
        // everything lives in the image, so nothing is freed.
        tree->destruct_mashed_class();

        REQUIRE(trace.events.size() == 9);
        CHECK(trace.events[6].kind == event_destruct_mashed);
        CHECK(trace.events[6].id == 1);
        CHECK(trace.events[7].id == 3);
        CHECK(trace.events[8].id == 2);
        CHECK(tree->root_node == nullptr);
        CHECK(tree->size == 0);
        CHECK(tree->mash_image_offset == 0);
        CHECK(root->element == nullptr);
        CHECK(allocations.empty());
        CHECK(frees.empty());
    }

    TEST_CASE("each lopsided loading order gets rotated back into the same balanced shape") {
        // 3 2 1 and 1 2 3 lean all the way to one side, and a single rotation fixes them.
        // 3 1 2 and 1 3 2 zig-zag, and take a double rotation, one on the child and then one on the top.
        for (const auto &ids : { std::vector<u32> { 3, 2, 1 },
                                 std::vector<u32> { 1, 2, 3 },
                                 std::vector<u32> { 3, 1, 2 },
                                 std::vector<u32> { 1, 3, 2 } }) {

            CAPTURE(ids[0]);
            CAPTURE(ids[1]);

            image_t<256> image;
            trace_t      trace;
            put_tree(image, 192, ids, trace);

            auto* tree = mash::unmash_in_place<record_tree>(image.bytes.data(), 256);

            check_tree(*tree, {1, 2, 3});
            REQUIRE(tree->root_node != nullptr);
            CHECK(tree->root_node->element->id == 2);
            CHECK(tree->root_node->height == 1);

            tree->destruct_mashed_class();
        }
    }

    TEST_CASE("the tree stays balanced for sorted, reversed and shuffled loads") {
        constexpr u32 count = 64;

        std::vector<u32> sorted;

        for (u32 index = 0; index < count; ++index)
            sorted.push_back(index + 1);

        // stepping by 37 visits every slot exactly once, since 37 and 64 share no factors; it's a cheap shuffle.
        std::vector<u32> reversed(sorted.rbegin(), sorted.rend());
        std::vector<u32> shuffled;

        for (u32 index = 0; index < count; ++index)
            shuffled.push_back(index * 37 % count + 1);

        for (const auto* ids : {&sorted, &reversed, &shuffled}) {
            CAPTURE((*ids)[0]);

            image_t<2048> image;
            trace_t       trace;
            put_tree(image, 1856, *ids, trace);

            auto* tree = mash::unmash_in_place<record_tree>(image.bytes.data(), 2048);

            check_tree(*tree, sorted);

            // a balanced tree of 64 nodes can't be more than 8 levels tall; an unbalanced one could be 64.
            CHECK(tree->root_node->height <= 7);

            tree->destruct_mashed_class();
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "an element equal to one already in the tree gets loaded but left out") {

        image_t<256> image;
        trace_t      trace;
        put_tree(image, 192, {2, 1, 2}, trace);

        auto* tree = mash::unmash_in_place<record_tree>(image.bytes.data(), 256);

        check_tree(*tree, {1, 2});
        CHECK(tree->root_node == image.at<record_node>(node_offset(0)));

        // the second 2 was still unmashed, and its bytes still count towards the tree's image bytes.
        // nothing points at its node, though, so construction and teardown never reach it.
        CHECK(tree->mash_image_offset == element_offset(2) + 8 - tree_offset);
        REQUIRE(trace.events.size() == 5);
        CHECK(trace.events[2].kind == event_unmash);
        CHECK(trace.events[2].object == image.at<record_t>(element_offset(2)));
        CHECK(trace.events[4].kind == event_construct);
        CHECK(trace.events[4].object == image.at<record_t>(element_offset(0)));

        tree->destruct_mashed_class();

        REQUIRE(trace.events.size() == 7);
        CHECK(trace.events[5].object == image.at<record_t>(element_offset(1)));
        CHECK(trace.events[6].object == image.at<record_t>(element_offset(0)));
        CHECK(frees.empty());
    }

    TEST_CASE("a node only loads its element when its marker byte says the element is there") {
        // 0xaf means present; any other value means the element was null when it was saved.
        for (u8 marker : {u8{0}, u8{1}, mash::member_class_mashed, u8{0xFF}}) {
            CAPTURE(marker);

            const bool present = marker == mash::member_class_mashed;

            // no header this time; the node is at 0, its element would be at 20 and the shared stream starts at 64.
            image_t<128> image;
            image.put(64, marker);

            trace_t trace;
            auto*   node = image.make<record_node>(0);

            node->element = (record_t*)0xDEAD0000;
            put_record(image, 20, 5, trace);

            mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 64, false);
            reader.set_buffer(mash::SHARED_BUFFER, image.bytes.data() + 64, 64);

            // the node's own twenty bytes were read by whoever loaded the node.
            reader.advance_buffer(mash::NORMAL_BUFFER, 20);
            node->unmash(&reader, nullptr, mash::NORMAL_BUFFER);

            CHECK(node->element == (present ? image.at<record_t>(20) : nullptr));
            CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == (present ? 28 : 20));
            CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 1);
            REQUIRE(trace.events.size() == (present ? 1u : 0u));

            if (present)
                CHECK(trace.events[0].parent == node);

            // a node's own teardown only reaches its element when there is one, and then lets go of it.
            node->destruct_mashed_class();

            CHECK(node->element == nullptr);
            CHECK(trace.events.size() == (present ? 2u : 0u));
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "an owned tree destroys and frees its elements and nodes, children before parents") {

        trace_t trace;

        {
            // initialize(ALLOCATED) is the reset for a tree that isn't from an image.
            record_tree tree {};
            tree.initialize(mash::ALLOCATED);

            CHECK(tree.root_node == nullptr);
            CHECK(tree.size == 0);
            CHECK(tree.destruct_contents);
            CHECK(tree.begin().node == nullptr);

            put_owned_nodes(tree, trace);
            check_tree(tree, {1, 2, 3});
        }

        // the tree's destructor clears it; no image is involved, so all three records get their real destructors.
        REQUIRE(trace.events.size() == 3);
        CHECK(trace.events[0].kind == event_destructor);
        CHECK(trace.events[0].id == 1);
        CHECK(trace.events[1].id == 3);
        CHECK(trace.events[2].id == 2);
        CHECK(trace.destructor_saw_live_storage);
        CHECK(free_count(tests::heap_allocator) == 6);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "a tree that doesn't own its elements frees only its nodes") {

        trace_t trace;

        {
            record_tree tree {};
            tree.initialize(mash::ALLOCATED);
            tree.destruct_contents = false;

            put_owned_nodes(tree, trace);
        }

        // the three records are still alive; the memory fixture cleans them up after the test.
        CHECK(trace.events.empty());
        CHECK(free_count(tests::heap_allocator) == 3);
        CHECK(live_count(tests::heap_allocator) == 3);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "find walks down from the root comparing elements and hands back the one that matches") {

        trace_t     trace;
        record_tree tree {};
        tree.initialize(mash::ALLOCATED);

        put_owned_nodes(tree, trace);

        // the key only has to compare equal; it isn't one of the tree's own records.
        for (u32 id : {1u, 2u, 3u}) {
            CAPTURE(id);

            record_t  key{id, &trace};
            record_t* found = tree.find(&key);

            REQUIRE(found != nullptr);
            CHECK(found->id == id);
            CHECK(found != &key);
        }

        record_t missing{4, &trace};

        CHECK(tree.find(&missing) == nullptr);
        CHECK(tree.find(nullptr) == nullptr);
    }
}
