#pragma once

namespace util {
    template <typename T> class singleton {
        
protected:
        singleton() = default;

public:
        singleton           (const singleton&) = delete;
        singleton& operator=(const singleton&) = delete;
        singleton           (singleton&&)      = delete;
        singleton& operator=(singleton&&)      = delete;

        static T &get() {
            static T instance;

            return instance;
        }
    };
} // util