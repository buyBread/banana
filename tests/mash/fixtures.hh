#pragma once

#include <algorithm>
#include <array>
#include <cstring>
#include <initializer_list>
#include <new>
#include <utility>
#include <vector>

#include "support/memory.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace mash { namespace detail {
    struct vtable_registry;
    namespace references {
        extern util::memory_reference<vtable_registry> vtable_registry;
    }
}}}

namespace tests { namespace mash_support {
    // mashing saves connected c++ objects into a block of bytes called a mash image.
    // loading those bytes gives us the stored member values, but the saved pointers are not usable addresses.
    // unmashing visits the objects and replaces their pointer fields with addresses inside the loaded block.
    // the first object is called the root; its unmash method leads us to the objects it contains or points to.

    // the bytes are split into two parts called the normal and shared streams.
    // object bodies usually occupy the normal stream; counts and string lengths come from the shared stream.
    // each stream has its own next-read position, called a cursor; reader.cc shows how these positions move.

    // unmash repairs the connections first; construct_mashed_class then initializes the objects for use.
    // an ordinary constructor can reset loaded fields; these construction methods preserve what unmash just repaired.
    // traversal.cc follows a parent and child through both steps; containers.cc and arrays.cc add collections.

    // borrowed objects use bytes inside the image rather than memory allocated just for that object.
    // their destruct_mashed_class methods do cleanup, but the image must stay alive until its objects are finished.
    // strings.cc covers borrowed characters; virtual_types.cc covers virtual methods; graphs.cc loads resource types.

    // engine globals normally refer to addresses inside the running game.
    // a binding points one of those globals at our test's storage, then restores its old address on scope exit.
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

    // each test builds a small image by hand; the numbers passed to put and at are byte offsets from its beginning.
    // put writes a value at that offset, while at returns a pointer to those bytes as the requested type.
    // for example, put(16, u32{7}) writes four bytes and at<u32>(16) points at the same value.
    // neither helper constructs an object; the test decides when to run constructors or mash construction methods.
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

        void header(u32 shared_offset) {
            // a header occupies bytes 0 through 15; the root begins at byte 16 and shared_offset starts the shared data.
            // in_use is a one-byte flag set by resource loading; its neighbors have distinct values to catch wider writes.
            put(0,
                treyarch::mash::mash_info_struct::mash_header{ 0x12345678,
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

    // a record is a small stand-in for a game object; each of its mash methods appends an event to this list.
    // its id identifies the object, while normal_used and shared_used capture the reader positions at that moment.
    // tests can then check which object was visited first and how much data had already been read.
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

    template<size_t size>
    record_t* put_record(image_t<size> &image,
                        u32             offset,
                        u32             id,
                        trace_t        &trace) {

        // a record occupies eight bytes in this 32-bit build; its id comes first and its trace pointer comes second.
        // copying those fields leaves the event list untouched until the test actually visits the record.
        image.put(offset, id);
        image.put(offset + 4, &trace);
        return image.template at<record_t>(offset);
    }

    // this record has virtual methods and extra fields beyond mash_virtual_base.
    // the reader must restore its virtual table before asking how large the complete object is.
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
        void  operator delete(void* allocation);
    };

    // mash strings use several globals even when no resource is being loaded.
    // empty strings point at empty_storage; string_count tracks constructed strings; upper_table maps uppercase bytes.
    // source_slab is an address used as a label for borrowed characters; the string compares labels before freeing them.
    class c_string_fixture : public c_memory_fixture {
    public:
        std::array<char, 32> empty_storage {};
        i32                  string_count = 0;
        u32                  slab_token   = 0;
        void*                source_slab  = &slab_token;
        std::array<u8, 256>  upper_table {};

        c_string_fixture();
        ~c_string_fixture();

    private:
        c_reference_binding<char>  m_empty_binding;
        c_reference_binding<i32>   m_count_binding;
        c_reference_binding<void*> m_slab_binding;
        c_reference_binding<u8>    m_upper_binding;
    };

    // an ordinary string constructor does not initialize the field that marks borrowed characters.
    // this helper starts with zeroed bytes, then uses placement new to run the constructor in that existing storage.
    // it calls the destructor on scope exit unless release has handed cleanup back to the test.
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

        c_string_slot(const c_string_slot&) = delete;
        c_string_slot &operator=(const c_string_slot&) = delete;

        treyarch::mash::string* operator->() { return  m_value; }
        treyarch::mash::string &operator *() { return *m_value; }

        treyarch::mash::string* release() {
            // tests that call finalize or destruct_mashed_class themselves must avoid a second teardown here.
            return std::exchange(m_value, nullptr);
        }
    };

    // virtual objects store a type key in the image; the registry maps each key to a callable c++ virtual table.
    // its entries form a linked list, divided into smaller search ranges called buckets.
    // these structs reproduce the memory layout read by lookup; the fixture supplies entries without starting the game.
    struct registry_node_t {
        registry_node_t* next;
        registry_node_t* previous;
        u32              key;
        u32              value;
    };

    struct registry_layout_t {
        u32               unk_00;
        u32               unk_04;
        registry_node_t*  head;
        u32               unk_0c;
        u32               unk_10;
        registry_node_t** bucket_bounds;
        u32               unk_18;
        u32               unk_1c;
        u32               bucket_mask;
        u32               bucket_count;
    };

    struct registry_entry_t {
        // buckets are supplied explicitly rather than computed by the lookup code being tested.
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
            <treyarch::mash::detail::vtable_registry> m_binding;

    public:
        c_registry_fixture();
        
        void set(std::initializer_list<registry_entry_t> entries,
                 u32                                     mask = 7,
                 u32                                     count = 8);
    };
}}
