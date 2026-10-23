#include "vc8/fixtures.hh"

u32 tests::vc8_support::value_trace_t::count(e_value_event kind) const {
    u32 result = 0;

    for (const value_event_t &event : events)
        result += event.kind == kind;

    return result;
}

std::vector<i32> tests::vc8_support::value_trace_t::values(e_value_event kind) const {
    std::vector<i32> result;

    for (const value_event_t &event : events) {
        if (event.kind == kind)
            result.push_back(event.value);
    }

    return result;
}

tests::vc8_support::tracked_value_t::tracked_value_t(i32            value,
                                                     value_trace_t &trace) : value(value),
                                                                             trace(&trace) {

    ++trace.live;
    trace.events.push_back({ value_constructed, value });
}

tests::vc8_support::tracked_value_t::tracked_value_t(const tracked_value_t &other) : value(other.value),
                                                                                     trace(other.trace) {

    ++trace->live;
    trace->events.push_back({ value_copied, value });
}

tests::vc8_support::tracked_value_t::tracked_value_t(tracked_value_t &&other) noexcept : value(other.value),
                                                                                         trace(other.trace) {

    ++trace->live;
    trace->events.push_back({ value_moved, value });

    other.value = -1;
}

tests::vc8_support::tracked_value_t::~tracked_value_t() {
    trace->events.push_back({ value_destroyed, value });
    --trace->live;
}

tests::vc8_support::tracked_value_t &tests::vc8_support::tracked_value_t::operator=(const tracked_value_t &other) {
    value = other.value;
    trace->events.push_back({ value_copy_assigned, value });

    return *this;
}

tests::vc8_support::tracked_value_t &tests::vc8_support::tracked_value_t::operator=(tracked_value_t &&other) noexcept {
    value = other.value;
    trace->events.push_back({ value_move_assigned, value });

    other.value = -1;

    return *this;
}

std::vector<u32> tests::vc8_support::shuffled_keys(u32 count, u32 seed) {
    std::vector<u32> result;
    random_t         random { seed };

    for (u32 key = 0; key < count; ++key)
        result.push_back(key);

    for (u32 remaining = count; remaining > 1; --remaining)
        std::swap(result[remaining - 1], result[random.next() % remaining]);

    return result;
}
