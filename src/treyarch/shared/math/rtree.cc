#include "treyarch/shared/math/math.hh"
#include "treyarch/shared/math/rtree.hh"
#include "treyarch/shared/math/types/vector4.hh"
#include "treyarch/shared/mutex.hh"

using namespace treyarch;

struct packed_node {
    i16 bounds[6];
    u32 relative_offset;
};

struct rtree_root {
    vector4      region_center;
    vector4      world_to_quantized;
    u8           reserved_020[0x10];
    packed_node* root_nodes;
    u8*          node_pool;
    i32          visitor_index_base;
    i32          leaf_depth;
};

struct rtree_handle {
    rtree_root*            root;
    engine_recursive_lock* lock;
};

struct traversal_entry {
    packed_node* nodes;
    i32          depth;
};

ASSERT_SIZEOF  (packed_node,                  0x10);
ASSERT_OFFSETOF(packed_node, relative_offset, 0x0C);

ASSERT_SIZEOF  (rtree_root,                     0x40);
ASSERT_OFFSETOF(rtree_root, root_nodes,         0x30);
ASSERT_OFFSETOF(rtree_root, node_pool,          0x34);
ASSERT_OFFSETOF(rtree_root, visitor_index_base, 0x38);
ASSERT_OFFSETOF(rtree_root, leaf_depth,         0x3C);

ASSERT_SIZEOF  (rtree_handle,       0x08);
ASSERT_OFFSETOF(rtree_handle, root, 0x00);
ASSERT_OFFSETOF(rtree_handle, lock, 0x04);

f32 quantize_coordinate(f32 value, f32 origin, f32 scale) {
    f32 translated = (f32)((f64)value - (f64)origin);
    
    return (f32)((f64)translated * (f64)scale);
}

void pack_query_bounds(const rtree_root*  root,
                       const vector3     &minimum,
                       const vector3     &maximum,
                             i16*         result) {

    for (u32 axis = 0; axis < 3; ++axis) {
        f32 first = quantize_coordinate(minimum[axis],
                                        root->region_center[axis],
                                        root->world_to_quantized[axis]);
        f32 second = quantize_coordinate(maximum[axis],
                                         root->region_center[axis],
                                         root->world_to_quantized[axis]);

        math::clamp_value_abs(&first,  32766.0f);
        math::clamp_value_abs(&second, 32766.0f);

        f32 query_minimum = second <= first ? second : first;
        f32 query_maximum = first <= second ? second : first;

        result[2 * axis + 0] = (i16)(i32)(0.0f - query_maximum);
        result[2 * axis + 1] = (i16)(i32)query_minimum;
    }
}

bool intersects(const packed_node* node, const i16* query_bounds) {
    for (u32 index = 0; index < 6; ++index) {
        if (query_bounds[index] > node->bounds[index])
            return false;
    }

    return true;
}

void traverse(const vector3        &minimum,
              const vector3        &maximum,
              const rtree_root*     root,
                    math::visitor*  result_visitor) {

    constexpr u32 traversal_capacity = 800;
    constexpr u32 result_capacity    = 5500;

    i16 query_bounds[6];
    pack_query_bounds(root, minimum, maximum, query_bounds);

    traversal_entry traversal_queue[traversal_capacity];
    packed_node*    results[result_capacity];
    u32             read_index   = 0;
    u32             write_index  = 1;
    u32             result_count = 0;

    traversal_queue[0].nodes = root->root_nodes;
    traversal_queue[0].depth = 0;

    while (read_index != write_index) {
        traversal_entry entry = traversal_queue[read_index];
        read_index = (read_index + 1) % traversal_capacity;

        for (u32 child_index = 0; child_index < 8; ++child_index) {
            packed_node* child = &entry.nodes[child_index];

            if (!intersects(child, query_bounds))
                continue;

            if (entry.depth >= root->leaf_depth) {
                results[result_count++] = child;
                continue;
            }

            traversal_queue[write_index].nodes =
                (packed_node*)(root->node_pool + child->relative_offset);
            traversal_queue[write_index].depth = entry.depth + 1;
            write_index = (write_index + 1) % traversal_capacity;
        }
    }

    for (u32 index = 0; index < result_count; ++index) {
        i32 visitor_index = (i32)((u32)root->visitor_index_base +
                                  results[index]->relative_offset);

        result_visitor->vtable->visit(result_visitor, visitor_index);
    }
}

void math::query_sphere(      rtree*    value,
                        const vector3  &center,
                              f32       radius,
                              visitor*  result_visitor) {

    auto* tree = (rtree_handle*)value;

    engine_lock_scope lock(tree->lock);

    vector3 minimum(center.x - radius,
                    center.y - radius,
                    center.z - radius);
    vector3 maximum(center.x + radius,
                    center.y + radius,
                    center.z + radius);

    traverse(minimum, maximum, tree->root, result_visitor);
}
