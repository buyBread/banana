set_xmakever("3.0.9")

-- project

set_project("banana")
set_version("0.0.1")

set_plat        ("windows")
set_toolchains  ("msvc")
set_defaultarchs("x86")
set_allowedarchs("x86")
set_defaultmode ("release")
set_allowedmodes("debug", "devel", "release")
set_languages   ("c17", "c++23")
set_warnings    ("all", "extra")

-- options

option("game")
    set_default    (true)
    set_showmenu   (true)
    set_description("build banana")
option_end()

option("tests")
    set_default    (false)
    set_showmenu   (true)
    set_description("build unit tests")
option_end()

set_policy("generator.compile_commands", true)
add_rules("plugin.compile_commands.autoupdate")

add_defines("WIN32_LEAN_AND_MEAN",
            "NOMINMAX")

-- build modes

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

-- banana

if has_config("game") then
    add_requires("minhook",
                 { arch = "x86" })

    add_requires("imgui",
                 { configs = { win32 = true,
                               dx9   = true } })

    rule("ngl.shader.package")
        before_build(function (target)
            local compiler = target:dep("ngl_shader_compiler"):targetfile()

            local source   = path.join(os.projectdir(), "src", "treyarch", "ngl", "shaders")
            local output   = path.join(os.projectdir(), ".cache", "shaders", "ngl.pack")
            local resource = path.join(source, "resources.rc")

            os.vrunv(compiler, {source, output, resource})
        end)
    rule_end()

    target("ngl_shader_compiler")
        set_default(false)

        set_group ("tools")
        set_kind  ("binary")
        set_policy("build.fence", true)

        add_files("src/tools/ngl_shader_compiler/main.cc")
        add_files("src/tools/ngl_shader_compiler/compiler.cc")
        add_files("src/treyarch/ngl/shaders/description.cc")

        add_includedirs("src")
        
        add_links("d3dcompiler")
    target_end()

    target("banana")
        set_default(true)

        set_group   ("game")
        set_kind    ("shared")
        set_basename("d3d9")

        add_deps    ("ngl_shader_compiler")
        add_rules   ("ngl.shader.package")
        add_packages("minhook", "imgui")

        add_defines("_CRT_SECURE_NO_WARNINGS")

        on_load(function (target)
            local git_hash = os.getenv("BANANA_GIT_COMMIT") or os.getenv("GITHUB_SHA")

            if not git_hash or #git_hash == 0 then
                git_hash = "unknown"

                try { function ()
                    git_hash = os.iorun("git rev-parse --short HEAD"):trim()
                end }
            end

            target:add("defines", "GIT_COMMIT_HASH=\"" .. git_hash:sub(1, 7) .. "\"")
            target:add("defines", "BANANA_VERSION=\"" .. target:version() .. "\"")
        end)

        add_files   ("src/**.cc")
        add_files   ("src/treyarch/ngl/shaders/resources.rc")
        remove_files("src/tools/**.cc")

        add_includedirs("src")

        add_syslinks("winmm")
    target_end()
end

-- unit tests

if has_config("tests") then
    add_requires("doctest 2.5.2",
                 { configs = { cmake = false } })

    target("tests")
        set_default(false)

        set_group  ("tests")
        set_kind   ("binary")
        set_rundir("$(projectdir)")

        add_packages("doctest")

        add_files("tests/**.cc")

        add_files("src/treyarch/shared/mash/mash_info.cc")
        add_files("src/treyarch/shared/mash/string.cc")
        add_files("src/treyarch/shared/mash/virtual_base.cc")
        add_files("src/treyarch/amalga/resource_directory.cc")
        
        add_includedirs("src", "tests")
        add_defines("_CRT_SECURE_NO_WARNINGS")

        add_tests("all")
        on_test(function (target)
            return os.execv("python", {
                path.join(os.projectdir(), "scripts", "run_tests.py"),
                path.absolute(target:targetfile())
            }, { try = true }) == 0
        end)
    target_end()
end
