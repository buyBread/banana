#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"

using namespace treyarch;

void ngl::d3d9::set_render_state(D3DRENDERSTATETYPE state, DWORD value) {
    DWORD* states = &references::render_states.get();

    if (states[state] == value)
        return;

    states[state] = value;
    
    references::device.get()->SetRenderState(state, value);
}

void ngl::d3d9::set_sampler_state(u32                 stage,
                                  D3DSAMPLERSTATETYPE state,
                                  DWORD               value) {

    DWORD* states = (DWORD*)&references::sampler_states.get();

    DWORD &cached = states[stage * 16 + state - 1];

    if (cached == value)
        return;

    cached = value;
    
    references::device.get()->SetSamplerState(stage, state, value);
}

void ngl::d3d9::initialize_sampler_filters() {
    sampler_state_cache* states = &references::sampler_states.get();
    IDirect3DDevice9*    device =  references::device.get();

    for (u32 stage = 0; stage < 16; ++stage) {
        sampler_state_cache &state = states[stage];

        (void)state;
        (void)device;

        set_sampler_state(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        set_sampler_state(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        set_sampler_state(stage, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
    }
}

void ngl::d3d9::apply_blend_mode(u64 value) {
    u32 mode        = (u32)value;
    u8  source      = (u8)(value >> 32);
    u8  destination = (u8)(mode >> 24);

    if (mode & 0x00800000) {
        set_render_state(D3DRS_ALPHABLENDENABLE, TRUE);
        set_render_state(D3DRS_BLENDOP, (mode >> 16) & 0x3F);
        set_render_state(D3DRS_SRCBLEND, source);
        set_render_state(D3DRS_DESTBLEND, destination);

        if (source == 14 || source == 15 || destination == 14 || destination == 15) {
            u32 factor = (mode & 0xFF) * 0x01010101;
            set_render_state(D3DRS_BLENDFACTOR, factor);
        }
    } else
        set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);

    if (mode & 0x00400000) {
        set_render_state(D3DRS_ALPHATESTENABLE, TRUE);
        set_render_state(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
        set_render_state(D3DRS_ALPHAREF, mode & 0xFF);
    } else
        set_render_state(D3DRS_ALPHATESTENABLE, FALSE);
}
