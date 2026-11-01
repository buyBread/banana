#pragma once

#include "game/wds/entity/interface/player_interface.hh"
#include "game/arch_base_vhandle.hh"
#include "game/delta_variable.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace references {
        // development_option<bool> RPGTuning/RELOAD_CURRENT_UPGRADE_SETTINGS
        inline util::memory_reference<u8> reload_current_upgrade_settings { 0x01112D0C };

        // what frame_advance last reloaded the upgrade settings with; 1 from the start
        inline util::memory_reference<i32> unk_00f4db94 { 0x00F4DB94 };

        // .data 1; frame_advance takes the velocity from slot 95 while it's set, else from the physical interface
        inline util::memory_reference<u8> unk_00f4de2c { 0x00F4DE2C };

        // set to 0 every hero frame
        inline util::memory_reference<u8> unk_01112c58 { 0x01112C58 };

        // "frame_advance(num)", the master object's function frame_advance calls
        inline util::memory_reference<string_hash> frame_advance_num { 0x01112D5C };

        // two of the {3.75, 0.0035} pairs at 0x00F4D590, rewritten every hero frame
        inline util::memory_reference<f32> unk_00f4d590 { 0x00F4D590 };
        inline util::memory_reference<f32> unk_00f4d594 { 0x00F4D594 };
        inline util::memory_reference<f32> unk_00f4d5a0 { 0x00F4D5A0 };
        inline util::memory_reference<f32> unk_00f4d5a4 { 0x00F4D5A4 };
    } // references

    // one of the two at +0x638
    struct spiderman_rvb_unk_0638 {
        u32      unk_00;            // the hero's combat frame only hands slot 368 a record with it set
        u8       unk_04[0x4C];      // sub_60D3E0 gives its position
        bool     unk_50;            // frame_advance copies the rest into its unk_0ca8 record while set
        u8       pad_51[0x03];
        vector3d unk_54;
        f32      unk_60;
    };

    // one of the two at +0xCA8, filled from the matching +0x638 object
    struct spiderman_rvb_unk_0ca8 {
        u8       reserved_00[0x04];
        vector3d unk_04;
        vector3d unk_10;
        f32      unk_1c;
    };

    // the hero's player interface
    class spiderman_rvb_player_interface : public player_interface {

    public:
        bool                    unk_0530;         // the als's player_in_blacksuit_mode
        bool                    unk_0531;
        u8                      reserved_0532[0x06];
        bool                    unk_0538;
        u8                      reserved_0539[0x07];
        f32                     unk_0540;
        u8                      reserved_0544[0x08];
        u8                      unk_054c[0x30];   // advanced every frame (sub_987AC0)
        f32                     unk_057c;         // counted down, held at 0 in mode 8
        u8                      reserved_0580[0x18];
        delta_variable<i32>     unk_0598;         // set to 42 every frame
        u8                      reserved_05a0[0x90];
        f32                     horizontal_speed; // the velocity's length in x and z
        void*                   unk_0634;
        spiderman_rvb_unk_0638* unk_0638[2];
        u8                      reserved_0640[0x08];
        void*                   unk_0648[8];      // each advanced every frame (sub_994200)
        u8                      reserved_0668[0x0C];
        bool                    unk_0674;         // consumed by frame_advance, which then times it in unk_0678
        u8                      reserved_0675[0x03];
        f32                     unk_0678;
        u8                      reserved_067c[0x40];
        vector3d                last_position;    // the owner's position as frame_advance left it
        u8                      reserved_06c8[0x168];
        f32                     unk_0830;         // the seven timers frame_advance counts down
        f32                     unk_0834;
        f32                     unk_0838;
        f32                     unk_083c;
        f32                     unk_0840;
        u8                      reserved_0844[0x04];
        f32                     unk_0848;
        f32                     unk_084c;
        u8                      reserved_0850[0x1CC];
        i32                     unk_0a1c;         // set to 106 every frame
        u8                      reserved_0a20[0x134];
        f32                     unk_0b54;         // slot 486 runs when it runs out
        f32                     unk_0b58;         // slot 488 runs when it runs out
        u8                      reserved_0b5c[0x48];
        u32                     unk_0ba4;         // flags; slots 306 and 307 read bits 0 and 1
        u8                      reserved_0ba8[0x68];
        arch_base_vhandle       unk_0c10;         // flagged 0x10 through its slot 68 once unk_0c14 runs out
        f32                     unk_0c14;
        u8                      reserved_0c18[0x88];
        f32                     unk_0ca0;         // resets the combat target capacities when it runs out
        f32                     fall_timer;       // how long the owner has been below y -8 without standing
        spiderman_rvb_unk_0ca8* unk_0ca8;         // two of them
        u8                      reserved_0cac[0x04];

        explicit spiderman_rvb_player_interface(mash::generating_vtable_version so_do_nothing) : player_interface(so_do_nothing) {}

        ~spiderman_rvb_player_interface() override;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool get_ifc_num(const string_hash &name, f32* value, bool document) override;
        bool set_ifc_num(const string_hash &name, f32 value, bool document) override;
        bool get_ifc_vec(const string_hash &name, vector3d* value, bool document) override;

        void set_owner(actor* owner) override;

        f32               unk_021() override;
        i32               unk_022(f32 unk) override;
        bool              unk_023(i32 unk) override;
        void              unk_024(actor* owner) override;
        void*             unk_025(f32 delta_t) override;
        bool              unk_026() override;
        bool              unk_027(i32 unk) override;
        bool              unk_028(i32 unk) override;
        bool              unk_029() override;
        f32               unk_030() override;
        i32               unk_031(i32 unk) override;
        i32               unk_032(i32 unk) override;
        i32               unk_034(i32 unk) override;
        i32               unk_035(i32 unk) override;
        void*             unk_036(void* unk) override;
        void              set_zip_override_anchor_num(f32 num) override;
        f32               unk_038() override;
        i32               unk_039(i32 unk) override;
        void*             unk_040(void* unk) override;
        void*             unk_041(void* unk, i32 unk_2) override;
        i32               unk_042() override;
        void              unk_043(i32 unk) override;
        bool              unk_044(i32 unk) override;
        bool              unk_045() override;
        void              unk_046(i32 unk) override;
        void              unk_047(u32 unk) override;
        void*             unk_048(void* unk) override;
        bool              unk_049(i32 unk) override;
        i32               unk_050(arch_base_vhandle target) override;
        i32               unk_051(i32 unk, i32 unk_2, i32 unk_3) override;
        void*             unk_052(void* unk) override;
        void*             unk_053(void* unk, void* unk_2) override;
        void              unk_054(i32 unk, i32 unk_2) override;
        void*             unk_055(void* unk) override;
        f32               unk_056() override;
        i32               unk_057() override;
        i32               unk_060(arch_base_vhandle target) override;
        arch_base_vhandle unk_061() override;
        void*             unk_062(void* unk) override;
        i32               unk_063(i32 unk) override;
        i32               unk_064(i32 unk) override;
        void*             unk_065(void* unk) override;
        void              unk_066(i32 unk, i32 unk_2, i32 unk_3) override;
        i32               unk_067(i32 unk) override;
        bool              unk_068(i32 unk) override;
        bool              unk_069() override;
        i32               unk_070(i32 unk) override;
        void*             unk_071(void* unk) override;
        bool              unk_072() override;
        bool              unk_074() override;
        i32               unk_076() override;
        i32               unk_077() override;
        void              set_swing_override_anchor(arch_base_vhandle anchor) override;
        void              set_zip_override_anchor(arch_base_vhandle anchor) override;
        arch_base_vhandle unk_080() override;
        void              unk_081(f32 unk) override;
        i32               unk_082(f32 unk) override;
        f32               unk_084() override;
        void              unk_086(i32 unk) override;
        bool              unk_087() override;
        bool              unk_088(i32 unk) override;
        bool              map_controls(i32 config, bool force) override;
        void              render_phase() override;
        void              update_interface() override;
        void              frame_advance(f32 delta_t) override;
        void              unk_094(f32 delta_t) override;
        bool              unk_096(i32 unk) override;
        bool              unk_097() override;
        bool              unk_098(i32 unk) override;
        i32               unk_099() override;
        i32               unk_100() override;
        i32               unk_101() override;
        i32               unk_107() override;
        i32               unk_108(i32 unk) override;
        i32               unk_113() override;
        bool              unk_114(i32 unk) override;
        i32               unk_115() override;
        bool              unk_116(i32 unk) override;
        i32               unk_118() override;
        bool              unk_119(i32 unk) override;
        i32               unk_121() override;
        bool              unk_125() override;
        bool              unk_126() override;
        bool              unk_129(bool unk) override;
        bool              unk_130() override;
        bool              unk_131() override;
        bool              unk_134(i32 unk) override;
        bool              unk_135(i32 unk) override;
        i32               unk_136() override;
        i32               unk_137() override;
        i32               unk_139() override;
        i32               unk_141() override;
        void              unk_145(f32 unk) override;
        void              unk_146(f32 unk) override;
        void              unk_147(f32 unk) override;
        f32               unk_148() override;
        f32               unk_149() override;
        bool              unk_152() override;
        bool              unk_153(i32 unk) override;
        bool              unk_156() override;
        i32               unk_157() override;
        i32               unk_158(i32 unk) override;
        void              unk_163() override;
        void              unk_164() override;
        i32               unk_165() override;
        i32               unk_166() override;
        bool              unk_167(i32 unk) override;
        bool              unk_168() override;
        bool              unk_169(i32 unk) override;
        bool              unk_170() override;
        bool              unk_171(i32 unk) override;
        bool              unk_172() override;
        bool              unk_173(i32 unk) override;
        bool              unk_174() override;
        bool              unk_175(i32 unk) override;
        bool              unk_176() override;
        bool              unk_177(i32 unk) override;
        bool              unk_178() override;
        bool              unk_179(i32 unk) override;
        bool              unk_180() override;
        bool              unk_188() override;
        bool              unk_192() override;
        bool              unk_193(i32 unk) override;
        bool              unk_194(f32 unk) override;
        i32               unk_195() override;
        i32               unk_196(i32 unk) override;
        i32               unk_197() override;
        void              unk_198() override;
        i32               unk_201(f32 unk) override;
        i32               unk_202() override;
        void              unk_203() override;
        i32               unk_204(i32 unk) override;
        void*             unk_205(void* unk) override;
        i32               unk_206(f32 unk, entity* unk_2, i32 unk_3) override;
        void              unk_207() override;
        void              unk_208(i32 unk) override;
        void              unk_209(i32 unk) override;
        f32               unk_210() override;
        i32               unk_215() override;
        void              init_attacks() override;
        void              update_attacks(f32 delta_t) override;
        void              init_tricks() override;
        bool              unk_237(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4, i32 unk_5, i32 unk_6, i32 unk_7) override;
        void*             unk_243(void* unk) override;
        void*             unk_244(void* unk) override;
        i32               unk_245() override;
        f32               unk_246() override;
        f32               unk_247() override;
        bool              unk_248() override;
        bool              unk_249(i32 unk) override;
        f32               unk_250() override;
        f32               unk_251() override;
        void              unk_252(i32 unk) override;
        f32               unk_253() override;
        void              unk_254(i32 unk) override;
        void              unk_255(f32 delta_t) override;
        void              unk_256() override;
        bool              unk_257() override;
        void*             unk_258(void* unk) override;
        i32               unk_262(i32 unk, i32 unk_2) override;
        bool              unk_263(void* unk, i32 unk_2) override;
        bool              unk_264(f32 delta_t, void* unk_2, i32 unk_3, i32 unk_4, i32 unk_5) override;
        bool              unk_265() override;
        void              unk_266(i32 unk, i32 unk_2, i32 unk_3) override;
        void              unk_267() override;
        bool              unk_268() override;
        i32               unk_269(i32 unk) override;
        bool              unk_270() override;
        bool              unk_271() override;
        bool              unk_272() override;
        entity*           unk_273(entity* target) override;
        bool              unk_284() override;
        bool              unk_305() override;
        i32               unk_306() override;
        i32               unk_307() override;
        f32               unk_308() override;
        f32               unk_310() override;
        f32               unk_315() override;
        bool              unk_319(f32 delta_t, game_button* button, i32 unk_3) override;
        void*             unk_320() override;
        void*             unk_321(const line_info* unk) override;
        bool              unk_322(i32 unk) override;
        void*             unk_323(void* unk) override;
        i32               unk_324(i32 unk) override;
        f32               unk_325() override;
        bool              unk_326() override;
        bool              unk_327(i32 unk) override;
        void              unk_330(f32 unk, i32 unk_2) override;
        void              unk_331(f32 unk, i32 unk_2) override;
        void              unk_332(f32 unk, i32 unk_2) override;
        i32               unk_333(f32 unk, i32 unk_2) override;
        void              unk_334(f32 unk, i32 unk_2) override;
        void              unk_335(f32 unk, i32 unk_2) override;
        void              unk_336(f32 unk, i32 unk_2) override;
        i32               unk_337() override;
        i32               unk_338() override;
        i32               unk_339() override;
        bool              unk_340() override;
        i32               unk_341() override;
        i32               unk_342() override;
        i32               unk_343() override;
        void              unk_344(f32 unk) override;
        void              unk_345(i32 unk) override;
        void              unk_346(f32 unk) override;
        void              unk_347(f32 unk) override;
        void              unk_348(f32 unk) override;
        void              unk_349(i32 unk) override;
        void              unk_350(i32 unk) override;
        void              unk_351(i32 unk) override;
        void              unk_352(i32 unk) override;
        void              unk_353(i32 unk) override;
        void              unk_354(i32 unk) override;
        void              unk_355(i32 unk) override;
        void              unk_356(i32 unk) override;
        void              unk_357(i32 unk) override;
        void              unk_358(i32 unk) override;
        void              unk_359(i32 unk) override;
        void              unk_360(i32 unk) override;
        void              unk_361(i32 unk) override;
        void*             unk_363(void* unk, void* unk_2, void* unk_3, void* unk_4, void* unk_5, void* unk_6) override;
        void*             unk_364(void* unk, void* unk_2, void* unk_3, void* unk_4, void* unk_5, void* unk_6, void* unk_7) override;
        i32               unk_365(void* unk, void* unk_2) override;
        void*             unk_366(void* unk, void* unk_2, void* unk_3, void* unk_4) override;
        i32               unk_367(void* unk, void* unk_2) override;
        void              unk_368(f32 unk, void* record) override;
        void              unk_369(i32 unk, i32 unk_2) override;
        bool              unk_370(i32 unk, i32 unk_2, i32 unk_3) override;
        i32               unk_371(i32 unk, i32 unk_2) override;
        i32               unk_373() override;
        f32               unk_374() override;
        f32               unk_375() override;
        i32               unk_376() override;
        bool              unk_377(vector3d* point) override;
        bool              unk_378() override;
        i32               unk_379() override;
        i32               unk_380(i32 unk) override;
        f32               unk_381(void* unk, i32 unk_2, f32 unk_3) override;
        bool              unk_382(i32 unk) override;
        i32               unk_383() override;
        bool              unk_384() override;
        i32               unk_385() override;
        i32               unk_386(f32 unk, i32 unk_2) override;
        void*             unk_387() override;
        bool              unk_388() override;
        bool              unk_389(void* unk, f32 unk_2) override;
        bool              unk_390() override;
        bool              unk_391(i32 unk, i32 unk_2) override;
        bool              unk_392(i32 unk) override;
        bool              unk_393(f32 unk) override;
        i32               unk_394() override;
        i32               unk_395() override;
        i32               unk_396(i32 unk) override;
        i32               unk_398(i32 unk) override;
        void*             unk_399(i32 index) override;
        void              unk_400(polytube* tube, f32 radius) override;
        i32               unk_401() override;
        i32               unk_402(i32 unk, i32 unk_2) override;
        f32               unk_403() override;
        f32               unk_404(f32 unk) override;
        f32               unk_405(f32 unk) override;
        f32               unk_406(f32 unk) override;
        f32               unk_407(f32 unk) override;
        void              unk_408(i32 unk, i32 unk_2) override;
        void              unk_409(i32 unk, i32 unk_2, i32 unk_3) override;
        void              unk_410(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4) override;
        i32               unk_411(i32 unk, i32 unk_2) override;
        void              unk_412(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4, i32 unk_5, i32 unk_6) override;
        i32               unk_413(i32 unk, i32 unk_2, void* unk_3) override;
        f32               unk_414() override;
        f32               unk_415() override;
        f32               unk_416() override;
        i32               unk_417() override;
        i32               unk_418() override;
        i32               unk_419() override;
        i32               unk_420() override;
        i32               unk_421() override;
        i32               unk_422() override;
        i32               unk_423() override;
        f32               unk_425() override;
        f32               unk_427() override;
        f32               unk_429() override;
        f32               unk_432() override;
        f32               unk_433() override;
        f32               unk_434() override;
        f32               unk_438() override;
        void              unk_439(i32 unk) override;
        f32               unk_440() override;
        f32               unk_441() override;
        f32               unk_442() override;
        f32               unk_443() override;
        f32               unk_444() override;
        f32               unk_445() override;
        f32               unk_446() override;
        f32               unk_447() override;
        f32               unk_448() override;
        f32               unk_449() override;
        f32               unk_450() override;
        f32               unk_451() override;
        f32               unk_452() override;
        i32               unk_453(i32 unk, i32 unk_2, i32 unk_3) override;
        f32               unk_454() override;
        i32               unk_455(i32 unk) override;
        bool              unk_456() override;
        bool              unk_457(i32 unk) override;
        void              unk_458(void* unk, void* unk_2, f32 unk_3) override;
        i32               unk_459() override;
        i32               unk_460(f32 unk) override;
        bool              unk_461() override;
        bool              unk_462() override;
        bool              unk_463() override;
        i32               unk_464() override;
        void              unk_465() override;
        bool              unk_466() override;
        void*             unk_467() override;
        void              unk_468() override;
        bool              unk_469() override;
        bool              unk_470() override;
        i32               unk_471() override;
        void              unk_472() override;
        void*             unk_483(void* unk, i32 unk_2) override;
        bool              unk_484() override;
        bool              unk_485() override;
        void              unk_486(bool unk, f32 unk_2) override;
        bool              unk_487() override;
        bool              unk_488(bool unk, f32 unk_2) override;
        i32               unk_489() override;
        i32               unk_490() override;
        f32               unk_491() override;
        void              unk_492(i32 unk) override;
        i32               unk_493() override;
        i32               unk_494() override;
        i32               unk_495() override;
        i32               unk_496() override;
        i32               unk_497() override;
        i32               unk_498() override;
        i32               unk_499() override;
        i32               unk_500() override;
        i32               unk_501() override;
        f32               unk_502() override;
        i32               unk_503() override;
        bool              unk_504(i32 unk) override;
        void              unk_506(i32 unk) override;
        void*             unk_507(void* unk) override;
        i32               unk_508(i32 unk) override;
        i32               unk_509() override;
        void              unk_510(i32 unk) override;
        bool              unk_511() override;
        bool              unk_512(i32 unk) override;
        bool              unk_513(i32 unk) override;
        void*             unk_514(i32 unk) override;
        bool              unk_515() override;
        i32               unk_517() override;
        bool              unk_518(i32 unk) override;
        bool              unk_519() override;
        bool              unk_520() override;
        i32               unk_522() override;
        void              unk_523(bool unk, bool unk_2) override;
        bool              unk_524() override;
        i32               unk_525(i32 unk, i32 unk_2) override;
        i32               unk_526() override;
        void              unk_527(f32 unk) override;
        line_info*        unk_528() override;
        void*             unk_529(i32 unk) override;
        bool              unk_530(f32 unk) override;
        bool              unk_531(i32 unk) override;
        bool              unk_532(const line_info* unk) override;
        void              unk_534(i32 unk, f32 unk_2) override;

        virtual bool  unk_536(i32 unk);
        virtual bool  unk_537(void* unk);
        virtual bool  unk_538(i32 unk);
        virtual f32   unk_539();
        virtual void  unk_540(f32 unk);             // the cut scene player's time step while it plays
        virtual f32   unk_541(i32 unk);
        virtual void* unk_542(void* unk, i32 unk_2);
        virtual bool  unk_543();
        virtual i32   unk_544(f32 delta_t);
        virtual bool  unk_545();
        virtual i32   unk_546(i32 unk);
        virtual bool  unk_547();
        virtual f32   unk_548(f32 unk);
        virtual f32   unk_549(f32 unk);
        virtual i32   unk_550(void* unk, i32 unk_2);
        virtual void* unk_551(void* unk);
        virtual i32   unk_552(void* unk, i32 unk_2);
        virtual void  unk_553(i32 unk, i32 unk_2, i32 unk_3);
        virtual f32   unk_554();
        virtual void  unk_555(f32 unk);
        virtual void* unk_556(void* unk);
        virtual void* unk_557(i32 unk);
        virtual void  unk_558(f32 unk);
        virtual void* unk_559(f32 unk);
        virtual void* unk_560(f32 unk);
        virtual i32   unk_561(void* unk, void* unk_2);
        virtual f32   unk_562(i32 unk);
        virtual f32   unk_563(f32 unk, f32 unk_2);
        virtual i32   unk_564(f32 unk);
        virtual i32   unk_565(f32 unk);
        virtual void  unk_566(i32 unk);
        virtual void* unk_567(f32 unk);
        virtual void  unk_568();
        virtual void  unk_569();
        virtual bool  unk_570(i32 unk, void* unk_2, i32 unk_3, i32 unk_4, f32 unk_5);
        virtual i32   unk_571(i32 unk);
        virtual void  unk_572(f32 unk, bool black); // switches the suit's behaviours
        virtual i32   unk_573();
        virtual bool  unk_574();
        virtual bool  unk_575();
        virtual bool  unk_576();
        virtual i32   unk_577();
        virtual bool  unk_578();
        virtual i32   unk_579(void* unk, void* unk_2, void* unk_3, i32 unk_4, i32 unk_5);
        virtual void* unk_580(f32 unk);
        virtual bool  unk_581(i32 unk, i32 unk_2);
        virtual i32   unk_582(f32 unk);
        virtual i32   unk_583();
        virtual f32   unk_584();
        virtual void  unk_585();
        virtual void* unk_586(void* unk);
        virtual i32   unk_587(i32 unk, i32 unk_2);
        virtual i32   unk_588(void* unk, i32 unk_2);
        virtual i32   unk_589(i32 unk, f32 unk_2);
        virtual void* unk_590(void* unk);
        virtual i32   unk_591(i32 unk, i32 unk_2);
        virtual i32   unk_592(void* unk, i32 unk_2);
        virtual void  unk_593(void* unk, i32 unk_2, f32 unk_3);
        virtual i32   unk_594(void* unk, i32 unk_2);
        virtual i32   unk_595(f32 unk);

        static string_hash my_type_key;
    };

    ASSERT_OFFSETOF(spiderman_rvb_unk_0638, unk_50, 0x50);
    ASSERT_OFFSETOF(spiderman_rvb_unk_0638, unk_54, 0x54);
    ASSERT_OFFSETOF(spiderman_rvb_unk_0638, unk_60, 0x60);

    ASSERT_SIZEOF  (spiderman_rvb_unk_0ca8,         0x20);
    ASSERT_OFFSETOF(spiderman_rvb_unk_0ca8, unk_04, 0x04);
    ASSERT_OFFSETOF(spiderman_rvb_unk_0ca8, unk_10, 0x10);
    ASSERT_OFFSETOF(spiderman_rvb_unk_0ca8, unk_1c, 0x1C);

    ASSERT_SIZEOF  (spiderman_rvb_player_interface,                   0xCB0);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0530,          0x530);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0538,          0x538);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0540,          0x540);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_054c,          0x54C);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_057c,          0x57C);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0598,          0x598);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, horizontal_speed, 0x630);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0638,          0x638);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0648,          0x648);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0674,          0x674);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0678,          0x678);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, last_position,    0x6BC);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0830,          0x830);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0848,          0x848);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0a1c,          0xA1C);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0b54,          0xB54);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0ba4,          0xBA4);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0c10,          0xC10);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0ca0,          0xCA0);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, fall_timer,       0xCA4);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0ca8,          0xCA8);
} // treyarch
