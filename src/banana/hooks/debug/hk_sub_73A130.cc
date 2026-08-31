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

class hk_sub_73A130 : public c_hook<hk_sub_73A130, void*(__fastcall*)(archive_directory_t* archive, void* edx, void* pack_slot)> {

public:
    hk_sub_73A130() : c_hook((void*)0x0073A130, "debug") {}

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

            // decal_basic.PCFX (0x2007A0) + desired byte (0x000C9C)
            asset->raw_data[0x0020143C] = 0x4c;

            break;
        }

        return result;
    }
};

hk_sub_73A130 hk {};