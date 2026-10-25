#pragma once

#include "treyarch/game/als/als.hh"
#include "treyarch/game/als/internal.hh"
#include "treyarch/game/als/mocomp.hh"
#include "treyarch/game/als/param.hh"
#include "treyarch/game/als/state_machine.hh"
#include "treyarch/shared/math/algebra/vector3d.hh"
#include "treyarch/shared/memory/heap.hh"
#include "treyarch/shared/resource_key.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"
#include "vc8/vector.hh"

namespace treyarch { namespace als {
    struct animation_logic_system_unk_a68 {
        u32 unk_00;
        u32 unk_04;
        u32 unk_08;
        u32 unk_0c;

        animation_logic_system_unk_a68() : unk_00(0),
                                           unk_04(0),
                                           unk_08(0) {}

        void* operator new(std::size_t size) noexcept {
            return memory::heap::allocate(size);
        }

        void operator delete(void* allocation) noexcept {
            memory::heap::free(allocation);
        }
    };

    // an actor's animation logic: its base machine, its layers and the mocomp moving it
    class animation_logic_system : public animation_logic_system_interface {

    public:
        bool                               force_updated; // set by force_update, cleared by the next frame
        u8                                 pad_005[0x03];
        internal_parameter_cache           the_cache;
        animation_logic_system_shared*     als_shared;
        vc8::vector<layer_state_machine*>  the_layers;
        base_state_machine                 the_base_sm;
        actor*                             the_actor;
        animation_controller*              the_controller;
        vector3d                           actor_position;
        i32                                mocomp_layer_index;
        motion_compensator*                curr_mocomp;
        motion_compensator*                the_mocomp_data; // 256 bytes the mocomps are built in
        animation_logic_system_unk_a68*    unk_a68;
        void*                              unk_a6c;
        bool                               is_suspended;
        bool                               needs_reinit_anim;
        bool                               unk_a72;
        bool                               unk_a73;
        bool                               unk_a74;
        bool                               unk_a75;
        u8                                 pad_a76[0x02];
        animation_logic_system_interface*  coordinator;
        i32                                coordinator_unk;
        vc8::vector
            <animation_logic_system_interface*> coordinated;

        explicit animation_logic_system(actor* _the_actor);
        ~animation_logic_system() override;

        void* operator new(std::size_t size) noexcept {
            return memory::heap::allocate(size);
        }

        void operator delete(void* allocation) noexcept {
            memory::heap::free(allocation);
        }

        state_machine_interface* get_als_layer(layer_id the_layer) override;
        bool                     has_als_layer(layer_id the_layer) const override;

        bool   did_transition_succeed() const override;
        bool   did_transition_succeed_from_request() const override;
        void   suspend_logic_system(bool suspend) override;
        bool   als_is_suspended() const override;
        void   create_instance_data(animation_logic_system_shared* shared_portion) override;
        void   delete_instance_data() override;
        void   reset_animation_player() override;
        void*  get_unk_a68() const override;
        bool   is_high_priority() const override;
        actor* get_actor() const override;
        void   cache_actor_position() override;

        void                              set_coordinator(animation_logic_system_interface* new_coordinator, i32 unk) override;
        bool                              has_coordinator() const override;
        animation_logic_system_interface* get_coordinator() const override;
        i32                               get_coordinator_unk() const override;
        bool                              is_coordinator() const override;
        vc8::vector
            <animation_logic_system_interface*>* get_coordinated() override;
        void                              add_coordinated(animation_logic_system_interface* the_als) override;
        void                              remove_coordinated(animation_logic_system_interface* the_als) override;

        void                  set_unk_a75() override;
        animation_controller* get_controller() const override;
        void                  frame_advance(f32 delta_t, bool force) override;

        virtual void unk_025(void* unk);
        virtual void change_mocomp(const state* curr_state, layer_id layer);
    };

    // what an entity's resource asks for; post-load setup turns it into the actor's als
    struct als_res_data {
        resource_key                      key;
        animation_logic_system_interface* the_als;

        void post_unmash_fixup(actor* owner, u32 unk);
    };

    ASSERT_SIZEOF  (animation_logic_system_unk_a68, 0x10);

    ASSERT_SIZEOF  (animation_logic_system,                     0xA90);
    ASSERT_OFFSETOF(animation_logic_system, force_updated,      0x004);
    ASSERT_OFFSETOF(animation_logic_system, the_cache,          0x008);
    ASSERT_OFFSETOF(animation_logic_system, als_shared,         0x274);
    ASSERT_OFFSETOF(animation_logic_system, the_layers,         0x278);
    ASSERT_OFFSETOF(animation_logic_system, the_base_sm,        0x288);
    ASSERT_OFFSETOF(animation_logic_system, the_actor,          0xA48);
    ASSERT_OFFSETOF(animation_logic_system, the_controller,     0xA4C);
    ASSERT_OFFSETOF(animation_logic_system, actor_position,     0xA50);
    ASSERT_OFFSETOF(animation_logic_system, mocomp_layer_index, 0xA5C);
    ASSERT_OFFSETOF(animation_logic_system, curr_mocomp,        0xA60);
    ASSERT_OFFSETOF(animation_logic_system, the_mocomp_data,    0xA64);
    ASSERT_OFFSETOF(animation_logic_system, unk_a68,            0xA68);
    ASSERT_OFFSETOF(animation_logic_system, unk_a6c,            0xA6C);
    ASSERT_OFFSETOF(animation_logic_system, is_suspended,       0xA70);
    ASSERT_OFFSETOF(animation_logic_system, needs_reinit_anim,  0xA71);
    ASSERT_OFFSETOF(animation_logic_system, unk_a72,            0xA72);
    ASSERT_OFFSETOF(animation_logic_system, unk_a75,            0xA75);
    ASSERT_OFFSETOF(animation_logic_system, coordinator,        0xA78);
    ASSERT_OFFSETOF(animation_logic_system, coordinator_unk,    0xA7C);
    ASSERT_OFFSETOF(animation_logic_system, coordinated,        0xA80);

    ASSERT_SIZEOF  (als_res_data,          0x0C);
    ASSERT_OFFSETOF(als_res_data, the_als, 0x08);
}} // treyarch::als
