#pragma once

namespace treyarch { namespace memory {
    struct fixed_pool;
}}

namespace treyarch { namespace event_pools {
    memory::fixed_pool &event_pool();
    memory::fixed_pool &event_type_pool();
    memory::fixed_pool &recipient_pool();
}} // treyarch::event_pools
