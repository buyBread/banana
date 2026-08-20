#pragma once

/*
    banana's worker-based logging utility
    refactored to lighten the load on the game thread from trashy IO spam
*/

#include <cstdio>
#include <string>
#include <string_view>
#include <format>
#include <ctime>
#include <chrono>
#include <deque>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <filesystem>

#include "flags.hh"
#include "util/console.hh"
#include "util/singleton.hh"
#include "util/appdata_path.hh"
#include "util/types.hh"

class s_logging : public util::singleton<s_logging> {

    enum class e_log_type : u8 {
        msg, wrn, err, dbg
    };

    struct log_entry {
        e_log_type                            type;
        std::string                           text;
        std::chrono::system_clock::time_point time;
    };

    FILE* m_file = nullptr;

    std::deque<log_entry>   m_queue;
    std::mutex              m_queue_mutex;
    std::condition_variable m_queue_cv;
    std::jthread            m_worker;

    std::atomic<u16> m_dropped_messages = 0;

    static constexpr size_t max_queue_size = 8192; // some large-ish number for safety

    static std::string_view watermark(e_log_type type) {
        switch (type) {
            case e_log_type::msg: return "[ MSG ]";
            case e_log_type::wrn: return "[ WRN ]";
            case e_log_type::err: return "[ ERR ]";
            case e_log_type::dbg: return "[ DBG ]";
        }

        return {};
    }

    static std::string_view colored_watermark(e_log_type type) {
        switch (type) {
            case e_log_type::msg: return "[\033[36m MSG\033[0m ]"; // cyan
            case e_log_type::wrn: return "[\033[33m WRN\033[0m ]"; // yello
            case e_log_type::err: return "[\033[31m ERR\033[0m ]"; // red
            case e_log_type::dbg: return "[\033[35m DBG\033[0m ]"; // purpur
        }

        return {};
    }

    void open() {
        if (m_file)
            return;

        util::allocate_console();
        
        auto dir = util::appdata_path() / "logs";
        auto fp  = dir / std::format("{}.txt", std::time(nullptr));

        std::filesystem::create_directories(dir);

        m_file = std::fopen(fp.string().c_str(), "a");
    }

    void enqueue(log_entry&& entry) {
        bool notify = false;

        {
            std::lock_guard lock(m_queue_mutex);

            if (m_queue.size() >= max_queue_size) {
                ++m_dropped_messages;
                
                return;
            }

            notify = m_queue.empty();

            m_queue.emplace_back(std::move(entry));
        }

        if (notify)
            m_queue_cv.notify_one();
    }

    void worker_loop(std::stop_token stop) {
        open();

        std::deque<log_entry> local;

        for (;;) {
            { // wait for j*b
                std::unique_lock lock(m_queue_mutex);

                m_queue_cv.wait(lock, [&] {
                    return stop.stop_requested() || !m_queue.empty();
                });

                if (m_queue.empty() && stop.stop_requested())
                    break;

                local.swap(m_queue); // drain
            }

            std::string console_output;
            std::string file_output;

            /*
                avoid unnecessary reallocations
                should be enough...
            */
            console_output.reserve(local.size() * 384);
            file_output   .reserve(local.size() * 384);

            bool should_flush = false;

            for (auto& entry : local) {
#ifndef NDEBUG
                console_output += std::format("{} {}\n",
                    colored_watermark(entry.type),
                    entry.text);
#endif
                file_output    += std::format("{} | {} {}\n",
                    entry.time,
                    watermark(entry.type),
                    entry.text
                );

#if ALWAYS_FLUSH
                should_flush = true;
#else
                if (entry.type == e_log_type::wrn ||
                    entry.type == e_log_type::err)

                    should_flush = true;
#endif
            }

#ifndef NDEBUG
            if (!console_output.empty()) {
                std::fwrite(
                    console_output.data(),
                    1,
                    console_output.size(),
                    stdout
                );
            }
#endif
            if (m_file && !file_output.empty()) {
                std::fwrite(
                    file_output.data(),
                    1,
                    file_output.size(),
                    m_file
                );

                if (should_flush)
                    std::fflush(m_file);
            }

            local.clear();

            const auto dropped = m_dropped_messages.exchange(0);

            if (dropped != 0) {
#ifndef NDEBUG
                std::printf("%s",
                    std::format("{} dropped {} messages", colored_watermark(e_log_type::wrn), dropped).c_str());
#endif
            }
        }
    }

    template <typename... t_args>
    void log(e_log_type type, std::format_string<t_args...> fmt, t_args&&... args) {
        enqueue({
            .type = type,
            .text = std::format(fmt, std::forward<t_args>(args)...),
            .time = std::chrono::system_clock::now(),
        });
    }

public:
    s_logging() :
        m_worker([this](std::stop_token stop) {
            worker_loop(stop);
        }) {}

    ~s_logging() {
        close();
    }

    void close() {
        if (m_worker.joinable()) {
            m_worker  .request_stop();
            m_queue_cv.notify_all();
            m_worker  .join();
        }

        if (m_file) {
            std::fflush(m_file);
            std::fclose(m_file);

            m_file = nullptr;
        }
    }

    void flush() {
        std::fflush(m_file);
    }

    template <typename... t_args>
    void msg(std::format_string<t_args...> fmt, t_args&&... args) {
        log(e_log_type::msg, fmt, std::forward<t_args>(args)...);
    }
    void msg(std::string_view str) {
        log(e_log_type::msg, "{}", str);
    }

    template <typename... t_args>
    void wrn(std::format_string<t_args...> fmt, t_args&&... args) {
        log(e_log_type::wrn, fmt, std::forward<t_args>(args)...);
    }
    void wrn(std::string_view str) {
        log(e_log_type::wrn, "{}", str);
    }

    template <typename... t_args>
    void err(std::format_string<t_args...> fmt, t_args&&... args) {
        log(e_log_type::err, fmt, std::forward<t_args>(args)...);
    }
    void err(std::string_view str) {
        log(e_log_type::err, "{}", str);
    }

    template <typename... t_args>
    void dbg(std::format_string<t_args...> fmt, t_args&&... args) {
#ifndef DEBUG
        return;
#endif
        log(e_log_type::dbg, fmt, std::forward<t_args>(args)...);
    }
    void dbg(std::string_view str) {
#ifndef DEBUG
        return;
#endif
        log(e_log_type::dbg, "{}", str);
    }
};

namespace banana {
    inline auto &log = s_logging::get();
} // banana