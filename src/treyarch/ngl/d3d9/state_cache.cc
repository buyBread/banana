#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"

using namespace treyarch;

void ngl::d3d9::initialize_sampler_filters() {
    sampler_state_cache* states = &references::sampler_states.get();
    IDirect3DDevice9*    device =  references::device.get();

    for (u32 stage = 0; stage < 16; ++stage) {
        sampler_state_cache &state = states[stage];

        if (state.min_filter != D3DTEXF_LINEAR) {
            state.min_filter = D3DTEXF_LINEAR;
            device->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        }

        if (state.mag_filter != D3DTEXF_LINEAR) {
            state.mag_filter = D3DTEXF_LINEAR;
            device->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        }

        if (state.mip_filter != D3DTEXF_LINEAR) {
            state.mip_filter = D3DTEXF_LINEAR;
            device->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        }
    }
}
