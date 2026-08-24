#include <cstdlib>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/shader_program_cache.hh"
#include "treyarch/shared/hash/algo.hh"
#include "treyarch/shared/memory/game_heap.hh"

using namespace treyarch;
using namespace treyarch::ngl;

static u32 shader_bytecode_size(const DWORD* bytecode) {
    const DWORD* instruction = bytecode + 1;

    while ((*instruction & D3DSI_OPCODE_MASK) != D3DSIO_END) {
        DWORD token = *instruction;

        if ((token & D3DSI_OPCODE_MASK) == D3DSIO_COMMENT) {
            u32 comment_size = (token & D3DSI_COMMENTSIZE_MASK) >> D3DSI_COMMENTSIZE_SHIFT;

            instruction += comment_size + 1;
        } else {
            u32 operand_count = (token & D3DSI_INSTLENGTH_MASK) >> D3DSI_INSTLENGTH_SHIFT;

            instruction += operand_count + 1;
        }
    }

    return (u32)(instruction - bytecode + 1) * sizeof(DWORD);
}

static u32 mix_shader_crc(u32 crc) {
    div_t division = std::div((i32)(crc ^ 0xDEADBEEFu), 127773);

    i32 mixed = 16807 * division.rem - 2836 * division.quot;

    if (mixed < 0)
        mixed += I32_MAX;

    return (u32)mixed;
}

template<typename T>
static u32 resolve_shader_bucket(const d3d9::shader_program_cache::program_cache<T> &cache,
                                       u32                                           crc) {

    u32 bucket = mix_shader_crc(crc) & cache.mask;

    if (cache.bucket_count <= bucket)
        bucket -= (cache.mask >> 1) + 1;

    return bucket;
}

template<typename T>
static d3d9::shader_program_cache::program_node<T>* find_shader_program(d3d9::shader_program_cache::program_cache<T> &cache,
                                                                        u32                                           crc) {

    u32 bucket = resolve_shader_bucket(cache, crc);
    auto* node = cache.buckets.begin[bucket];
    auto* end  = cache.buckets.begin[bucket + 1];

    while (node != end && node->crc < crc)
        node = node->next;

    if (node != end && node->crc == crc)
        return node;

    return cache.programs.sentinel;
}

template<typename T>
static void refresh_shader_buckets(
    ngl::d3d9::shader_program_cache::program_cache<T> &cache) {

    auto* sentinel = cache.programs.sentinel;

    for (auto** bucket = cache.buckets.begin; bucket != cache.buckets.end; ++bucket)
        *bucket = sentinel;

    auto* node = sentinel->next;

    for (u32 bucket = 0; bucket < cache.bucket_count; ++bucket) {
        cache.buckets.begin[bucket] = node;

        while (node != sentinel && resolve_shader_bucket(cache, node->crc) == bucket)
            node = node->next;
    }

    cache.buckets.begin[cache.bucket_count] = sentinel;
}

template<typename T>
static void append_shader_program(d3d9::shader_program_cache::program_node<T>* sentinel,
                                  d3d9::shader_program_cache::program_node<T>* node) {

    node->previous = sentinel->previous;
    node->next     = sentinel;

    sentinel->previous->next = node;
    sentinel->previous       = node;
}

template<typename T>
static void split_shader_bucket(ngl::d3d9::shader_program_cache::program_cache<T> &cache) {
    u32 split_bucket  = cache.bucket_count - (cache.mask >> 1) - 1;
    auto* split_begin = cache.buckets.begin[split_bucket];
    auto* split_end   = cache.buckets.begin[split_bucket + 1];

    u32 vector_size = (u32)(cache.buckets.end - cache.buckets.begin);

    if (vector_size - 1 <= cache.bucket_count) {
        u32 new_mask        = vector_size * 2 - 3;
        u32 new_vector_size = new_mask + 2;

        auto** buckets = (ngl::d3d9::shader_program_cache::program_node<T>**)
            memory::game_heap::allocate(new_vector_size * sizeof(ngl::d3d9::shader_program_cache::program_node<T>*));

        memory::game_heap::free(cache.buckets.begin);

        cache.buckets.begin    = buckets;
        cache.buckets.end      = buckets + new_vector_size;
        cache.buckets.capacity = buckets + new_vector_size;
        cache.mask             = new_mask;
    } else if (cache.mask < cache.bucket_count)
        cache.mask = cache.mask * 2 + 1;

    auto* sentinel = cache.programs.sentinel;
    auto* node     = split_begin;
    u32 split_size = 0;

    for (auto* scan = split_begin; scan != split_end; scan = scan->next)
        ++split_size;

    for (u32 index = 0; index < split_size; ++index) {
        auto* next = node->next;

        if ((mix_shader_crc(node->crc) & cache.mask) != split_bucket) {
            node->previous->next = node->next;
            node->next->previous = node->previous;

            append_shader_program(sentinel, node);
        }

        node = next;
    }

    ++cache.bucket_count;

    refresh_shader_buckets(cache);
}

template<typename T>
static d3d9::shader_program_cache::program_node<T>* insert_shader_program(d3d9::shader_program_cache::program_cache<T> &cache,
                                                                          u32                                           crc,
                                                                          T*                                            program) {

    if (cache.bucket_count <= cache.programs.size >> 2)
        split_shader_bucket(cache);

    u32 bucket = resolve_shader_bucket(cache, crc);
    auto* node = cache.buckets.begin[bucket];
    auto* end  = cache.buckets.begin[bucket + 1];

    while (node != end && node->crc < crc)
        node = node->next;

    if (node != end && node->crc == crc)
        return node;

    auto* inserted = (ngl::d3d9::shader_program_cache::program_node<T>*)
        memory::game_heap::allocate(sizeof(ngl::d3d9::shader_program_cache::program_node<T>));

    inserted->next     = node;
    inserted->previous = node->previous;
    inserted->crc      = crc;
    inserted->program  = program;

    node->previous->next = inserted;
    node->previous       = inserted;

    ++cache.programs.size;

    refresh_shader_buckets(cache);

    return inserted;
}

HRESULT d3d9::shader_program_cache::create_vertex_program(const DWORD*                   bytecode,
                                                                IDirect3DVertexShader9** output) {

    u32 byte_count = shader_bytecode_size(bytecode);
    u32 crc        = hash::crc2(bytecode, byte_count, U32_MAX);

    auto &cache = references::vertex_programs.get();

    auto* node = find_shader_program(cache, crc);

    if (node != cache.programs.sentinel) {
        *output = node->program;

        return D3D_OK;
    }

    HRESULT result = d3d9::references::device.read()->CreateVertexShader(bytecode, output);

    insert_shader_program(cache, crc, *output);

    return result;
}

HRESULT d3d9::shader_program_cache::create_pixel_program(const DWORD*                  bytecode,
                                                               IDirect3DPixelShader9** output) {

    u32 byte_count = shader_bytecode_size(bytecode);
    u32 crc        = hash::crc2(bytecode, byte_count, U32_MAX);

    auto &cache = references::pixel_programs.get();

    auto* node = find_shader_program(cache, crc);

    if (node != cache.programs.sentinel) {
        *output = node->program;

        return D3D_OK;
    }

    HRESULT result = d3d9::references::device.read()->CreatePixelShader(bytecode, output);

    insert_shader_program(cache, crc, *output);

    return result;
}
