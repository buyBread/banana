#include "log.hh"

void s_logging::open() {
    auto filename = FMT("banana_log_{}.txt", std::time(nullptr));

    this->file = fopen(filename.c_str(), "a");
}

void s_logging::close() {
    fflush(this->file);
    fclose(this->file);
}

void s_logging::msg(std::string_view str) {
    this->log(LOG_TYPE::msg, "{}", str);
}

void s_logging::wrn(std::string_view str) {
    this->log(LOG_TYPE::wrn, "{}", str);
}

void s_logging::err(std::string_view str) {
    this->log(LOG_TYPE::err, "{}", str);
}

void s_logging::dbg(std::string_view str) {
    this->log(LOG_TYPE::dbg, "{}", str);
}