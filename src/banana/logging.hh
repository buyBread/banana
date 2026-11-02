#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <deque>
#include <filesystem>
#include <format>
#include <future>
#include <iterator>
#include <memory>
#include <mutex>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "banana/util/types.hh"
#include "banana/util/singleton.hh"
#include "banana/util/appdata_path.hh"

namespace banana {
    enum class log_level : u8 {
        msg, wrn, err, dbg,
    };

    struct log_line {
        u64 sequence = 0;

        std::chrono::system_clock::time_point time {};
        
        log_level level = log_level::msg;

        std::string category;
        std::string text;
    };

    class log_feed {
        const u64 m_capacity;

        mutable std::mutex m_mutex;

        std::deque<log_line> m_lines;
        u64                  m_next_sequence = 0;

    public:
        explicit log_feed(u64 capacity = 20000) : m_capacity(std::max<u64>(capacity, 1)) {}

        void append(std::span<log_line> lines) {
            std::lock_guard lock(m_mutex);

            for (auto &line : lines) {
                line.sequence = m_next_sequence++;
                m_lines.emplace_back(std::move(line));

                if (m_lines.size() > m_capacity)
                    m_lines.pop_front();
            }
        }

        // the caller owns cursor; start at 0 to read the available history.
        // returns the number of overwritten lines the caller missed.
        u64 read_since(u64 &cursor, std::vector<log_line> &out, u64 limit = 4096) const {
            std::lock_guard lock(m_mutex);

            const auto earliest = m_next_sequence - m_lines.size();
            const auto missed   = cursor < earliest ? earliest - cursor : 0;

            cursor = std::clamp(cursor, earliest, m_next_sequence);

            const auto take  = std::min<u64>(m_next_sequence - cursor, limit);
            const auto first = cursor - earliest;

            out.reserve(out.size() + static_cast<size_t>(take));

            for (u64 i = 0; i < take; ++i)
                out.emplace_back(m_lines[static_cast<size_t>(first + i)]);

            cursor += take;
            
            return missed;
        }

        u64 next_sequence() const {
            std::lock_guard lock(m_mutex);
            return m_next_sequence;
        }

        u64 capacity() const noexcept {
            return m_capacity;
        }
    };

    class s_logging_manager;

    class logger {
        friend class s_logging_manager;

        struct queued_item {
            log_level                             level = log_level::msg;
            std::chrono::system_clock::time_point time {};
            std::string                           text;
            std::shared_ptr<std::promise<void>>   barrier;
        };

        class mpsc_ring {
            struct slot {
                std::atomic<u64> sequence { 0 };
                queued_item      value;
            };

            static constexpr u64 capacity = 32768;
            static constexpr u64 mask     = capacity - 1;
            static_assert(capacity >= 2 && (capacity & mask) == 0, "capacity must be pow2");

                        std::unique_ptr<slot[]> m_slots;
            alignas(64) std::atomic<u64>        m_head { 0 };
            alignas(64) u64                     m_tail = 0;

        public:
            // bounded mpmc queue
            mpsc_ring() : m_slots(std::make_unique<slot[]>(capacity)) {
                for (u64 i = 0; i < capacity; ++i)
                    m_slots[i].sequence.store(i, std::memory_order_relaxed);
            }

            bool try_push(queued_item &item) noexcept {
                auto pos = m_head.load(std::memory_order_relaxed);

                for (;;) {
                    auto &cell = m_slots[pos & mask];
                    
                    const auto sequence   = cell.sequence.load(std::memory_order_acquire);
                    const auto difference = static_cast<i64>(sequence - pos);

                    if (difference == 0) {
                        if (m_head.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed, std::memory_order_relaxed)) {
                            cell.value = std::move(item);
                            cell.sequence.store(pos + 1, std::memory_order_release);
                            
                            return true;
                        }
                    } else if (difference < 0)
                        return false;
                    else
                        pos = m_head.load(std::memory_order_relaxed);
                }
            }

            // consumer only
            bool try_pop(queued_item &item) noexcept {
                      auto &cell     = m_slots[m_tail & mask];
                const auto  sequence = cell.sequence.load(std::memory_order_acquire);

                if (static_cast<i64>(sequence - (m_tail + 1)) != 0)
                    return false;

                item = std::move(cell.value);
                
                cell.value = {};
                cell.sequence.store(m_tail + capacity, std::memory_order_release);
                
                ++m_tail;
                
                return true;
            }

            // Consumer only.
            bool empty() const noexcept {
                const auto &cell     = m_slots[m_tail & mask];
                const auto  sequence = cell.sequence.load(std::memory_order_acquire);
                
                return static_cast<i64>(sequence - (m_tail + 1)) != 0;
            }
        };

        std::string  m_category;
        log_feed    &m_feed;
        FILE*        m_file = nullptr;
        mpsc_ring    m_queue;

        std::atomic<bool> m_accepting { true };
        std::atomic<u32>  m_writers   { 0 };
        std::atomic<bool> m_sleeping  { false };

        std::atomic<u64> m_dropped_pending { 0 };
        std::atomic<u64> m_dropped_total   { 0 };

        std::jthread m_worker;

        static std::string_view watermark(log_level level) noexcept {
            switch (level) {
                case log_level::msg: return "[ MSG ]";
                case log_level::wrn: return "[ WRN ]";
                case log_level::err: return "[ ERR ]";
                case log_level::dbg: return "[ DBG ]";
            }
        }

        logger(std::string category, log_feed &feed) : m_category(std::move(category)), m_feed(feed) {
            const auto dir = util::appdata_path() / "logs";
            
            std::filesystem::create_directories(dir);

            const auto path = dir / std::format("{}-{}.txt", m_category, std::time(nullptr));

#ifdef _WIN32
            m_file = ::_wfopen(path.c_str(), L"ab");
#else
            m_file = std::fopen(path.string().c_str(), "ab");
#endif
            if (!m_file)
                throw std::runtime_error(std::format("cannot open log file: {}", path.string()));

            std::setvbuf(m_file, nullptr, _IOFBF, 1 << 20);

            try {
                m_worker = std::jthread([this](std::stop_token stop) { run(stop); });
            } catch (...) {
                std::fclose(m_file);

                m_file = nullptr;
                
                throw;
            }
        }

        void notify_worker() noexcept {
            if (m_sleeping.exchange(false, std::memory_order_acq_rel))
                m_sleeping.notify_one();
        }

        void publish(log_level level, std::string text) {
            m_writers.fetch_add(1, std::memory_order_acq_rel);

            if (!m_accepting.load(std::memory_order_acquire)) {
                m_writers.fetch_sub(1, std::memory_order_acq_rel);
                
                notify_worker();
                
                return;
            }

            queued_item item;
            item.level = level;
            item.time  = std::chrono::system_clock::now();
            item.text  = std::move(text);

            if (!m_queue.try_push(item)) {
                m_dropped_pending.fetch_add(1, std::memory_order_relaxed);
                m_dropped_total  .fetch_add(1, std::memory_order_relaxed);
            }

            m_writers.fetch_sub(1, std::memory_order_acq_rel);
            
            notify_worker();
        }

        void run(std::stop_token stop) {
            constexpr u64 batch_size = 256;

            std::vector<queued_item> batch;
            std::vector<log_line>    feed_batch;
            std::string              file_text;

            batch     .reserve(batch_size);
            feed_batch.reserve(batch_size + 1);
            file_text .reserve(batch_size * 128);

            std::chrono::sys_seconds cached_second {};
            std::string              cached_time;

            auto commit = [&](bool flush_file) {
                if (!file_text.empty()) {
                    std::fwrite(file_text.data(), 1, file_text.size(), m_file);
                    
                    file_text.clear();
                }

                if (!feed_batch.empty()) {
                    m_feed.append(feed_batch);
                    
                    feed_batch.clear();
                }

#ifndef DEBUG
                if (flush_file)
#endif
                    std::fflush(m_file);
            };

            auto append = [&](log_level                             level,
                              std::chrono::system_clock::time_point time,
                              std::string                           text) {

                const auto second = std::chrono::floor<std::chrono::seconds>(time);

                if (cached_time.empty() || second != cached_second) {
                    cached_second = second;
                    cached_time = std::format("{:%Y-%m-%d %H:%M:%S}", second);
                }

                const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(time - second).count();

                std::format_to(std::back_inserter(file_text), "{}.{:03} | {} | {} :: {}\n",
                               cached_time,
                               millis,
                               watermark(level),
                               m_category,
                               text);

                feed_batch.emplace_back(log_line {
                    0, time, level, m_category, std::move(text)
                });

                return level == log_level::wrn || level == log_level::err;
            };

            for (;;) {
                queued_item item;
                
                batch.clear();

                while (batch.size() < batch_size && m_queue.try_pop(item)) {
                    batch.emplace_back(std::move(item));
                    
                    item = {};
                }

                if (!batch.empty()) {
                    bool flush_file = false;

                    for (auto &record : batch) {
                        if (record.barrier) {
                            commit(true);

                            record.barrier->set_value();
                        } else
                            flush_file |= append(record.level, record.time, std::move(record.text));
                    }

                    const auto dropped = m_dropped_pending.exchange(0, std::memory_order_relaxed);

                    if (dropped) {
                        flush_file = true;

                        append(log_level::wrn,
                               std::chrono::system_clock::now(),
                               std::format("dropped {} log messages (queue full)", dropped));
                    }

                    commit(flush_file);

                    continue;
                }

                if (stop.stop_requested() && m_writers.load(std::memory_order_acquire) == 0 && m_queue.empty())
                    break;

                m_sleeping.store(true, std::memory_order_release);

                if (!m_queue.empty() || (stop.stop_requested() && m_writers.load(std::memory_order_acquire) == 0)) {
                    m_sleeping.store(false, std::memory_order_release);

                    continue;
                }

                m_sleeping.wait(true, std::memory_order_acquire);
            }

            const auto dropped = m_dropped_pending.exchange(0, std::memory_order_relaxed);

            if (dropped) {
                append(log_level::wrn,
                       std::chrono::system_clock::now(),
                       std::format("dropped {} log messages (queue full)", dropped));
                commit(true);
            }

            std::fflush(m_file);
        }

        void stop() {
            m_accepting.store(false, std::memory_order_release);

            if (m_worker.joinable()) {
                m_worker.request_stop();
                
                notify_worker();
                
                m_worker.join();
            }
        }

    public:
        logger           (const logger&) = delete;
        logger &operator=(const logger&) = delete;

        ~logger() {
            stop();

            if (m_file) {
                std::fclose(m_file);
                
                m_file = nullptr;
            }
        }

        const std::string &category() const noexcept {
            return m_category;
        }

        u64 dropped_total() const noexcept {
            return m_dropped_total.load(std::memory_order_relaxed);
        }

        // don't call from this logger's worker thread.
        void flush() {
            if (std::this_thread::get_id() == m_worker.get_id())
                throw std::logic_error("logger::flush() called on its worker");

            m_writers.fetch_add(1, std::memory_order_acq_rel);

            if (!m_accepting.load(std::memory_order_acquire)) {
                m_writers.fetch_sub(1, std::memory_order_acq_rel);
                notify_worker();
                
                return;
            }

            auto completion = std::make_shared<std::promise<void>>();
            auto finished   = completion->get_future();

            queued_item item;
            item.barrier = std::move(completion);

            while (!m_queue.try_push(item))
                std::this_thread::yield();

            m_writers.fetch_sub(1, std::memory_order_acq_rel);
            notify_worker();
            finished.wait();
        }

        template <typename... args_t>
        void write(log_level level, std::format_string<args_t...> fmt, args_t &&...args) {
            if (!m_accepting.load(std::memory_order_relaxed))
                return;

            publish(level, std::format(fmt, std::forward<args_t>(args)...));
        }
        void write(log_level level, std::string_view text) {
            if (!m_accepting.load(std::memory_order_relaxed))
                return;

            publish(level, std::string(text));
        }

        template <typename... args_t>
        void msg(std::format_string<args_t...> fmt, args_t &&...args) {
            write(log_level::msg, fmt, std::forward<args_t>(args)...);
        }
        void msg(std::string_view text) {
            write(log_level::msg, text);
        }

        template <typename... args_t>
        void wrn(std::format_string<args_t...> fmt, args_t &&...args) {
            write(log_level::wrn, fmt, std::forward<args_t>(args)...);
        }
        void wrn(std::string_view text) {
            write(log_level::wrn, text);
        }

        template <typename... args_t>
        void err(std::format_string<args_t...> fmt, args_t &&...args) {
            write(log_level::err, fmt, std::forward<args_t>(args)...);
        }
        void err(std::string_view text) {
            write(log_level::err, text);
        }

        template <typename... args_t>
        void dbg(std::format_string<args_t...> fmt, args_t &&...args) {
#ifdef DEBUG
            write(log_level::dbg, fmt, std::forward<args_t>(args)...);
#else
            (void)fmt;
            ((void)args, ...);
#endif
        }
        void dbg(std::string_view text) {
#ifdef DEBUG
            write(log_level::dbg, text);
#else
            (void)text;
#endif
        }
    };

    class s_logging_manager : public util::singleton<s_logging_manager> {
        friend class util::singleton<s_logging_manager>;

        std::mutex m_registry_mutex;
        
        log_feed m_feed;

        std::unordered_map
            <std::string,
             std::unique_ptr<logger>> m_categories;
        
        bool m_shutdown = false;

        s_logging_manager() = default;

    public:
        ~s_logging_manager() {
            shutdown();
        }

        // cache the returned reference on hot paths.
        logger &category(std::string_view name) {
            if (name.empty() || name.size() > 64 ||
                !std::all_of(name.begin(), name.end(), [](uchar ch) {
                    return (ch >= 'a' && ch <= 'z') ||
                           (ch >= 'A' && ch <= 'Z') ||
                           (ch >= '0' && ch <= '9') ||
                            ch == '_' || ch == '-';
                }))

                throw std::invalid_argument("log category must be 1-64 ASCII alphanumeric/_/- characters");

            std::lock_guard lock(m_registry_mutex);

            if (m_shutdown)
                throw std::logic_error("logging manager has shut down");

            std::string key(name);

            if (auto it = m_categories.find(key); it != m_categories.end())
                return *it->second;

            auto  owned  = std::unique_ptr<logger>(new logger(key, m_feed));
            auto &result = *owned;

            m_categories.emplace(std::move(key), std::move(owned));
            
            return result;
        }

        log_feed &feed() noexcept {
            return m_feed;
        }

        void flush_all() {
            std::lock_guard lock(m_registry_mutex);

            for (auto &[name, owned] : m_categories)
                owned->flush();
        }

        // terminal: stop producers before calling this.
        void shutdown() {
            std::lock_guard lock(m_registry_mutex);

            if (m_shutdown)
                return;

            m_shutdown = true;

            for (auto &[name, owned] : m_categories)
                owned->stop();
        }
    };
} // namespace banana
