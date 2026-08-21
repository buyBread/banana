name    = "banana"
version = "0.0.1"

-- PROJECT

set_project     (name)
set_version     (version)
set_defaultarchs("x86")
set_languages   ("c17", "c++23")
set_warnings    ("all", "extra")
set_plat        ("windows")
set_toolchains  ("msvc")

-- DEPENDENCIES

add_requires("minhook", { arch = "x86" } )
add_requires("imgui", { config = {
        win32 = true,
        dx9   = true }
    })

set_policy("generator.compile_commands", true)
add_rules("plugin.compile_commands.autoupdate")

-- MODES

if is_mode("debug") then
    add_cxflags("/wd4100")
    add_cxflags("/wd4005")

    add_defines("DEBUG")

    set_optimize("none")
    set_symbols ("debug")
end

if is_mode("devel") then
    add_cxflags("/wd4100")
    add_cxflags("/wd4005")

    set_optimize("fast")
    set_symbols ("debug")
end

if is_mode("release") then
    add_defines ("NDEBUG")

    set_strip   ("all")
    set_symbols ("hidden")
    set_optimize("fastest")
    set_policy  ("build.optimization.lto", true)
end

-- NGL SHADER COMPILER

rule("ngl.shader.package")
    before_build(function(target)
        local compiler = target:dep("ngl_shader_compiler"):targetfile()

        local source   = path.join(os.projectdir(), "src", "treyarch", "ngl", "shaders")
        local output   = path.join(os.projectdir(), ".cache", "shaders", "ngl.pack")
        local resource = path.join(source, "program", "resources.rc")

        os.vrunv(compiler, { source, output, resource })
    end)

target("ngl_shader_compiler")
    set_default(false)

    set_kind  ("binary")
    set_policy("build.fence", true)

    add_defines("WIN32_LEAN_AND_MEAN")
    add_defines("NOMINMAX") -- famous war criminal, bill gates

    add_files      ("src/tools/ngl_shader_compiler/main.cc")
    add_files      ("src/tools/ngl_shader_compiler/compiler.cc")
    add_files      ("src/treyarch/ngl/shaders/description.cc")
    add_includedirs("src")
    add_links      ("d3dcompiler")

-- TARGET

target(name)
    set_default(true)

    set_kind    ("shared")
    set_basename("d3d9")

    add_deps("ngl_shader_compiler")
    add_rules("ngl.shader.package")

    add_packages("minhook")
    add_packages("imgui")

    add_defines("WIN32_LEAN_AND_MEAN")
    add_defines("_CRT_SECURE_NO_WARNINGS")
    add_defines("NOMINMAX") -- famous war criminal, bill gates

    on_load(function (target)
        local git_hash = "unknown"
        
        try { function ()
            git_hash = os.iorun("git rev-parse --short HEAD"):trim()
        end }

        -- add_defines("GIT_COMMIT_HASH=\""..git_hash.."\"")
        target:add("defines", "GIT_COMMIT_HASH=\""..git_hash.."\"")
    end)

    add_files      ("src/**.cc")
    remove_files   ("src/tools/**.cc")
    add_files      ("src/treyarch/ngl/shaders/resources.rc")
    add_includedirs("src")
    add_syslinks    ("winmm")
