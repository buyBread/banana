#pragma once

#include <algorithm>
#include <array>
#include <cstring>
#include <initializer_list>
#include <new>
#include <utility>
#include <vector>
#include <doctest/doctest.h>

#include "support/memory.hh"
#include "treyarch/mash/mash_info.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/memory_reference.hh"

// lets doctest print a mash string's text when a check on it fails.
namespace doctest {
    template<>
    struct StringMaker<treyarch::mash::string> {
        static String convert(const treyarch::mash::string &value) {
            return String("\"") + value.c_str() + "\"";
        }
    };
}

namespace tests { namespace mash_support {
    // these tests double as a walkthrough of mash, the engine's way of loading saved objects straight out of memory.

    // the game's tools take a bunch of connected c++ objects and write them all into one block of bytes, called a mash image.
    // the game loads that block as-is and then unmashes it, which turns those bytes back into objects it can use.
    // objects point at each other with pointers, which are just memory addresses.
    // an address saved by the tools means nothing in the game, since the image gets loaded somewhere else entirely.
    // so unmashing walks the image object by object and rewrites every saved pointer to the spot in the image where its target ended up.
    // nothing gets copied out along the way; the objects keep living inside the image, so it has to stay alive while they're in use.

    // the image is split into two streams, normal and shared.
    // normal holds the objects themselves, while shared holds the little things that steer the walk, like counts and string lengths.
    // each stream has its own read position, a cursor, which only moves forward as things get read.

    // loading is two passes over the objects.
    // unmash rewrites the pointers first; construct_mashed_class then gets every object ready for use.
    // it can't be done with ordinary constructors, since those would reset the very fields that were just loaded.
    // unloading has its own method too, destruct_mashed_class, which cleans up without freeing bytes that belong to the image.

    // engine globals normally sit at fixed addresses inside the running game, which doesn't exist here.
    // a binding points one of those globals at some storage in our test instead, then puts the old address back when it goes away.
    template<typename T>
    class c_reference_binding {
        util::memory_reference<T> &m_reference;
        util::memory_reference<T>  m_saved;

    public:
        c_reference_binding(util::memory_reference<T> &reference,
                            T*                         storage) : m_reference(reference), m_saved(reference) {

            m_reference = util::memory_reference<T>((u32)storage);
        }

        ~c_reference_binding() {
            m_reference = m_saved;
        }

        c_reference_binding(const c_reference_binding&)            = delete;
        c_reference_binding &operator=(const c_reference_binding&) = delete;
    };

    // a hand-built mash image; every number passed to its helpers is a byte offset from the start of the image.
    // put(16, u32{7}) writes four bytes at offset 16, and at<u32>(16) gives a pointer to those same four bytes.
    // neither of them runs any constructors, they only deal with raw bytes.
    template<size_t size = 1024>
    struct alignas(64) image_t {
        std::array<u8, size> bytes{};

        template<typename T>
        T* at(u32 offset) {
            return (T*)(bytes.data() + offset);
        }

        template<typename T>
        void put(u32 offset, const T &value) {
            std::memcpy(bytes.data() + offset, &value, sizeof(value));
        }

        // builds a fresh T right inside the image, which is where loaded objects live too.
        template<typename T>
        T* make(u32 offset) {
            return ::new (at<T>(offset)) T{};
        }

        void header(u32 shared_offset) {
            // the header takes up the first 16 bytes; the normal stream starts right after it and the shared one at shared_offset.
            // in_use is a one byte flag that resource loading flips on and off.
            // the three bytes after it get odd values, so a write that spills over in_use would get noticed.
            put(0,
                treyarch::mash::mash_info_struct::mash_header{ treyarch::mash::mash_info_struct::get_master_crc(),
                                                               16,
                                                               shared_offset,
                                                               0,
                                                               0x1E,
                                                               0x49,
                                                               0x28 });
        }
    };

    enum e_event {
        event_unmash,
        event_construct,
        event_destruct_mashed,
        event_destructor
    };

    struct event_t {
        e_event                     kind;
        u32                         id;
        void*                       object;
        void*                       parent;
        treyarch::mash::buffer_type buffer;
        u32                         normal_used;
        u32                         shared_used;
    };

    // a log of the mash methods called on our stand-in objects, in the order they were called.
    // normal_used and shared_used are snapshots of both cursors, which shows how much had been read at that point.
    struct trace_t {
        std::vector<event_t> events;
        bool destructor_saw_live_storage = true;

        void push(e_event                           kind,
                  u32                               id,
                  void*                             object,
                  void*                             parent = nullptr,
                  treyarch::mash::buffer_type       buffer = treyarch::mash::NORMAL_BUFFER,
                  treyarch::mash::mash_info_struct* reader = nullptr);
    };

    // a tiny stand-in for a game object; every mash method it has just adds an event to its trace.
    // it's eight bytes in this 32-bit build, its id and then its trace pointer.
    struct record_t {
        u32      id;
        trace_t* trace;

        ~record_t();

        void unmash(treyarch::mash::mash_info_struct* reader,
                    void*                             parent,
                    treyarch::mash::buffer_type       buffer);

        void construct_mashed_class();
        void destruct_mashed_class();
    };

    // writes a saved record into the image and returns where it sits.
    template<size_t size>
    record_t* put_record(image_t<size> &image,
                         u32            offset,
                         u32            id,
                         trace_t       &trace) {

        image.put(offset,     id);
        image.put(offset + 4, &trace);

        return image.template at<record_t>(offset);
    }

    // c_virtual_record answers to virtual_record_key and claims to be a subclass of virtual_record_parent_key.
    constexpr u32 virtual_record_key        = 0x1001;
    constexpr u32 virtual_record_parent_key = 0x1000;

    // the same stand-in as record_t, but with virtual methods.
    // a class with virtual methods gets a hidden first field, a pointer to its vtable, which is a table of those methods.
    // that makes it twelve bytes; the vtable pointer, the id and the trace pointer.
    // a vtable's address changes between program runs, so a saved one is useless and the image holds a type key in its place.
    // virtual_types.cc goes over how the key turns back into a vtable.
    class c_virtual_record : public treyarch::mash::mash_virtual_base {

    public:
        u32      id;
        trace_t* trace;

        c_virtual_record(u32 id, trace_t* trace);
       ~c_virtual_record() override;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;

        void unmash(treyarch::mash::mash_info_struct* reader,
                    void*                             parent,
                    treyarch::mash::buffer_type       buffer) override;

        treyarch::mash::virtual_types_key get_virtual_type_key() const override;

        bool is_subclass_of(treyarch::mash::virtual_types_key key) const override;

        i32 get_mash_sizeof() const override;

        void* operator new(size_t size);
    };

    // writes a saved virtual record into the image, with its type key where the vtable pointer goes.
    template<size_t size>
    c_virtual_record* put_virtual_record(image_t<size> &image,
                                         u32            offset,
                                         u32            id,
                                         trace_t       &trace) {

        image.put(offset,     virtual_record_key);
        image.put(offset + 4, id);
        image.put(offset + 8, &trace);

        return image.template at<c_virtual_record>(offset);
    }

    // the first four bytes of an object with virtual methods; its vtable pointer, or a type key while it's still unloaded.
    inline u32 first_word(const void* object) {
        return *(const u32*)object;
    }

    // mash strings rely on a few globals even outside of loading.
    // empty strings all point at one shared empty string, string_count tracks how many strings are alive and upper_table drives to_upper.
    // source_slab is only ever compared by address; a string carrying it borrows its characters from an image and must never free them.
    class c_string_fixture : public c_memory_fixture {

    public:
        std::array<char, 32> empty_storage {};
        i32                  string_count = 0;
        u32                  slab_token   = 0;
        void*                source_slab  = &slab_token;
        std::array<u8, 256>  upper_table {};

        c_string_fixture();
        ~c_string_fixture();

        // the shared empty string; it sits in the middle of empty_storage, so writes just before it stay inside the array.
        char* empty_string() {
            return empty_storage.data() + 16;
        }

        // makes value borrow characters, the same way unmash leaves a loaded string.
        void borrow(treyarch::mash::string &value, char* characters);

    private:
        c_reference_binding<char>  m_empty_binding;
        c_reference_binding<i32>   m_count_binding;
        c_reference_binding<void*> m_slab_binding;
        c_reference_binding<u8>    m_upper_binding;
    };

    // a mash string's ordinary constructors leave its source slab field alone, so a string on the stack would start with junk there.
    // this keeps the string in zeroed storage instead and uses placement new, which runs a constructor on memory we already have.
    // the destructor runs when the slot goes away, unless release handed that job back to the test.
    class c_string_slot {
        alignas(treyarch::mash::string) u8 m_storage[sizeof(treyarch::mash::string)] {};

        treyarch::mash::string* m_value;

    public:
        template<typename... Args>
        explicit c_string_slot(Args&&... args) {
            m_value = ::new (m_storage) treyarch::mash::string(std::forward<Args>(args)...);
        }

        ~c_string_slot() {
            if (m_value)
                m_value->~string();
        }

        c_string_slot(const c_string_slot&)            = delete;
        c_string_slot &operator=(const c_string_slot&) = delete;

        treyarch::mash::string* operator->() { return  m_value; }
        treyarch::mash::string &operator *() { return *m_value; }

        treyarch::mash::string* release() {
            // for tests that tear the string down themselves, so it doesn't get torn down twice.
            return std::exchange(m_value, nullptr);
        }
    };

    // the vtable registry maps type keys to vtables; it's normally built by the game while it starts up.
    // its entries sit in one circular linked list sorted into buckets, which are runs of that list that lookup searches through.
    // these structs copy the registry's memory layout, so a test can hand lookup a registry of its own.
    struct registry_node_t {
        registry_node_t* next;
        registry_node_t* previous;
        u32              key;
        u32              value;
    };

    struct registry_layout_t {
        u32               traits;
        u32               entries_allocator;
        registry_node_t*  entries_head;
        u32               entries_size;
        u32               bounds_allocator;
        registry_node_t** bounds_first;
        registry_node_t** bounds_last;
        registry_node_t** bounds_end;
        u32               mask;
        u32               bucket_count;
    };

    static_assert(sizeof(registry_layout_t) == sizeof(treyarch::vc8::hash_map<u32, u32>));

    struct registry_entry_t {
        // the bucket is spelled out rather than computed, so the test can't share a mistake with the lookup it's testing.
        u32 bucket;
        u32 key;
        u32 value;
    };

    class c_registry_fixture {
        registry_node_t                 m_head {};
        std::vector<registry_node_t>    m_nodes;
        std::array<registry_node_t*, 9> m_bounds {};
        registry_layout_t               m_registry {};

        c_reference_binding
            <treyarch::vc8::hash_map<u32, u32>> m_binding;

    public:
        u32 virtual_record_vtable = 0;

        c_registry_fixture();

        void set(std::initializer_list<registry_entry_t> entries,
                 u32                                     mask  = 7,
                 u32                                     count = 8);

        // fills the registry with just c_virtual_record, so a saved virtual_record_key turns into its vtable.
        void register_virtual_record();
    };
}}
