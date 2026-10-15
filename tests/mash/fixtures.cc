#include <doctest/doctest.h>

#include "mash/fixtures.hh"
#include "treyarch/shared/memory/heap.hh"

void tests::mash_support::trace_t::push(e_event                           kind,
                                        u32                               id,
                                        void*                             object,
                                        void*                             parent,
                                        treyarch::mash::buffer_type       buffer,
                                        treyarch::mash::mash_info_struct* reader) {

    events.push_back({ kind,
                       id,
                       object,
                       parent,
                       buffer,
                       reader ? reader->buffer_size_used[0] : 0,
                       reader ? reader->buffer_size_used[1] : 0 });
}

tests::mash_support::record_t::~record_t() {
    if (trace) {
        trace->push(event_destructor, id, this);
        trace->destructor_saw_live_storage &= current_memory_fixture && current_memory_fixture->contains(this);
    }
}

void tests::mash_support::record_t::unmash(treyarch::mash::mash_info_struct* reader,
                                           void*                             parent,
                                           treyarch::mash::buffer_type       buffer) {

    if (trace)
        trace->push(event_unmash, id, this, parent, buffer, reader);
}

void tests::mash_support::record_t::construct_mashed_class() {
    if (trace)
        trace->push(event_construct, id, this);
}

void tests::mash_support::record_t::destruct_mashed_class() {
    if (trace)
        trace->push(event_destruct_mashed, id, this);
}

tests::mash_support::c_virtual_record::c_virtual_record(u32 id,
                                                        trace_t* trace) : id(id),
                                                                          trace(trace) {}

tests::mash_support::c_virtual_record::~c_virtual_record() {
    if (trace) {
        trace->push(event_destructor, id, this);
        trace->destructor_saw_live_storage &= current_memory_fixture && current_memory_fixture->contains(this);
    }
}

void tests::mash_support::c_virtual_record::construct_mashed_class() {
    if (trace)
        trace->push(event_construct, id, this);
}

void tests::mash_support::c_virtual_record::destruct_mashed_class() {
    if (trace)
        trace->push(event_destruct_mashed, id, this);
}

void tests::mash_support::c_virtual_record::unmash(treyarch::mash::mash_info_struct* reader, void* parent,
                                                treyarch::mash::buffer_type buffer) {
    
    if (trace)
        trace->push(event_unmash, id, this, parent, buffer, reader);
}

treyarch::mash::virtual_types_key tests::mash_support::c_virtual_record::get_virtual_type_key() const {
    return 0x1001;
}

bool tests::mash_support::c_virtual_record::is_subclass_of(treyarch::mash::virtual_types_key key) const {
    return key == 0x1000;
}

i32 tests::mash_support::c_virtual_record::get_mash_sizeof() const {
    return sizeof(*this);
}

void* tests::mash_support::c_virtual_record::operator new(size_t size) {
    return treyarch::memory::heap::allocate((u32)size);
}

void tests::mash_support::c_virtual_record::operator delete(void* allocation) {
    treyarch::memory::heap::free(allocation);
}

tests::mash_support::c_string_fixture::c_string_fixture() : m_empty_binding(treyarch::mash::references::null_string_guts, empty_storage.data() + 16),
                                                            m_count_binding(treyarch::mash::references::string_count, &string_count),
                                                            m_slab_binding (treyarch::mash::references::string_source_slab, &source_slab),
                                                            m_upper_binding(treyarch::mash::references::upper_case_table, upper_table.data()) {

    // leave guard bytes on both sides of the empty string so negative clear offsets remain inside test storage.
    empty_storage.fill('?');
    empty_storage[16] = '\0';

    for (u32 value = 'a'; value <= 'z'; ++value)
        upper_table[value] = (u8)(value - 32);

    // a zero table entry means leave the input byte alone; this is a byte mapping rather than unicode casing.
    for (u32 value = 0xE0; value <= 0xFF; ++value) {
        if (value != 0xF7)
            upper_table[value] = (u8)(value - 32);
    }
}

tests::mash_support::c_string_fixture::~c_string_fixture() {
    CHECK_MESSAGE(string_count == 0, "unbalanced string construction and teardown");
}

tests::mash_support::c_registry_fixture::c_registry_fixture() : m_binding(treyarch::mash::detail::references::vtable_registry,
                                                                          (treyarch::mash::detail::vtable_registry*)&m_registry) {

    set({});
}

void tests::mash_support::c_registry_fixture::set(std::initializer_list<registry_entry_t> entries,
                                                  u32                                     mask,
                                                  u32                                     count) {

    std::vector<registry_entry_t> sorted(entries);

    std::sort(sorted.begin(), sorted.end(), [](const auto &left, const auto &right) {
        return left.bucket < right.bucket || (left.bucket == right.bucket && left.key < right.key);
    } );

    // all buckets share one circular list; the head is also the end marker returned by a failed lookup.
    m_nodes.resize(sorted.size());

    m_head.next     = m_nodes.empty() ? &m_head :  m_nodes.data();
    m_head.previous = m_nodes.empty() ? &m_head : &m_nodes.back();

    for (u32 index = 0; index < sorted.size(); ++index) {
        m_nodes[index] = {
            index + 1 < sorted.size() ?
                &m_nodes[index + 1] : &m_head,
            index ?
                &m_nodes[index - 1] : &m_head,
            sorted[index].key,
            sorted[index].value
        };
    }

    // the next bucket's start is this bucket's end, including when several empty buckets share a boundary.
    for (u32 bucket = 0; bucket <= count; ++bucket) {
        u32 index = 0;

        while (index < sorted.size() && sorted[index].bucket < bucket)
            ++index;

        m_bounds[bucket] = index < sorted.size() ? &m_nodes[index] : &m_head;
    }

    m_registry.head          = &m_head;
    m_registry.bucket_bounds = m_bounds.data();
    m_registry.bucket_mask   = mask;
    m_registry.bucket_count  = count;
}