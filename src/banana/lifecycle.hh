#pragma once

#include <string_view>
#include <format>

#include "banana/logging.hh"

enum class e_lifecycle {
    pending,
    ready,      // acquired device
    failed,
    rebuilding, // lost device
    stopping,
};

template <> struct std::formatter<e_lifecycle> : std::formatter<std::string_view> {
    auto format(e_lifecycle v, std::format_context &ctx) const {
        std::string_view as_string;

        switch(v) {
            case e_lifecycle::pending:    as_string = "pending";    break;
            case e_lifecycle::ready:      as_string = "ready";      break;
            case e_lifecycle::failed:     as_string = "failed";     break;
            case e_lifecycle::rebuilding: as_string = "rebuilding"; break;
            case e_lifecycle::stopping:   as_string = "stopping";   break;
        }

        return std::formatter<std::string_view>::format(as_string, ctx);
    }
};

namespace banana {
    inline std::atomic<e_lifecycle> _state { e_lifecycle::pending };

    namespace state {
        inline e_lifecycle current() {
            return _state.load(std::memory_order_acquire);
        }

        inline void poll(e_lifecycle current_state) {
            banana::_state.wait(current_state, std::memory_order_acquire);
        }

        inline void update(e_lifecycle next) {
            auto current_state = current();

            for (;;) {
                switch(current_state) {
                    case e_lifecycle::pending:
                        if (next == e_lifecycle::ready  ||
                            next == e_lifecycle::failed ||
                            next == e_lifecycle::stopping)
                            
                            break;

                        return;
                    case e_lifecycle::ready:
                        if (next == e_lifecycle::rebuilding ||
                            next == e_lifecycle::failed     ||
                            next == e_lifecycle::stopping)
                            
                            break;

                        return;
                    case e_lifecycle::rebuilding:
                        if (next == e_lifecycle::ready  ||
                            next == e_lifecycle::failed ||
                            next == e_lifecycle::stopping)
                            
                            break;

                        return;
                    case e_lifecycle::failed:
                    case e_lifecycle::stopping:
                        return;
                }

                if (_state.compare_exchange_weak(current_state,
                                                 next,
                                                 std::memory_order_release,
                                                 std::memory_order_acquire)) {
                        
                    log.dbg("banana::state: {} -> {}", current_state, next);

                    _state.notify_all();

                    return;
                }
            }
        }
    } // state
} // banana