#pragma once

#include <ctime>

#include "vc8/vector.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the soap save/load machine, one step per frame (sub_79CAF0)
    enum e_game_data_state : i32 {
        game_data_state_sign_in,
        game_data_state_wait_for_sign_in,
        game_data_state_sign_in_warning,
        game_data_state_select_device,
        game_data_state_device_unavailable,
        game_data_state_wait_for_device,
        game_data_state_no_device,
        game_data_state_perform,
        game_data_state_idle
    };

    enum e_game_data_request : i32 {
        game_data_request_plain,
        game_data_request_read_then_save,
        game_data_request_copy_slot,
        game_data_request_enumerate
    };

    // one per save slot, four of them
    struct game_data_slot_record {
        i32 slot;              // -1 for none
        u8  reserved_04[0x38];
        tm  save_time;
        i32 real_world_timer;
        u32 game_var_size;
        i32 valid;
    };

    // what a save carries besides the slot record
    struct game_save_record {
        u32      unk_00;          // a hash export_game_settings looks up and then ignores
        vector3d hero_position;
        u8       reserved_10[0x30];
        f32      hero_hit_points; // 0 restores the hero to full
        u32      unk_44;          // combo interface +0x34
        u32      unk_48;          // combo interface +0x38
        u8       reserved_4c[0x20];
    };

    struct game_data_payload {
        u8                        reserved_000[0x04];
        vc8::vector<void*>        autosave_triggers;
        u8*                       buffer;
        u32                       buffer_size;
        u32                       buffer_capacity;
        game_data_slot_record*    slot_records;
        i32                       slot_status[4]; // 3 blocks saving into the slot
        game_save_record          save_record;
        u8                        autosave_pending;
        u8                        saving_or_loading;
        u8                        waiting_for_result;
        u8                        last_save_was_autosave;
        u8                        is_load_request;
        u8                        apply_loaded_data;
        u8                        reserved_0a6[0x02];
        i32                       requested_slot;
        i32                       source_slot;
        i32                       target_slot;
        e_game_data_request       request_kind;
        u8                        enumerate_finished;
        u8                        autosave_enabled;
        u8                        reserved_0ba[0x03];
        u8                        unk_0bd;
        u8                        unk_0be; // a pending pause or unpause, picked by unk_0bf
        u8                        unk_0bf;
        u8                        unk_0c0; // set when the frame step paused the game itself
        u8                        reserved_0c1[0x03];
        u32                       idle_frames;
        u8                        online_context_requested;
        u8                        online_context_set;
        u8                        reserved_0ca[0x06];
        e_game_data_state         state;
        u8                        blocked_by_notification;
        u8                        created_new_slot;
        u8                        request_outstanding;
        u8                        force_device_selection;
        u8                        force_autosave;
        u8                        reserved_0d9[0x03];
    };

    class game_data {

    public:
        void*              vtable; // a soap::observer
        game_data_payload* m;      // 𓂀 𓋹 𓆣 𓁹 𓃠

        void read_notifications();
        void frame_advance();

        bool load_remainder(u8* keep_waiting); // consumes a finished enumerate or read
        void soft_load();
        void export_game_settings();           // puts the loaded hero back
    };

    void frame_advance_soap(game_data* data);

    ASSERT_SIZEOF  (game_data_slot_record,                  0x6C);
    ASSERT_OFFSETOF(game_data_slot_record, save_time,       0x3C);
    ASSERT_OFFSETOF(game_data_slot_record, valid,           0x68);

    ASSERT_SIZEOF  (game_save_record,                  0x6C);
    ASSERT_OFFSETOF(game_save_record, hero_position,   0x04);
    ASSERT_OFFSETOF(game_save_record, hero_hit_points, 0x40);
    ASSERT_OFFSETOF(game_save_record, unk_44,          0x44);

    ASSERT_SIZEOF  (game_data_payload,                           0xDC);
    ASSERT_OFFSETOF(game_data_payload, autosave_triggers,        0x04);
    ASSERT_OFFSETOF(game_data_payload, buffer,                   0x14);
    ASSERT_OFFSETOF(game_data_payload, slot_records,             0x20);
    ASSERT_OFFSETOF(game_data_payload, slot_status,              0x24);
    ASSERT_OFFSETOF(game_data_payload, save_record,              0x34);
    ASSERT_OFFSETOF(game_data_payload, autosave_pending,         0xA0);
    ASSERT_OFFSETOF(game_data_payload, saving_or_loading,        0xA1);
    ASSERT_OFFSETOF(game_data_payload, apply_loaded_data,        0xA5);
    ASSERT_OFFSETOF(game_data_payload, requested_slot,           0xA8);
    ASSERT_OFFSETOF(game_data_payload, request_kind,             0xB4);
    ASSERT_OFFSETOF(game_data_payload, enumerate_finished,       0xB8);
    ASSERT_OFFSETOF(game_data_payload, autosave_enabled,         0xB9);
    ASSERT_OFFSETOF(game_data_payload, unk_0bd,                  0xBD);
    ASSERT_OFFSETOF(game_data_payload, unk_0c0,                  0xC0);
    ASSERT_OFFSETOF(game_data_payload, idle_frames,              0xC4);
    ASSERT_OFFSETOF(game_data_payload, online_context_requested, 0xC8);
    ASSERT_OFFSETOF(game_data_payload, state,                    0xD0);
    ASSERT_OFFSETOF(game_data_payload, blocked_by_notification,  0xD4);
    ASSERT_OFFSETOF(game_data_payload, force_autosave,           0xD8);

    ASSERT_SIZEOF  (game_data,    0x08);
    ASSERT_OFFSETOF(game_data, m, 0x04);
} // treyarch
