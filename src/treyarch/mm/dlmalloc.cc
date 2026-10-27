#include <cstdlib>
#include <cstring>

#include "treyarch/mm/dlmalloc.hh"

using namespace treyarch;
using namespace treyarch::mm;

/*
    dlmalloc's configuration and macros
*/

static constexpr u32 malloc_alignment      = 8;
static constexpr u32 chunk_align_mask      = malloc_alignment - 1;
static constexpr u32 chunk_overhead        = 8; // with footers
static constexpr u32 mmap_foot_pad         = 16;
static constexpr u32 min_chunk_size        = 16;
static constexpr u32 max_request           = 0xFFFFFFC0;
static constexpr u32 min_request           = min_chunk_size - chunk_overhead - 1;
static constexpr u32 max_small_request     = 0xF0;
static constexpr u32 top_foot_size         = 40;
static constexpr u32 state_size            = 0x1E0; // pad_request(sizeof(malloc_state))
static constexpr u32 segment_size          = 24;    // pad_request(sizeof(malloc_segment))
static constexpr u32 nsmallbins            = 32;
static constexpr u32 ntreebins             = 32;
static constexpr u32 smallbin_shift        = 3;
static constexpr u32 treebin_shift         = 8;
static constexpr u32 sys_alloc_pad         = 0xC000;

static constexpr u32 pinuse_bit            = 1;
static constexpr u32 cinuse_bit            = 2;
static constexpr u32 inuse_bits            = pinuse_bit | cinuse_bit;
static constexpr u32 fencepost_head        = inuse_bits | 4;
static constexpr u32 is_mmapped_bit        = 1;
static constexpr u32 use_mmap_bit          = 1;
static constexpr u32 use_noncontiguous_bit = 4;
static constexpr u32 extern_bit            = 8;

static u32           align_offset(const void* address)                 { return ((u32)address & chunk_align_mask) ? (malloc_alignment - ((u32)address & chunk_align_mask)) & chunk_align_mask : 0; }
static u32           pad_request(u32 request)                          { return (request + chunk_overhead + chunk_align_mask) & ~chunk_align_mask; }
static u32           request2size(u32 request)                         { return request < min_request ? min_chunk_size : pad_request(request); }

static void*         chunk2mem(const void* chunk)                      { return (u8*)chunk + 8; }
static malloc_chunk* mem2chunk(const void* mem)                        { return (malloc_chunk*)((u8*)mem - 8); }
static malloc_chunk* align_as_chunk(u8* address)                       { return (malloc_chunk*)(address + align_offset(address + 8)); }
static malloc_chunk* chunk_plus_offset(const void* chunk, u32 offset)  { return (malloc_chunk*)((u8*)chunk + offset); }
static malloc_chunk* chunk_minus_offset(const void* chunk, u32 offset) { return (malloc_chunk*)((u8*)chunk - offset); }

static u32           chunksize(const malloc_chunk* p)                  { return p->head & ~inuse_bits; }
static bool          cinuse(const malloc_chunk* p)                     { return p->head & cinuse_bit; }
static bool          pinuse(const malloc_chunk* p)                     { return p->head & pinuse_bit; }
static malloc_chunk* next_chunk(const malloc_chunk* p)                 { return chunk_plus_offset(p, chunksize(p)); }
static bool          is_mmapped(const malloc_chunk* p)                 { return !pinuse(p) && (p->prev_foot & is_mmapped_bit); }

static u32           small_index(u32 size)                             { return size >> smallbin_shift; }
static u32           small_index2size(u32 index)                       { return index << smallbin_shift; }
static malloc_chunk* smallbin_at(malloc_state* m, u32 index)           { return (malloc_chunk*)&m->smallbins[index << 1]; }
static u32           idx2bit(u32 index)                                { return 1u << index; }
static u32           least_bit(u32 bits)                               { return bits & (0 - bits); }
static u32           left_bits(u32 bits)                               { return (bits << 1) | (0 - (bits << 1)); }
static u32           leftshift_for_tree_index(u32 index)               { return index == ntreebins - 1 ? 0 : 31 - ((index >> 1) + treebin_shift - 2); }

static malloc_tree_chunk* leftmost_child(const malloc_tree_chunk* t)   { return t->child[0] ? t->child[0] : t->child[1]; }

static bool          ok_magic(const malloc_state* m)                   { return m->magic == references::mparams.get().magic; }
static bool          ok_address(const malloc_state* m, const void* a)  { return (u8*)a >= m->least_addr; }
static bool          ok_next(const void* p, const void* n)             { return (u8*)p < (u8*)n; }
static malloc_state* get_mstate_for(const malloc_chunk* p)             { return (malloc_state*)(chunk_plus_offset(p, chunksize(p))->prev_foot ^ references::mparams.get().magic); }
static bool          is_global(const malloc_state* m)                  { return m == &references::gm.get(); }

static u32           granularity_align(u32 size)                       { return (size + references::mparams.get().granularity) & ~(references::mparams.get().granularity - 1); }

static bool          segment_holds(const malloc_segment* s, const void* a)   { return (u8*)a >= s->base && (u8*)a < s->base + s->size; }
static bool          is_mmapped_segment(const malloc_segment* s)             { return s->sflags & is_mmapped_bit; }
static bool          is_extern_segment(const malloc_segment* s)              { return s->sflags & extern_bit; }

static void mark_inuse_foot(malloc_state* m, malloc_chunk* p, u32 size) {
    chunk_plus_offset(p, size)->prev_foot = (u32)m ^ references::mparams.get().magic;
}

static void set_inuse(malloc_state* m, malloc_chunk* p, u32 size) {
    p->head = (p->head & pinuse_bit) | size | cinuse_bit;
    chunk_plus_offset(p, size)->head |= pinuse_bit;
    mark_inuse_foot(m, p, size);
}

static void set_inuse_and_pinuse(malloc_state* m, malloc_chunk* p, u32 size) {
    p->head = size | pinuse_bit | cinuse_bit;
    chunk_plus_offset(p, size)->head |= pinuse_bit;
    mark_inuse_foot(m, p, size);
}

static void set_size_and_pinuse_of_inuse_chunk(malloc_state* m, malloc_chunk* p, u32 size) {
    p->head = size | pinuse_bit | cinuse_bit;
    mark_inuse_foot(m, p, size);
}

static void set_size_and_pinuse_of_free_chunk(malloc_chunk* p, u32 size) {
    p->head = size | pinuse_bit;
    chunk_plus_offset(p, size)->prev_foot = size;
}

static void set_free_with_pinuse(malloc_chunk* p, u32 size, malloc_chunk* next) {
    next->head &= ~pinuse_bit;
    set_size_and_pinuse_of_free_chunk(p, size);
}

static void corruption_error_action() {
    std::abort();
}

static void usage_error_action() {
    std::abort();
}

// the portable index computations; mm builds without the bit-scan intrinsics
static u32 compute_tree_index(u32 size) {
    u32 x = size >> treebin_shift;

    if (!x)
        return 0;

    if (x > 0xFFFF)
        return ntreebins - 1;

    u32 y = x;
    u32 n = ((y - 0x100) >> 16) & 8;
    u32 k = (((y <<= n) - 0x1000) >> 16) & 4;

    n += k;
    n += k = (((y <<= k) - 0x4000) >> 16) & 2;
    k  = 14 - n + ((y <<= k) >> 15);

    return (k << 1) + ((size >> (k + (treebin_shift - 1))) & 1);
}

static u32 compute_bit2idx(u32 bit) {
    u32 y = bit - 1;
    u32 k = y >> (16 - 4) & 16;
    u32 n = k;

    y >>= k;
    n += k = y >> (8 - 3) & 8;
    y >>= k;
    n += k = y >> (4 - 2) & 4;
    y >>= k;
    n += k = y >> (2 - 1) & 2;
    y >>= k;
    n += k = y >> (1 - 0) & 1;
    y >>= k;

    return n + y;
}

/*
    bins
*/

static void insert_small_chunk(malloc_state* m, malloc_chunk* p, u32 size) {
    u32           index = small_index(size);
    malloc_chunk* bin   = smallbin_at(m, index);
    malloc_chunk* first = bin;

    if (!(m->smallmap & idx2bit(index)))
        m->smallmap |= idx2bit(index);
    else if (ok_address(m, bin->fd))
        first = bin->fd;
    else
        corruption_error_action();

    bin->fd   = p;
    first->bk = p;
    p->fd     = first;
    p->bk     = bin;
}

static void unlink_small_chunk(malloc_state* m, malloc_chunk* p, u32 size) {
    malloc_chunk* forward = p->fd;
    malloc_chunk* back    = p->bk;
    u32           index   = small_index(size);

    if (forward == back)
        m->smallmap &= ~idx2bit(index);
    else if ((forward == smallbin_at(m, index) || ok_address(m, forward)) &&
             (back    == smallbin_at(m, index) || ok_address(m, back))) {

        forward->bk = back;
        back->fd    = forward;
    } else
        corruption_error_action();
}

static void unlink_first_small_chunk(malloc_state* m, malloc_chunk* bin, malloc_chunk* p, u32 index) {
    malloc_chunk* forward = p->fd;

    if (bin == forward)
        m->smallmap &= ~idx2bit(index);
    else if (ok_address(m, forward)) {
        bin->fd     = forward;
        forward->bk = bin;
    } else
        corruption_error_action();
}

static void replace_dv(malloc_state* m, malloc_chunk* p, u32 size) {
    if (m->dvsize)
        insert_small_chunk(m, m->dv, m->dvsize);

    m->dvsize = size;
    m->dv     = p;
}

static void insert_large_chunk(malloc_state* m, malloc_tree_chunk* x, u32 size) {
    u32                 index = compute_tree_index(size);
    malloc_tree_chunk** head  = &m->treebins[index];

    x->index    = index;
    x->child[0] = x->child[1] = nullptr;

    if (!(m->treemap & idx2bit(index))) {
        m->treemap |= idx2bit(index);

        *head     = x;
        x->parent = (malloc_tree_chunk*)head;
        x->fd     = x->bk = x;

        return;
    }

    malloc_tree_chunk* t    = *head;
    u32                bits = size << leftshift_for_tree_index(index);

    for (;;) {
        if (chunksize((malloc_chunk*)t) != size) {
            malloc_tree_chunk** child = &t->child[(bits >> 31) & 1];

            bits <<= 1;

            if (*child)
                t = *child;
            else if (ok_address(m, child)) {
                *child    = x;
                x->parent = t;
                x->fd     = x->bk = x;

                break;
            } else {
                corruption_error_action();

                break;
            }
        } else {
            malloc_tree_chunk* forward = t->fd;

            if (ok_address(m, t) && ok_address(m, forward)) {
                t->fd     = forward->bk = x;
                x->fd     = forward;
                x->bk     = t;
                x->parent = nullptr;
            } else
                corruption_error_action();

            break;
        }
    }
}

static void unlink_large_chunk(malloc_state* m, malloc_tree_chunk* x) {
    malloc_tree_chunk* parent = x->parent;
    malloc_tree_chunk* r;

    if (x->bk != x) {
        malloc_tree_chunk* forward = x->fd;

        r = x->bk;

        if (ok_address(m, forward)) {
            forward->bk = r;
            r->fd       = forward;
        } else
            corruption_error_action();
    } else {
        malloc_tree_chunk** rp;

        if ((r = *(rp = &x->child[1])) || (r = *(rp = &x->child[0]))) {
            malloc_tree_chunk** cp;

            while (*(cp = &r->child[1]) || *(cp = &r->child[0]))
                r = *(rp = cp);

            if (ok_address(m, rp))
                *rp = nullptr;
            else
                corruption_error_action();
        }
    }

    if (!parent)
        return;

    malloc_tree_chunk** head = &m->treebins[x->index];

    if (x == *head) {
        if (!(*head = r))
            m->treemap &= ~idx2bit(x->index);
    } else if (ok_address(m, parent)) {
        if (parent->child[0] == x)
            parent->child[0] = r;
        else
            parent->child[1] = r;
    } else
        corruption_error_action();

    if (!r)
        return;

    if (!ok_address(m, r)) {
        corruption_error_action();

        return;
    }

    r->parent = parent;

    if (malloc_tree_chunk* c0 = x->child[0]) {
        if (ok_address(m, c0)) {
            r->child[0] = c0;
            c0->parent  = r;
        } else
            corruption_error_action();
    }

    if (malloc_tree_chunk* c1 = x->child[1]) {
        if (ok_address(m, c1)) {
            r->child[1] = c1;
            c1->parent  = r;
        } else
            corruption_error_action();
    }
}

static void insert_chunk(malloc_state* m, malloc_chunk* p, u32 size) {
    if (small_index(size) < nsmallbins)
        insert_small_chunk(m, p, size);
    else
        insert_large_chunk(m, (malloc_tree_chunk*)p, size);
}

static void unlink_chunk(malloc_state* m, malloc_chunk* p, u32 size) {
    if (small_index(size) < nsmallbins)
        unlink_small_chunk(m, p, size);
    else
        unlink_large_chunk(m, (malloc_tree_chunk*)p);
}

static u8* call_mmap(malloc_state* m, u32 size) {
    if (!m->callbacks.map)
        return (u8*)-1;

    return (u8*)m->callbacks.map(size, m->callbacks.user_data);
}

static i32 call_munmap(malloc_state* m, void* address, u32 size) {
    if (!m->callbacks.unmap)
        return -1;

    return m->callbacks.unmap(address, size, m->callbacks.user_data);
}

/*
    dlmalloc
*/

// sub_5F8B80
static malloc_segment* segment_holding(malloc_state* m, const void* address) {
    malloc_segment* sp = &m->seg;

    for (;;) {
        if (segment_holds(sp, address))
            return sp;

        if (!(sp = sp->next))
            return nullptr;
    }
}

// sub_5F8BA0
static i32 init_mparams() {
    malloc_params &mparams = references::mparams.get();

    if (mparams.page_size)
        return 0;

    mparams.mmap_threshold = 0x40000;
    mparams.trim_threshold = 0x200000;
    mparams.default_mflags = use_mmap_bit;

    // INSECURE: a fixed magic
    if (!mparams.magic) {
        mparams.magic               = 0x58585858;
        references::gm.get().mflags = mparams.default_mflags;
    }

    // fixed, where a WIN32 build asks GetSystemInfo
    mparams.page_size   = 0x10000;
    mparams.granularity = 0x10000;

    return 0;
}

// sub_5F8C00
static void init_top(malloc_state* m, malloc_chunk* p, u32 psize) {
    u32 offset = align_offset(chunk2mem(p));

    p      = chunk_plus_offset(p, offset);
    psize -= offset;

    m->top     = p;
    m->topsize = psize;

    p->head                          = psize | pinuse_bit;
    chunk_plus_offset(p, psize)->head = top_foot_size;

    m->trim_check = references::mparams.get().trim_threshold;
}

// sub_5F8C30
static void init_bins(malloc_state* m) {
    for (u32 index = 0; index < nsmallbins; ++index) {
        malloc_chunk* bin = smallbin_at(m, index);

        bin->fd = bin->bk = bin;
    }
}

// sub_5F8C50
static void* prepend_alloc(malloc_state* m, u8* newbase, u8* oldbase, u32 nb) {
    malloc_chunk* p        = align_as_chunk(newbase);
    malloc_chunk* oldfirst = align_as_chunk(oldbase);
    u32           psize    = (u8*)oldfirst - (u8*)p;
    malloc_chunk* q        = chunk_plus_offset(p, nb);
    u32           qsize    = psize - nb;

    set_size_and_pinuse_of_inuse_chunk(m, p, nb);

    // consolidate the remainder with the first chunk of the old base
    if (oldfirst == m->top) {
        u32 tsize = m->topsize += qsize;

        m->top  = q;
        q->head = tsize | pinuse_bit;
    } else if (oldfirst == m->dv) {
        u32 dsize = m->dvsize += qsize;

        m->dv = q;
        set_size_and_pinuse_of_free_chunk(q, dsize);
    } else {
        if (!cinuse(oldfirst)) {
            u32 nsize = chunksize(oldfirst);

            unlink_chunk(m, oldfirst, nsize);

            oldfirst  = chunk_plus_offset(oldfirst, nsize);
            qsize    += nsize;
        }

        set_free_with_pinuse(q, qsize, oldfirst);
        insert_chunk(m, q, qsize);
    }

    return chunk2mem(p);
}

// sub_5F9040
static void add_segment(malloc_state* m, u8* tbase, u32 tsize, u32 mmapped) {
    // where the segment record, the fenceposts and the old top go
    u8*             old_top  = (u8*)m->top;
    malloc_segment* oldsp    = segment_holding(m, old_top);
    u8*             old_end  = oldsp->base + oldsp->size;
    u8*             rawsp    = old_end - (segment_size + 16 + chunk_align_mask);
    u8*             asp      = rawsp + align_offset(chunk2mem(rawsp));
    u8*             csp      = asp < old_top + min_chunk_size ? old_top : asp;
    malloc_chunk*   sp       = (malloc_chunk*)csp;
    malloc_segment* ss       = (malloc_segment*)chunk2mem(sp);
    malloc_chunk*   tnext    = chunk_plus_offset(sp, segment_size);
    malloc_chunk*   p        = tnext;

    init_top(m, (malloc_chunk*)tbase, tsize - top_foot_size);

    // push the current record
    set_size_and_pinuse_of_inuse_chunk(m, sp, segment_size);

    *ss = m->seg;

    m->seg.base   = tbase;
    m->seg.size   = tsize;
    m->seg.sflags = mmapped;
    m->seg.next   = ss;

    // trailing fenceposts
    for (;;) {
        malloc_chunk* nextp = chunk_plus_offset(p, 4);

        p->head = fencepost_head;

        if ((u8*)&nextp->head < old_end)
            p = nextp;
        else
            break;
    }

    // the rest of the old top becomes an ordinary free chunk
    if (csp != old_top) {
        malloc_chunk* q     = (malloc_chunk*)old_top;
        u32           psize = csp - old_top;
        malloc_chunk* tn    = chunk_plus_offset(q, psize);

        set_free_with_pinuse(q, psize, tn);
        insert_chunk(m, q, psize);
    }
}

// sub_5F92F0
static void* sys_alloc(malloc_state* m, u32 nb) {
    // mm pads every request it takes to the system
    nb += sys_alloc_pad;

    init_mparams();

    // the contiguous path; without MORECORE it only gives up on contiguity
    if (!(m->mflags & use_noncontiguous_bit)) {
        if (m->top)
            segment_holding(m, m->top);

        m->mflags |= use_noncontiguous_bit;
    }

    u32 rsize = granularity_align(nb + top_foot_size + 1);

    if (rsize > nb) {
        u8* tbase = call_mmap(m, rsize);

        if (tbase != (u8*)-1) {
            u32 tsize = rsize;

            if ((m->footprint += tsize) > m->max_footprint)
                m->max_footprint = m->footprint;

            if (!m->top) {
                // first-time initialization
                m->seg.base   = m->least_addr = tbase;
                m->seg.size   = tsize;
                m->seg.sflags = is_mmapped_bit;
                m->magic      = references::mparams.get().magic;

                init_bins(m);

                if (is_global(m))
                    init_top(m, (malloc_chunk*)tbase, tsize - top_foot_size);
                else {
                    // offset top by the embedded malloc_state
                    malloc_chunk* mn = next_chunk(mem2chunk(m));

                    init_top(m, mn, (u32)((tbase + tsize) - (u8*)mn) - top_foot_size);
                }
            } else {
                // try to merge with an existing segment
                malloc_segment* sp = &m->seg;

                while (sp && tbase != sp->base + sp->size)
                    sp = sp->next;

                if (sp && !is_extern_segment(sp) && (sp->sflags & is_mmapped_bit) == is_mmapped_bit && segment_holds(sp, m->top)) {
                    // append
                    sp->size += tsize;

                    init_top(m, m->top, m->topsize + tsize);
                } else {
                    if (tbase < m->least_addr)
                        m->least_addr = tbase;

                    sp = &m->seg;

                    while (sp && sp->base != tbase + tsize)
                        sp = sp->next;

                    if (sp && !is_extern_segment(sp) && (sp->sflags & is_mmapped_bit) == is_mmapped_bit) {
                        u8* oldbase = sp->base;

                        sp->base  = tbase;
                        sp->size += tsize;

                        return prepend_alloc(m, tbase, oldbase, nb);
                    }

                    add_segment(m, tbase, tsize, is_mmapped_bit);
                }
            }

            // allocate from the new or extended top
            if (nb < m->topsize) {
                u32           rest = m->topsize -= nb;
                malloc_chunk* p    = m->top;
                malloc_chunk* r    = m->top = chunk_plus_offset(p, nb);

                r->head = rest | pinuse_bit;
                set_size_and_pinuse_of_inuse_chunk(m, p, nb);

                return chunk2mem(p);
            }
        }
    }

    memory_space::allocation_failure(m);

    return nullptr;
}

// sub_5F94F0
static u32 release_unused_segments(malloc_state* m) {
    u32             released = 0;
    malloc_segment* pred     = &m->seg;
    malloc_segment* sp       = pred->next;

    while (sp) {
        u8*             base = sp->base;
        u32             size = sp->size;
        malloc_segment* next = sp->next;

        if (is_mmapped_segment(sp) && !is_extern_segment(sp)) {
            malloc_chunk* p     = align_as_chunk(base);
            u32           psize = chunksize(p);

            // can unmap if the first chunk holds the entire segment
            if (!cinuse(p) && (u8*)p + psize >= base + size - top_foot_size) {
                malloc_tree_chunk* tp = (malloc_tree_chunk*)p;

                if (p == m->dv) {
                    m->dv     = nullptr;
                    m->dvsize = 0;
                } else
                    unlink_large_chunk(m, tp);

                if (!call_munmap(m, base, size)) {
                    released     += size;
                    m->footprint -= size;

                    // unlink the obsolete record
                    sp       = pred;
                    sp->next = next;
                } else
                    insert_large_chunk(m, tp, psize);
            }
        }

        pred = sp;
        sp   = next;
    }

    return released;
}

// inlined @ sub_5FA580
static bool has_segment_link(malloc_state* m, malloc_segment* ss) {
    for (malloc_segment* sp = &m->seg; sp; sp = sp->next) {
        if ((u8*)sp >= ss->base && (u8*)sp < ss->base + ss->size)
            return true;
    }

    return false;
}

// inlined @ sub_5FA580
static i32 sys_trim(malloc_state* m, u32 pad) {
    u32 released = 0;

    if (pad >= max_request || !m->top)
        return 0;

    // enough room for the segment overhead
    pad += top_foot_size;

    if (m->topsize > pad) {
        // shrink top in granularity units, keeping at least one
        u32             unit  = references::mparams.get().granularity;
        u32             extra = ((m->topsize - pad + (unit - 1)) / unit - 1) * unit;
        malloc_segment* sp    = segment_holding(m, m->top);

        if (!is_extern_segment(sp) && is_mmapped_segment(sp) && sp->size >= extra && !has_segment_link(m, sp)) {
            u32 newsize = sp->size - extra;

            // no mremap; unmap the end
            if (!call_munmap(m, sp->base + newsize, extra))
                released = extra;
        }

        if (released) {
            sp->size     -= released;
            m->footprint -= released;

            init_top(m, m->top, m->topsize - released);
        }
    }

    released += release_unused_segments(m);

    // on failure, disable autotrim to avoid repeated failed future calls
    if (!released)
        m->trim_check = 0xFFFFFFFF;

    return released ? 1 : 0;
}

// sub_5F9900
static void* tmalloc_large(malloc_state* m, u32 nb) {
    malloc_tree_chunk* v     = nullptr;
    u32                rsize = 0 - nb;
    u32                index = compute_tree_index(nb);
    malloc_tree_chunk* t     = m->treebins[index];

    if (t) {
        // traverse the tree for this bin looking for a node with size == nb
        u32                sizebits = nb << leftshift_for_tree_index(index);
        malloc_tree_chunk* rst      = nullptr; // the deepest untaken right subtree

        for (;;) {
            u32 trem = chunksize((malloc_chunk*)t) - nb;

            if (trem < rsize) {
                v = t;

                if (!(rsize = trem))
                    break;
            }

            malloc_tree_chunk* rt = t->child[1];

            t = t->child[(sizebits >> 31) & 1];

            if (rt && rt != t)
                rst = rt;

            if (!t) {
                // the least subtree holding sizes > nb
                t = rst;

                break;
            }

            sizebits <<= 1;
        }
    }

    // the root of the next nonempty treebin
    if (!t && !v) {
        u32 leftbits = left_bits(idx2bit(index)) & m->treemap;

        if (leftbits)
            t = m->treebins[compute_bit2idx(least_bit(leftbits))];
    }

    // the smallest of the tree or subtree
    while (t) {
        u32 trem = chunksize((malloc_chunk*)t) - nb;

        if (trem < rsize) {
            rsize = trem;
            v     = t;
        }

        t = leftmost_child(t);
    }

    // a better fit in dv makes malloc use it instead
    if (!v || rsize >= m->dvsize - nb)
        return nullptr;

    malloc_chunk* vc = (malloc_chunk*)v;
    malloc_chunk* r  = chunk_plus_offset(vc, nb);

    if (!ok_address(m, v) || !ok_next(v, r)) {
        corruption_error_action();

        return nullptr;
    }

    unlink_large_chunk(m, v);

    if (rsize < min_chunk_size)
        set_inuse_and_pinuse(m, vc, rsize + nb);
    else {
        set_size_and_pinuse_of_inuse_chunk(m, vc, nb);
        set_size_and_pinuse_of_free_chunk(r, rsize);
        insert_chunk(m, r, rsize);
    }

    return chunk2mem(vc);
}

// sub_5F9DE0
static void* tmalloc_small(malloc_state* m, u32 nb) {
    u32                index = compute_bit2idx(least_bit(m->treemap));
    malloc_tree_chunk* t     = m->treebins[index];
    malloc_tree_chunk* v     = t;
    u32                rsize = chunksize((malloc_chunk*)t) - nb;

    while ((t = leftmost_child(t))) {
        u32 trem = chunksize((malloc_chunk*)t) - nb;

        if (trem < rsize) {
            rsize = trem;
            v     = t;
        }
    }

    malloc_chunk* vc = (malloc_chunk*)v;
    malloc_chunk* r  = chunk_plus_offset(vc, nb);

    if (!ok_address(m, v) || !ok_next(v, r)) {
        corruption_error_action();

        return nullptr;
    }

    unlink_large_chunk(m, v);

    if (rsize < min_chunk_size)
        set_inuse_and_pinuse(m, vc, rsize + nb);
    else {
        set_size_and_pinuse_of_inuse_chunk(m, vc, nb);
        set_size_and_pinuse_of_free_chunk(r, rsize);
        replace_dv(m, r, rsize);
    }

    return chunk2mem(vc);
}

// sub_5FA040
static malloc_state* init_user_mstate(u8* tbase, u32 tsize) {
    malloc_chunk* msp = align_as_chunk(tbase);
    malloc_state* m   = (malloc_state*)chunk2mem(msp);

    std::memset(m, 0, state_size);

    msp->head = state_size | pinuse_bit | cinuse_bit;

    m->seg.base      = m->least_addr    = tbase;
    m->seg.size      = m->footprint     = m->max_footprint = tsize;
    m->magic         = references::mparams.get().magic;
    m->mflags        = references::mparams.get().default_mflags | use_noncontiguous_bit;

    init_bins(m);

    malloc_chunk* mn = next_chunk(mem2chunk(m));

    init_top(m, mn, (u32)((tbase + tsize) - (u8*)mn) - top_foot_size);

    return m;
}

// sub_5FA110
malloc_state* mm::create_mspace(u32 capacity, i32, const memory_callbacks* callbacks) {
    init_mparams();

    if (capacity >= 0 - (state_size + top_foot_size + references::mparams.get().page_size))
        return nullptr;

    u32 rs    = capacity ? capacity + top_foot_size + state_size : references::mparams.get().granularity;
    u32 tsize = granularity_align(rs);

    if (!callbacks->map)
        return nullptr;

    u8* tbase = (u8*)callbacks->map(tsize, callbacks->user_data);

    if (tbase == (u8*)-1)
        return nullptr;

    malloc_state* m = init_user_mstate(tbase, tsize);

    m->seg.sflags = is_mmapped_bit;
    m->callbacks  = *callbacks;

    return m;
}

// sub_5FA1E0
malloc_state* mm::create_mspace_with_base(void* base, u32 capacity, i32, const memory_callbacks* callbacks) {
    init_mparams();

    if (capacity <= state_size + top_foot_size || capacity >= 0 - (state_size + top_foot_size + references::mparams.get().page_size))
        return nullptr;

    malloc_state* m = init_user_mstate((u8*)base, capacity);

    m->seg.sflags = extern_bit;
    m->callbacks  = *callbacks;

    return m;
}

// sub_5FA240
u32 mm::destroy_mspace(malloc_state* msp) {
    if (!ok_magic(msp))
        usage_error_action();

    u32 freed = 0;

    for (malloc_segment* sp = &msp->seg; sp;) {
        u8* base = sp->base;
        u32 size = sp->size;
        u32 flag = sp->sflags;

        sp = sp->next;

        if ((flag & is_mmapped_bit) && !(flag & extern_bit) && !call_munmap(msp, base, size))
            freed += size;
    }

    return freed;
}

// sub_5FA2B0
void* mm::mspace_malloc(malloc_state* msp, u32 bytes) {
    malloc_state* ms = msp;

    if (!ok_magic(ms))
        usage_error_action();

    u32 nb;

    if (bytes <= max_small_request) {
        nb = request2size(bytes);

        u32 index     = small_index(nb);
        u32 smallbits = ms->smallmap >> index;

        // a remainderless fit to a smallbin, or the next one up
        if (smallbits & 3) {
            index += ~smallbits & 1;

            malloc_chunk* bin = smallbin_at(ms, index);
            malloc_chunk* p   = bin->fd;

            unlink_first_small_chunk(ms, bin, p, index);
            set_inuse_and_pinuse(ms, p, small_index2size(index));

            return chunk2mem(p);
        }

        if (nb > ms->dvsize) {
            // the next nonempty smallbin
            if (smallbits) {
                u32           leftbits = (smallbits << index) & left_bits(idx2bit(index));
                u32           i        = compute_bit2idx(least_bit(leftbits));
                malloc_chunk* bin      = smallbin_at(ms, i);
                malloc_chunk* p        = bin->fd;

                unlink_first_small_chunk(ms, bin, p, i);

                u32 rsize = small_index2size(i) - nb;

                // with four-byte sizes the fit can't be remainderless
                set_size_and_pinuse_of_inuse_chunk(ms, p, nb);

                malloc_chunk* r = chunk_plus_offset(p, nb);

                set_size_and_pinuse_of_free_chunk(r, rsize);
                replace_dv(ms, r, rsize);

                return chunk2mem(p);
            }

            if (ms->treemap) {
                if (void* mem = tmalloc_small(ms, nb))
                    return mem;
            }
        }
    } else if (bytes >= max_request)
        nb = 0xFFFFFFFF; // too big to allocate; sys_alloc fails it
    else {
        nb = pad_request(bytes);

        if (ms->treemap) {
            if (void* mem = tmalloc_large(ms, nb))
                return mem;
        }
    }

    if (nb <= ms->dvsize) {
        u32           rsize = ms->dvsize - nb;
        malloc_chunk* p     = ms->dv;

        if (rsize >= min_chunk_size) {
            // split dv
            malloc_chunk* r = ms->dv = chunk_plus_offset(p, nb);

            ms->dvsize = rsize;

            set_size_and_pinuse_of_free_chunk(r, rsize);
            set_size_and_pinuse_of_inuse_chunk(ms, p, nb);
        } else {
            // exhaust dv
            u32 dvs = ms->dvsize;

            ms->dvsize = 0;
            ms->dv     = nullptr;

            set_inuse_and_pinuse(ms, p, dvs);
        }

        return chunk2mem(p);
    }

    if (nb < ms->topsize) {
        // split top
        u32           rsize = ms->topsize -= nb;
        malloc_chunk* p     = ms->top;
        malloc_chunk* r     = ms->top = chunk_plus_offset(p, nb);

        r->head = rsize | pinuse_bit;
        set_size_and_pinuse_of_inuse_chunk(ms, p, nb);

        return chunk2mem(p);
    }

    return sys_alloc(ms, nb);
}

// sub_5FA580
void mm::mspace_free(malloc_state*, void* mem) {
    if (!mem)
        return;

    // footers say which space a chunk belongs to
    malloc_chunk* p  = mem2chunk(mem);
    malloc_state* fm = get_mstate_for(p);

    if (!ok_magic(fm) || !ok_address(fm, p) || !cinuse(p)) {
        usage_error_action();

        return;
    }

    u32           psize = chunksize(p);
    malloc_chunk* next  = chunk_plus_offset(p, psize);

    if (!pinuse(p)) {
        u32 prevsize = p->prev_foot;

        if (prevsize & is_mmapped_bit) {
            prevsize &= ~is_mmapped_bit;
            psize    += prevsize + mmap_foot_pad;

            if (!call_munmap(fm, (u8*)p - prevsize, psize))
                fm->footprint -= psize;

            return;
        }

        // consolidate backward
        malloc_chunk* prev = chunk_minus_offset(p, prevsize);

        psize += prevsize;
        p      = prev;

        if (!ok_address(fm, prev)) {
            usage_error_action();

            return;
        }

        if (p != fm->dv)
            unlink_chunk(fm, p, prevsize);
        else if ((next->head & inuse_bits) == inuse_bits) {
            fm->dvsize = psize;

            set_free_with_pinuse(p, psize, next);

            return;
        }
    }

    if (!ok_next(p, next) || !pinuse(next)) {
        usage_error_action();

        return;
    }

    if (!cinuse(next)) {
        // consolidate forward
        if (next == fm->top) {
            u32 tsize = fm->topsize += psize;

            fm->top = p;
            p->head = tsize | pinuse_bit;

            if (p == fm->dv) {
                fm->dv     = nullptr;
                fm->dvsize = 0;
            }

            if (tsize > fm->trim_check)
                sys_trim(fm, 0);

            return;
        }

        if (next == fm->dv) {
            u32 dsize = fm->dvsize += psize;

            fm->dv = p;

            set_size_and_pinuse_of_free_chunk(p, dsize);

            return;
        }

        u32 nsize = chunksize(next);

        psize += nsize;

        unlink_chunk(fm, next, nsize);
        set_size_and_pinuse_of_free_chunk(p, psize);

        if (p == fm->dv) {
            fm->dvsize = psize;

            return;
        }
    } else
        set_free_with_pinuse(p, psize, next);

    insert_chunk(fm, p, psize);
}

// sub_5FAC60
static void* internal_memalign(malloc_state* m, u32 alignment, u32 bytes) {
    if (alignment <= malloc_alignment)
        return mspace_malloc(m, bytes);

    if (alignment < min_chunk_size)
        alignment = min_chunk_size;

    // a power of two
    if (alignment & (alignment - 1)) {
        u32 power = malloc_alignment << 1;

        while (power < alignment)
            power <<= 1;

        alignment = power;
    }

    if (bytes >= max_request - alignment) {
        if (m)
            memory_space::allocation_failure(m);

        return nullptr;
    }

    u32   nb  = request2size(bytes);
    u32   req = nb + alignment + min_chunk_size - chunk_overhead;
    u8*   mem = (u8*)mspace_malloc(m, req);

    if (!mem)
        return nullptr;

    void*         leader  = nullptr;
    void*         trailer = nullptr;
    malloc_chunk* p       = mem2chunk(mem);

    if ((u32)mem % alignment) {
        // misaligned: find an aligned spot inside the chunk
        u8*           br      = (u8*)mem2chunk((void*)(((u32)(mem + alignment - 1)) & (0 - alignment)));
        u8*           pos     = (u32)(br - (u8*)p) >= min_chunk_size ? br : br + alignment;
        malloc_chunk* newp    = (malloc_chunk*)pos;
        u32           leadsize = pos - (u8*)p;
        u32           newsize  = chunksize(p) - leadsize;

        if (is_mmapped(p)) {
            // for mmapped chunks, just adjust the offset
            newp->prev_foot = p->prev_foot + leadsize;
            newp->head      = newsize | cinuse_bit;
        } else {
            // give back the leader, use the rest
            set_inuse(m, newp, newsize);
            set_inuse(m, p, leadsize);

            leader = chunk2mem(p);
        }

        p = newp;
    }

    // give back the spare room at the end
    if (!is_mmapped(p)) {
        u32 size = chunksize(p);

        if (size > nb + min_chunk_size) {
            u32           remainder_size = size - nb;
            malloc_chunk* remainder      = chunk_plus_offset(p, nb);

            set_inuse(m, p, nb);
            set_inuse(m, remainder, remainder_size);

            trailer = chunk2mem(remainder);
        }
    }

    if (leader)
        mspace_free(m, leader);

    if (trailer)
        mspace_free(m, trailer);

    return chunk2mem(p);
}

// inlined @ sub_5FAF00
// inlined @ sub_5FBE90
void* mm::mspace_memalign(malloc_state* msp, u32 alignment, u32 bytes) {
    if (!ok_magic(msp))
        usage_error_action();

    return internal_memalign(msp, alignment, bytes);
}

// inlined @ sub_5FAF00
// inlined @ sub_5FAFB0
// inlined @ sub_5FABF0
u32 mm::mspace_usable_size(void* mem) {
    if (!mem)
        return 0;

    malloc_chunk* p  = mem2chunk(mem);
    malloc_state* fm = get_mstate_for(p);

    if (!ok_magic(fm))
        usage_error_action();

    if (ok_address(fm, p) && cinuse(p))
        return chunksize(p) - chunk_overhead;

    return 0;
}
