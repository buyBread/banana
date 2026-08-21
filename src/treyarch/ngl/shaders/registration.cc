#include <cstddef>

#include "banana/logging.hh"
#include "treyarch/ngl/shaders/registration.hh"
#include "treyarch/ngl/shaders/fake_peds/programs.hh"
#include "treyarch/ngl/shaders/pcuv/programs.hh"
#include "treyarch/ngl/shaders/puv/programs.hh"
#include "treyarch/ngl/shaders/road_lights/programs.hh"
#include "treyarch/ngl/shaders/sm_babyphat/programs.hh"
#include "treyarch/ngl/shaders/sm_babyphatnormal/programs.hh"
#include "treyarch/ngl/shaders/sm_bright_filter/programs.hh"
#include "treyarch/ngl/shaders/sm_buildinglod/programs.hh"
#include "treyarch/ngl/shaders/sm_bush/programs.hh"
#include "treyarch/ngl/shaders/sm_citylod/programs.hh"
#include "treyarch/ngl/shaders/sm_decal/programs.hh"
#include "treyarch/ngl/shaders/sm_decalchar/programs.hh"
#include "treyarch/ngl/shaders/sm_depth_shadow/programs.hh"
#include "treyarch/ngl/shaders/sm_phat/programs.hh"
#include "treyarch/ngl/shaders/sm_phat_palette/programs.hh"
#include "treyarch/ngl/shaders/sm_phat_palette_normal/programs.hh"
#include "treyarch/ngl/shaders/sm_phatchar/programs.hh"
#include "treyarch/ngl/shaders/sm_phatcharnormal/programs.hh"
#include "treyarch/ngl/shaders/sm_phatnormal/programs.hh"
#include "treyarch/ngl/shaders/sm_phatpalettechar/programs.hh"
#include "treyarch/ngl/shaders/sm_phatpalettecharnormal/programs.hh"
#include "treyarch/ngl/shaders/sm_phatspiderman/programs.hh"
#include "treyarch/ngl/shaders/sm_retrofit/programs.hh"
#include "treyarch/ngl/shaders/sm_road/programs.hh"
#include "treyarch/ngl/shaders/sm_roadlod/programs.hh"
#include "treyarch/ngl/shaders/sm_simple/programs.hh"
#include "treyarch/ngl/shaders/sm_translucent/programs.hh"
#include "treyarch/ngl/shaders/smsky/programs.hh"

using namespace treyarch;

namespace registration = ngl::shaders::registration;
namespace refs = registration::references;

using program_initializer = bool(*)();

struct registration_override {
    ngl::init_list* item;
    
    void (*register_item)();
};

static void initialize_programs(const char* name, program_initializer initialize) {
    if (!initialize())
        banana::log.err("failed to initialize the \"{}\" shader programs", name);
}

static void register_shader(      ngl::shader        &value,
                                  ngl::e_shader_id    expected_id,
                            const char*               name,
                                  program_initializer initialize) {

    u32 shader_id = ngl::shaders::registration::references::next_shader_id.read();

    if (shader_id != (u32)expected_id)
        banana::log.err("unexpected \"{}\" shader id (expected {}, found {})",
            name,
            (u32)expected_id,
            shader_id);

    value.id = (ngl::e_shader_id)shader_id;
    ngl::shaders::registration::references::next_shader_id.write(shader_id + 1);
    ngl::references::shaders.get().insert(&value);

    if (initialize)
        initialize_programs(name, initialize);
}

static const registration_override* get_overrides(size_t &count) {
    static const registration_override overrides[] {
        { &refs::fx.get(), [] {
            register_shader(refs::fx.get(), ngl::e_shader_id::fx,
                "fx", nullptr);
            refs::fx_parameter_data.write(&refs::fx_parameter_data_storage.get());
        }},
        { &refs::road_lights.get(), [] {
            initialize_programs("road_lights", ngl::shaders::road_lights::initialize);
        }},
        { &refs::sm_bush.get(), [] {
            register_shader(refs::sm_bush.get(), ngl::e_shader_id::sm_bush,
                "sm_bush", ngl::shaders::sm_bush::initialize);
        }},
        { &refs::sm_citylod.get(), [] {
            register_shader(refs::sm_citylod.get(), ngl::e_shader_id::sm_citylod,
                "sm_citylod", ngl::shaders::sm_citylod::initialize);
        }},
        { &refs::sm_retrofit.get(), [] {
            register_shader(refs::sm_retrofit.get(), ngl::e_shader_id::sm_retrofit,
                "sm_retrofit", ngl::shaders::sm_retrofit::initialize);
        }},
        { &refs::sm_buildinglod.get(), [] {
            register_shader(refs::sm_buildinglod.get(), ngl::e_shader_id::sm_buildinglod,
                "sm_buildinglod", ngl::shaders::sm_buildinglod::initialize);
        }},
        { &refs::sm_roadlod.get(), [] {
            register_shader(refs::sm_roadlod.get(), ngl::e_shader_id::sm_roadlod,
                "sm_roadlod", ngl::shaders::sm_roadlod::initialize);
        }},
        { &refs::sm_decalchar.get(), [] {
            register_shader(refs::sm_decalchar.get(), ngl::e_shader_id::sm_decalchar,
                "sm_decalchar", ngl::shaders::sm_decalchar::initialize);
        }},
        { &refs::sm_babyphatnormal.get(), [] {
            register_shader(refs::sm_babyphatnormal.get(), ngl::e_shader_id::sm_babyphatnormal,
                "sm_babyphatnormal", ngl::shaders::sm_babyphatnormal::initialize);
        }},
        { &refs::sm_babyphat.get(), [] {
            register_shader(refs::sm_babyphat.get(), ngl::e_shader_id::sm_babyphat,
                "sm_babyphat", ngl::shaders::sm_babyphat::initialize);
        }},
        { &refs::sm_decal.get(), [] {
            register_shader(refs::sm_decal.get(), ngl::e_shader_id::sm_decal,
                "sm_decal", ngl::shaders::sm_decal::initialize);
        }},
        { &refs::sm_phat_palette_normal.get(), [] {
            register_shader(refs::sm_phat_palette_normal.get(),
                ngl::e_shader_id::sm_phat_palette_normal,
                "sm_phat_palette_normal", ngl::shaders::sm_phat_palette_normal::initialize);
        }},
        { &refs::sm_phatnormal.get(), [] {
            register_shader(refs::sm_phatnormal.get(), ngl::e_shader_id::sm_phatnormal,
                "sm_phatnormal", ngl::shaders::sm_phatnormal::initialize);
        }},
        { &refs::sm_road.get(), [] {
            register_shader(refs::sm_road.get(), ngl::e_shader_id::sm_road,
                "sm_road", ngl::shaders::sm_road::initialize);
        }},
        { &refs::sm_phatpalettecharnormal.get(), [] {
            register_shader(refs::sm_phatpalettecharnormal.get(),
                ngl::e_shader_id::sm_phatpalettecharnormal,
                "sm_phatpalettecharnormal",
                ngl::shaders::sm_phatpalettecharnormal::initialize);
        }},
        { &refs::sm_phatpalettechar.get(), [] {
            register_shader(refs::sm_phatpalettechar.get(),
                ngl::e_shader_id::sm_phatpalettechar,
                "sm_phatpalettechar", ngl::shaders::sm_phatpalettechar::initialize);
        }},
        { &refs::sm_depth_shadow.get(), [] {
            register_shader(refs::sm_depth_shadow.get(), ngl::e_shader_id::sm_depth_shadow,
                "sm_depth_shadow", ngl::shaders::sm_depth_shadow::initialize);
        }},
        { &refs::smsky.get(), [] {
            register_shader(refs::smsky.get(), ngl::e_shader_id::smsky,
                "smsky", ngl::shaders::smsky::initialize);
        }},
        { &refs::sm_bright_filter.get(), [] {
            register_shader(refs::sm_bright_filter.get(), ngl::e_shader_id::sm_bright_filter,
                "sm_bright_filter", ngl::shaders::sm_bright_filter::initialize);
        }},
        { &refs::sm_phat_palette.get(), [] {
            register_shader(refs::sm_phat_palette.get(), ngl::e_shader_id::sm_phat_palette,
                "sm_phat_palette", ngl::shaders::sm_phat_palette::initialize);
        }},
        { &refs::sm_phatcharnormal.get(), [] {
            register_shader(refs::sm_phatcharnormal.get(), ngl::e_shader_id::sm_phatcharnormal,
                "sm_phatcharnormal", ngl::shaders::sm_phatcharnormal::initialize);
        }},
        { &refs::sm_phatchar.get(), [] {
            register_shader(refs::sm_phatchar.get(), ngl::e_shader_id::sm_phatchar,
                "sm_phatchar", ngl::shaders::sm_phatchar::initialize);
        }},
        { &refs::sm_phatspiderman.get(), [] {
            register_shader(refs::sm_phatspiderman.get(), ngl::e_shader_id::sm_phatspiderman,
                "sm_phatspiderman", ngl::shaders::sm_phatspiderman::initialize);
        }},
        { &refs::sm_phat.get(), [] {
            register_shader(refs::sm_phat.get(), ngl::e_shader_id::sm_phat,
                "sm_phat", ngl::shaders::sm_phat::initialize);
        }},
        { &refs::puv.get(), [] {
            register_shader(refs::puv.get(), ngl::e_shader_id::puv,
                "PUV", ngl::shaders::puv::initialize);
        }},
        { &refs::fake_peds.get(), [] {
            initialize_programs("fake_peds", ngl::shaders::fake_peds::initialize);
        }},
        { &refs::sm_simple.get(), [] {
            register_shader(refs::sm_simple.get(), ngl::e_shader_id::sm_simple,
                "SMSimple", ngl::shaders::sm_simple::initialize);
        }},
        { &refs::sm_translucent.get(), [] {
            register_shader(refs::sm_translucent.get(), ngl::e_shader_id::sm_translucent,
                "SMTranslucent", ngl::shaders::sm_translucent::initialize);
        }},
        { &refs::pcuv.get(), [] {
            register_shader(refs::pcuv.get(), ngl::e_shader_id::pcuv,
                "PCUV", ngl::shaders::pcuv::initialize);
        }},
    };

    count = sizeof(overrides) / sizeof(overrides[0]);
    
    return overrides;
}

static bool register_override(ngl::init_list* item) {
    size_t count;
    
    const registration_override* overrides = get_overrides(count);

    for (size_t index = 0; index < count; ++index) {
        if (overrides[index].item != item)
            continue;

        overrides[index].register_item();
        
        return true;
    }

    return false;
}

bool ngl::shaders::registration::try_register(init_list* item) {
    return register_override(item);
}
