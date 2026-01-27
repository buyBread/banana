-- PROJECT

name = "banana"

set_project        (name)
set_version        ("0.0.1")
set_defaultarchs   ("x86")
set_languages      ("c17", "cxx23")
set_warnings       ("all", "extra")

-- MISC.

set_policy("generator.compile_commands", true)
add_rules ("plugin.compile_commands.autoupdate")

-- DEPENDENCIES

add_requires("minhook", { arch = "x86" } )
add_requires("imgui", { config = { win32 = true, dx9 = true }} )

-- MODES

if is_mode("debug") or is_mode("devel") then
    add_cxflags("/wd4100")
    add_cxflags("/wd4005")
    
    set_symbols("debug")
end

if is_mode("debug") then
    add_defines("DEBUG")

    set_optimize("none")
end

if is_mode("devel") then
    set_optimize("fast")
end

if is_mode("release") then
    add_defines ("NDEBUG")

    set_strip   ("all")
    set_symbols ("hidden")
    set_optimize("fastest")
    set_policy  ("build.optimization.lto", true)
end

-- TARGET

target(name)
    set_default(true)

    set_kind      ("shared")
    set_plat      ("windows")
    set_toolchains("msvc")
    set_basename  ("d3d9")

    add_packages("minhook")
    add_packages("imgui")

    add_defines("_CRT_SECURE_NO_WARNINGS")

    add_files      ("src/**.cc")
    add_includedirs("src")