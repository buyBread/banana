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
