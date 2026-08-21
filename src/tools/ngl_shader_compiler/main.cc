#include <algorithm>
#include <array>
#include <atomic>
#include <cstddef>
#include <cstring>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <windows.h>

#include "treyarch/ngl/shaders/compiler.hh"
#include "treyarch/ngl/shaders/description.hh"
#include "treyarch/ngl/shaders/package.hh"
#include "util/types.hh"
#include "util/appdata_path.hh"

using namespace treyarch;
using namespace ngl::shaders;

static constexpr size_t compiler_worker_limit = 8;
static constexpr u32    cache_magic           = 0x434C474E;
static constexpr u16    cache_format_version  = 1;

#pragma pack(push, 1)
struct cache_header {
    u32 magic;
    u16 format_version;
    u16 header_size;
    u32 key;
    u64 input_hash;
    u32 bytecode_size;
};
#pragma pack(pop)

struct shader_task {
    shader_description    description;
    std::string           compiler_path;
    std::vector<u8>       source;
    u64                   input_hash;
    shader_compile_result compilation;
    bool                  cached;
};

static bool read_file(const std::filesystem::path &path,
                            std::vector<u8>       &output) {

    std::ifstream stream(path, std::ios::binary | std::ios::ate);

    if (!stream)
        return false;

    std::streamoff size = stream.tellg();

    if (size <= 0)
        return false;

    output.resize((size_t)size);
    stream.seekg(0, std::ios::beg);
    stream.read((char*)output.data(), size);
    
    return (bool)stream;
}

static bool write_file(const std::filesystem::path &path,
                       const std::vector<u8>       &data) {

    std::ofstream stream(path, std::ios::binary | std::ios::trunc);

    if (!stream)
        return false;

    stream.write((const char*)data.data(), data.size());

    return (bool)stream;
}

static std::filesystem::path get_cache_directory() {
    return util::appdata_path() / "shader_cache";
}

static std::filesystem::path get_cache_path(const std::filesystem::path &directory,
                                                  u32                    key,
                                                  u64                    input_hash) {

    return directory / std::format("{:08X}.{:016X}.cso", key, input_hash);
}

static bool load_cached_bytecode(const std::filesystem::path &path,
                                       u32                    key,
                                       u64                    input_hash,
                                       std::vector<u8>       &bytecode) {

    std::vector<u8> data;

    if (!read_file(path, data) || data.size() < sizeof(cache_header))
        return false;

    const cache_header* header = (const cache_header*)data.data();

    if (header->magic          != cache_magic          ||
        header->format_version != cache_format_version ||
        header->header_size    != sizeof(cache_header) ||
        header->key            != key                  ||
        header->input_hash     != input_hash           ||
        header->bytecode_size   % sizeof(u32)          ||
        header->bytecode_size  != data.size() - sizeof(cache_header))
        return false;

    bytecode.assign(data.begin() + sizeof(cache_header), data.end());
    
    return true;
}

static bool store_cached_bytecode(const std::filesystem::path &path,
                                        u32                    key,
                                        u64                    input_hash,
                                  const std::vector<u8>       &bytecode) {

    cache_header header {
        cache_magic,
        cache_format_version,
        sizeof(cache_header),
        key,
        input_hash,
        (u32)bytecode.size(),
    };

    std::vector<u8> data(sizeof(header) + bytecode.size());
    std::memcpy(data.data(), &header, sizeof(header));
    std::memcpy(data.data() + sizeof(header), bytecode.data(), bytecode.size());
    
    return write_file(path, data);
}

static bool describe_tasks(const std::filesystem::path    &source_directory,
                           const std::filesystem::path    &cache_directory,
                                 std::vector<shader_task> &tasks,
                                 size_t                   &cached_count) {

    for (u16 value = 0; value < (u16)e_shader_program::count; ++value) {
        e_shader_program program = (e_shader_program)value;

        size_t permutation_count = get_permutation_count(program);

        for (size_t permutation = 0; permutation < permutation_count; ++permutation) {
            shader_task task {};

            if (!describe_shader({ program, (u16)permutation }, task.description))
                return false;

            std::filesystem::path source_path = std::filesystem::absolute
                (source_directory / task.description.source_path).lexically_normal();

            task.compiler_path = source_path.string();

            if (!read_file(source_path, task.source)) {
                std::cerr << "failed to read shader source \"" << task.compiler_path << "\"\n";

                return false;
            }

            task.input_hash = package::hash_input(task.description,
                                                  task.source.data(),
                                                  task.source.size());

            u32 key = encode_shader_key(task.description.key);

            task.cached = load_cached_bytecode(get_cache_path(cache_directory,
                                                              key,
                                                              task.input_hash),
                                               key,
                                               task.input_hash,
                                               task.compilation.bytecode);

            if (task.cached)
                ++cached_count;

            tasks.push_back(std::move(task));
        }
    }

    return true;
}

static bool compile_tasks(      std::vector<shader_task> &tasks,
                          const std::filesystem::path    &cache_directory,
                                size_t                    compile_count) {

    if (compile_count) {
        size_t worker_count = std::min(compiler_worker_limit, (size_t)std::max(1u, std::thread::hardware_concurrency()));
        
        std::atomic<size_t> next_task {};
        std::vector<std::thread> workers;

        std::cout << "compiling " << compile_count << " shader variants with " << worker_count << " workers\n";

        workers.reserve(worker_count);

        for (size_t worker = 0; worker < worker_count; ++worker) {
            workers.emplace_back([&]() {
                for (;;) {
                    size_t index = next_task.fetch_add(1, std::memory_order_relaxed);

                    if (index >= tasks.size())
                        return;

                    shader_task &task = tasks[index];

                    if (task.cached)
                        continue;

                    compile_shader(task.description,
                                   task.source.data(),
                                   task.source.size(),
                                   task.compiler_path.c_str(),
                                   task.compilation);
                }
            });
        }

        for (std::thread &worker : workers)
            worker.join();
    }

    bool succeeded = true;

    for (const shader_task &task : tasks) {
        if (task.compilation.bytecode.empty()) {
            const char* message = task.compilation.diagnostics.empty() ?
                "no compiler diagnostics" : task.compilation.diagnostics.c_str();
            
            std::cerr << "failed to compile shader \"" << task.compiler_path << "\""
                      << "(0x" << std::hex << task.compilation.result << std::dec << "): "
                      << message << "\n";
            
            succeeded = false;
            
            continue;
        }

        if (task.cached)
            continue;

        u32 key = encode_shader_key(task.description.key);

        if (!store_cached_bytecode(get_cache_path(cache_directory,
                                                  key,
                                                  task.input_hash),
                                   key,
                                   task.input_hash,
                                   task.compilation.bytecode)) {

            std::cerr << "failed to cache shader \"" << task.description.source_name << "\"\n";
            
            succeeded = false;
        }
    }

    return succeeded;
}

static std::vector<u8> create_package(const std::vector<shader_task> &tasks) {
    size_t bytecode_offset = sizeof(package::header) + tasks.size() * sizeof(package::entry);
           bytecode_offset = (bytecode_offset + 3) & ~size_t(3);
    
    size_t package_size = bytecode_offset;

    for (const shader_task &task : tasks) {
        package_size += task.compilation.bytecode.size();
        package_size = (package_size + 3) & ~size_t(3);
    }

    std::vector<u8> data(package_size);

    package::header header {
        package::magic,
        package::format_version,
        sizeof(package::header),
        (u32)package_size,
        (u32)tasks.size(),
        sizeof(package::header),
        (u32)bytecode_offset,
    };

    std::memcpy(data.data(), &header, sizeof(header));

    size_t cursor = bytecode_offset;

    for (size_t index = 0; index < tasks.size(); ++index) {
        const shader_task &task = tasks[index];

        package::entry entry {
            encode_shader_key(task.description.key),
            task.input_hash,
            (u32)cursor,
            (u32)task.compilation.bytecode.size(),
        };

        std::memcpy( data.data() + sizeof(package::header) + index * sizeof(package::entry),
                    &entry,
                     sizeof(entry));
        std::memcpy(data.data() + cursor,
                    task.compilation.bytecode.data(),
                    task.compilation.bytecode.size());

        cursor += task.compilation.bytecode.size();
        cursor = (cursor + 3) & ~size_t(3);
    }

    return data;
}

static i32 run(const std::filesystem::path &source_directory,
               const std::filesystem::path &output_path,
               const std::filesystem::path &resource_path) {

    std::filesystem::path cache_directory = get_cache_directory();

    if (cache_directory.empty()) {
        std::cerr << "LOCALAPPDATA is unavailable; cannot locate the shader cache\n";
        
        return 1;
    }

    std::error_code error;
    std::filesystem::create_directories(cache_directory, error);

    if (error) {
        std::cerr << "failed to create shader cache \"" << cache_directory.string() << "\"\n";
        
        return 1;
    }

    std::vector<shader_task> tasks;
    size_t cached_count = 0;

    if (!describe_tasks(source_directory, cache_directory, tasks, cached_count))
        return 1;

    size_t compile_count = tasks.size() - cached_count;

    if (!compile_tasks(tasks, cache_directory, compile_count))
        return 1;

    std::vector<u8> package_data = create_package(tasks);
    std::vector<u8> previous_package;
    
    read_file(output_path, previous_package);

    if (previous_package == package_data) {
        std::cout << "shader package is current "
                  << "(" << tasks.size() << " variants, " << cached_count << " cached)\n";
        
        return 0;
    }

    std::filesystem::create_directories(output_path.parent_path(), error);

    if (error || !write_file(output_path, package_data)) {
        std::cerr << "failed to write shader package \"" << output_path.string() << "\"\n";
    
        return 1;
    }

    if (!resource_path.empty()) {
        std::filesystem::last_write_time(resource_path,
                                         std::filesystem::file_time_type::clock::now(),
                                         error);

        if (error) {
            std::cerr << "failed to invalidate shader resource \"" << resource_path.string() << "\"\n";
            
            return 1;
        }
    }

    std::cout << "packed " << tasks.size() << " shader variants "
              << "(" << compile_count << " compiled, " << cached_count << " cached)\n";

    return 0;
}

int main(int argc, char** argv) {
    if (argc != 3 && argc != 4) {
        std::cerr << "usage: ngl_shader_compiler <source_directory> <output_path> [resource_path]\n";
        
        return 1;
    }

    return run(
        argv[1],
        argv[2],
        argc == 4 ? argv[3] : std::filesystem::path {});
}
