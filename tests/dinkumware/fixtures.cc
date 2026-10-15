#include "dinkumware/fixtures.hh"

u32 tests::container_support::value_trace_t::count(e_value_event kind) const {
    u32 result = 0;

    for (const value_event_t &event : events)
        result += event.kind == kind;

    return result;
}

std::vector<i32> tests::container_support::value_trace_t::values(e_value_event kind) const {
    std::vector<i32> result;

    for (const value_event_t &event : events) {
        if (event.kind == kind)
            result.push_back(event.value);
    }

    return result;
}

tests::container_support::tracked_value_t::tracked_value_t(i32            value,
                                                           value_trace_t &trace) : value(value),
                                                                                   trace(&trace) {

    ++trace.live;
    trace.events.push_back({value_constructed, value});
}

tests::container_support::tracked_value_t::tracked_value_t(const tracked_value_t &other) : value(other.value),
                                                                                           trace(other.trace) {

    ++trace->live;
    trace->events.push_back({value_copied, value});
}

tests::container_support::tracked_value_t::tracked_value_t(tracked_value_t &&other) noexcept : value(other.value),
                                                                                               trace(other.trace) {

    ++trace->live;
    trace->events.push_back({value_moved, value});
    other.value = -1;
}

tests::container_support::tracked_value_t::~tracked_value_t() {
    trace->events.push_back({value_destroyed, value});
    --trace->live;
}

tests::container_support::tracked_value_t&
tests::container_support::tracked_value_t::operator=(const tracked_value_t &other) {
    value = other.value;
    trace->events.push_back({value_copy_assigned, value});

    return *this;
}

tests::container_support::tracked_value_t&
tests::container_support::tracked_value_t::operator=(tracked_value_t &&other) noexcept {
    value = other.value;
    trace->events.push_back({value_move_assigned, value});
    other.value = -1;

    return *this;
}

std::vector<u32> tests::container_support::shuffled_keys(u32 count, u32 seed) {
    std::vector<u32> result;

    for (u32 key = 0; key < count; ++key)
        result.push_back(key);

    // use a fixed sequence rather than a library-dependent shuffle so a failure can be replayed on the ci runner.
    for (u32 remaining = count; remaining > 1; --remaining) {
        seed = seed * 1664525u + 1013904223u;
        std::swap(result[remaining - 1], result[seed % remaining]);
    }

    return result;
}
