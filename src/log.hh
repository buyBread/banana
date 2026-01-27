#pragma once

#include <ctime>
#include <chrono>
#include <cstdio>
#include <string>
#include <format>

#include "singleton.hh"

#define FMT std::format
#define LOG s_logging::get()

enum LOG_TYPE {
    msg, wrn, err, dbg
};

class s_logging : public c_singleton<s_logging> {

private:
    FILE* file = nullptr;

    /* RESET   | \033[0m
       RED     | \033[31m
       GREEN   | \033[32m
       YELLOW  | \033[33m
       BLUE    | \033[34m
       MAGENTA | \033[35m
       CYAN    | \033[36m */

    static constexpr const char* wm_msg = "[ banana ]";
    static constexpr const char* wm_wrn = "[ banana WARN ]";
    static constexpr const char* wm_err = "[ banana ERROR ]";

    static constexpr const char* wm_msg_color = "[ \033[33mbanana\033[0m ]";
    static constexpr const char* wm_wrn_color = "[ \033[33mbanana\033[36m WARN\033[0m  ]";
    static constexpr const char* wm_err_color = "[ \033[33mbanana\033[31m ERROR\033[0m ]";
    static constexpr const char* wm_dbg_color = "[ \033[33mbanana\033[32m DEBUG\033[0m ]";

    template <typename... t_args>
    void log(LOG_TYPE type, std::format_string<t_args...> fmt, t_args&&... args) {
#ifndef DEBUG
        if (type == LOG_TYPE::dbg)
            return;
#endif

        if (!this->file)
            this->open();

        std::string watermark;
        std::string watermark_color;
        std::string formatted = std::format(fmt, std::forward<t_args>(args)...);
        
        bool should_flush = false;

        switch(type) {
            case LOG_TYPE::msg:
                watermark = wm_msg; watermark_color = wm_msg_color; break;
            case LOG_TYPE::wrn:
                watermark = wm_wrn; watermark_color = wm_wrn_color; should_flush = true; break;
            case LOG_TYPE::err:
                watermark = wm_err; watermark_color = wm_err_color; should_flush = true; break;
            case LOG_TYPE::dbg:
                watermark_color = wm_dbg_color; break;
        }

#ifndef NDEBUG
        printf("%s %s\n", watermark_color.c_str(), formatted.c_str());
#endif

        if (this->file && type != LOG_TYPE::dbg) {
            fprintf(this->file, "%s | %s %s\n",
                FMT("{}", std::chrono::system_clock::now()).c_str(),
                watermark.c_str(),
                formatted.c_str());

            if (should_flush)
                this->flush();
        }
    }

public:
    void open();
    void close();

    template <typename... t_args>
    void msg(std::format_string<t_args...> fmt, t_args&&... args) {
        this->log(LOG_TYPE::msg, fmt, std::forward<t_args>(args)...);
    }
    
    template <typename... t_args>
    void wrn(std::format_string<t_args...> fmt, t_args&&... args) {
        this->log(LOG_TYPE::wrn, fmt, std::forward<t_args>(args)...);
    }

    template <typename... t_args>
    void err(std::format_string<t_args...> fmt, t_args&&... args) {
        this->log(LOG_TYPE::err, fmt, std::forward<t_args>(args)...);
    }

    template <typename... t_args>
    void dbg(std::format_string<t_args...> fmt, t_args&&... args) {
        this->log(LOG_TYPE::dbg, fmt, std::forward<t_args>(args)...);
    }

    void msg(std::string_view str);
    void wrn(std::string_view str);
    void err(std::string_view str);
    void dbg(std::string_view str);

    inline void flush() {
        fflush(this->file);
    }
};