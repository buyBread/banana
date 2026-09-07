#include <array>
#include <cstring>

#include "banana/hooks/base.hh"
#include "util/types.hh"

struct archive_asset_t {
    u32 asset_hash;
    u32 asset_type;
    u32 asset_offset;
    u32 asset_size;
    u32 asset_size_2;
    u8* raw_data; // populated by sub_73A130
};

struct archive_directory_t {
    u32               field_00;
    u32               field_04;
    u32               asset_count;
    archive_asset_t** assets;
};

constexpr u32 game_apk_hash = 0x003A261A;
constexpr u32 merged_apk_type = 25;

// scuffed port (bored)
void patch_decal_shader(u8* shader) {
    std::array<u8, 0x1E70> patched {};

    std::memcpy(patched.data(),
                shader,
                0x1E70);

    auto write_u32 = [&](size_t offset, u32 value) {
        std::memcpy(patched.data() + offset,
                    &value,
                    sizeof(value));
    };

    write_u32(0x0004, 0x01F0FFFE);

    std::memset(patched.data() + 0x07B7,
                0,
                0x07C8 - 0x07B7);

    // steal bytes from compiler metadata for the two extra shader instructions
    std::memcpy(patched.data() + 0x07B7,
                "sekrit",
                sizeof("sekrit") - 1);

    std::memmove(patched.data() + 0x07C8,
                 patched.data() + 0x07E8,
                 0x1E70 - 0x07E8);

    write_u32(0x1000, 0xA1000040);
    write_u32(0x1010, 0xA1550040);

    constexpr std::array<u32, 4>
    add_r6 { 0x03000002,
             0x80080006,
             0x81FF0006,
             0xA0AA0042, };

    std::memmove(patched.data() + 0x19DC + sizeof(add_r6),
                 patched.data() + 0x19DC,
                 0x1E50 - 0x19DC);

    std::memcpy(patched.data() + 0x19DC,
                add_r6.data(),
                sizeof(add_r6));


    constexpr std::array<u32, 4>
    add_r0 { 0x03000002,
             0x80080000,
             0x81FF0000,
             0xA0AA0042, };

    std::memmove(patched.data() + 0x1B68 + sizeof(add_r0),
                 patched.data() + 0x1B68,
                 0x1E60 - 0x1B68);

    std::memcpy(patched.data() + 0x1B68,
                add_r0.data(),
                sizeof(add_r0));

    std::memcpy(shader, patched.data(), patched.size());
}

DEFINE_HOOK(hk_sub_73A130, void*(__fastcall*)(archive_directory_t*, void*, void*)) {

public:
    CONSTRUCT_HOOK(hk_sub_73A130, (void*)0x0073A130, "debug") {}

    static void* __fastcall detour(archive_directory_t* archive, void* edx, void* pack_slot) {
        void* result = m_original(archive, edx, pack_slot); // populate archive_asset_t.raw_data

        if (!archive || !archive->assets)
            return result;

        for (u32 i = 0; i < archive->asset_count; ++i) {
            archive_asset_t* asset = archive->assets[i];

            if (!asset)
                continue;

            if (asset->asset_hash != game_apk_hash || asset->asset_type != merged_apk_type)
                continue;

            auto* decal_basic = asset->raw_data + 0x002007A0;

            decal_basic[0x0C9C] = 0x4C;

            patch_decal_shader(decal_basic + 0x191C);
            patch_decal_shader(decal_basic + 0x3F1C);
            patch_decal_shader(decal_basic + 0x651C);
            patch_decal_shader(decal_basic + 0x8B1C);

            break;
        }

        return result;
    }
};

CREATE_HOOK(hk_sub_73A130);