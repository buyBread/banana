#pragma once

#include <cstddef>

#include "game/als/als.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mm/mm.hh"
#include "game/boolx.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class ai_core;

    // a piece of work the action processor inode runs on its core until it says it's done
    class ai_action_nugget {

    public:
        enum e_action_exit_status : u32 {
            no_exit,
            should_exit
        };

        enum e_operation_mode : u32 {
            mode_running,
            mode_blocked
        };

        struct ai_action_info {
            string_hash      my_hash_id;
            ai_core*         my_core;
            e_operation_mode current_mode;
            boolx            active;
            boolx            should_delete; // the processor deletes it once it's done
            u8               pad_00e[0x02];
        };

        ai_action_info my_action_info;

        ai_action_nugget() {} // for derived classes whose retail constructor builds this part
        virtual ~ai_action_nugget();

        void* operator new(std::size_t size) noexcept {
            return mm::allocate(size);
        }

        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        ai_core* get_core() const { return my_action_info.my_core; }

        virtual e_action_exit_status frame_advance(f32 delta_t) = 0;

        // sub_48E890
        virtual void activate() {
            my_action_info.active = true;
        }

        // sub_48E8A0
        // inlined @ sub_492DF0
        virtual void deactivate(bool on_finalize) {
            my_action_info.active = false;
        }

        virtual void set_action_running();
        virtual void set_action_blocked();
    };

    // plays a category on its core's als
    class play_anim_ai_action : public ai_action_nugget {

    public:
        als::category_id category;
        als::layer_id    layer;
        boolx            exit_on_finish; // frame_advance waits for the category to finish
        u8               pad_01d[0x03];

        play_anim_ai_action(ai_core* owner, const string_hash &my_hash);

        e_action_exit_status frame_advance(f32 delta_t) override;
        void                 activate() override;
        void                 deactivate(bool on_finalize) override;

        virtual void setup(als::category_id _category, als::layer_id _layer, bool exit_on_anim_finish);
        virtual void start_anim();
        virtual bool should_succeed() const;
        virtual bool is_master_request() const;
        virtual bool ignore_same_cat_protection() const;
    };

    ASSERT_SIZEOF  (ai_action_nugget::ai_action_info,                0x10);
    ASSERT_OFFSETOF(ai_action_nugget::ai_action_info, my_core,       0x04);
    ASSERT_OFFSETOF(ai_action_nugget::ai_action_info, current_mode,  0x08);
    ASSERT_OFFSETOF(ai_action_nugget::ai_action_info, active,        0x0C);
    ASSERT_OFFSETOF(ai_action_nugget::ai_action_info, should_delete, 0x0D);

    ASSERT_SIZEOF  (ai_action_nugget,                 0x14);
    ASSERT_OFFSETOF(ai_action_nugget, my_action_info, 0x04);

    ASSERT_SIZEOF  (play_anim_ai_action,                 0x20);
    ASSERT_OFFSETOF(play_anim_ai_action, category,       0x14);
    ASSERT_OFFSETOF(play_anim_ai_action, layer,          0x18);
    ASSERT_OFFSETOF(play_anim_ai_action, exit_on_finish, 0x1C);
} // treyarch
