#include <new>

#include "bink/bink.hh"
#include "treyarch/game/movie_manager.hh"
#include "treyarch/ngl/texture/runtime.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

// sub_6930C0; `state` is left as allocated
movie_manager::movie_manager() {
    vtable  = &references::movie_manager_vtable.get();
    unk_008 = 2;
    bink    = nullptr;
}

// sub_429150
void movie_manager::create_inst() {
    void* allocation = memory::heap::allocate(sizeof(movie_manager));

    movie_manager::instance() = allocation ? new (allocation) movie_manager() : nullptr;
}

// loc_6930E0 (a chunk sub_97B160 tail-jumps into)
void movie_manager::release_device_resources() {
    if (bink)
        release_textures();
}

// sub_6ABC30
bool movie_manager::restore_device_resources() {
    if (!bink)
        return true;

    BinkGetFrameBuffersInfo(bink, &frame_buffers);

    if (create_textures(&frame_buffers)) {
        BinkRegisterFrameBuffers(bink, &frame_buffers);

        u32 frame = bink->FrameNum;

        BinkGoto(bink, 1, 0);
        BinkGoto(bink, frame, 0);

        return true;
    }

    if (bink) {
        BinkClose(bink);
        bink = nullptr;
    }

    release_textures();

    return false;
}

// sub_692D00
bool movie_manager::create_textures(BINKFRAMEBUFFERS* frame_buffers) {
    movie_textures &textures = references::movie_textures.get();

    references::frame_maximum_v.write(1.0f);

    // frame 0 decides every plane's width
    u32 luma_width   = frame_buffers->Frames[0].YPlane.Allocate  ? frame_buffers->YABufferWidth   : 0;
    u32 alpha_width  = frame_buffers->Frames[0].APlane.Allocate  ? frame_buffers->YABufferWidth   : 0;
    u32 chroma_width = frame_buffers->Frames[0].cRPlane.Allocate ? frame_buffers->cRcBBufferWidth : 0;

    for (u32 index = 0; index < (u32)frame_buffers->TotalFrames; ++index) {
        BINKFRAMEPLANESET &frame = frame_buffers->Frames[index];

        textures.frame_decoded[index] = 0;

        struct plane_textures {
            BINKPLANE*     plane;
            u32            width;
            u32            height;
            ngl::texture** frame_texture;
            ngl::texture** upload_texture;
        };

        const plane_textures planes[] { { &frame.YPlane,  luma_width,   frame_buffers->YABufferHeight,   &textures.frame_y[index],  &textures.upload_y  },
                                        { &frame.APlane,  alpha_width,  frame_buffers->YABufferHeight,   &textures.frame_a[index],  &textures.upload_a  },
                                        { &frame.cRPlane, chroma_width, frame_buffers->cRcBBufferHeight, &textures.frame_cr[index], &textures.upload_cr },
                                        { &frame.cBPlane, chroma_width, frame_buffers->cRcBBufferHeight, &textures.frame_cb[index], &textures.upload_cb } };

        for (const plane_textures &entry : planes) {
            if (!entry.plane->Allocate)
                continue;

            ngl::texture* value = ngl::create_runtime_texture(0x00100000, D3DFMT_A8, entry.width, entry.height, 1, 1);
            *entry.frame_texture = value;

            // bink decodes straight into the texture memory it was handed while locked
            entry.plane->Buffer = ngl::d3d9::lock_texture_resource(&value->gpu_texture, 0, 0, &entry.plane->BufferPitch);

            switch (value->gpu_texture.resource_type) {
                case D3DRTYPE_TEXTURE:
                    ((IDirect3DTexture9*)value->gpu_texture.resource)->UnlockRect(0);

                    break;
                case D3DRTYPE_CUBETEXTURE:
                    ((IDirect3DCubeTexture9*)value->gpu_texture.resource)->UnlockRect(D3DCUBEMAP_FACE_POSITIVE_X, 0);

                    break;
                case D3DRTYPE_VOLUMETEXTURE:
                    ((IDirect3DVolumeTexture9*)value->gpu_texture.resource)->UnlockBox(0);

                    break;
            }

            if (!index)
                *entry.upload_texture = ngl::create_runtime_texture(0x00001000, D3DFMT_A8, entry.width, entry.height, 1, 1);
        }
    }

    return true;
}

// sub_693000
void movie_manager::release_textures() {
    movie_textures &textures = references::movie_textures.get();

    ngl::release_texture(textures.upload_y);
    textures.upload_y = nullptr;
    ngl::release_texture(textures.upload_a);
    textures.upload_a = nullptr;
    ngl::release_texture(textures.upload_cr);
    textures.upload_cr = nullptr;
    ngl::release_texture(textures.upload_cb);
    textures.upload_cb = nullptr;

    for (u32 index = 0; index < 2; ++index) {
        textures.frame_decoded[index] = 0;

        ngl::release_texture(textures.frame_y[index]);
        ngl::release_texture(textures.frame_a[index]);
        ngl::release_texture(textures.frame_cr[index]);
        ngl::release_texture(textures.frame_cb[index]);

        textures.frame_y[index]  = nullptr;
        textures.frame_a[index]  = nullptr;
        textures.frame_cr[index] = nullptr;
        textures.frame_cb[index] = nullptr;
    }
}
