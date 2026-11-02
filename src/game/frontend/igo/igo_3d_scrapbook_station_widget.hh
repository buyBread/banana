#pragma once

#include "game/frontend/igo/igo_3d_tab_page_widget.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "treyarch/mash/string.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the mission log, skills and options stations of the scrapbook
    class igo_3d_scrapbook_station_widget : public igo_3d_widget {

    public:
        bool page_unlocked; // is_unlocked latches a true argument into it

        virtual void         unk_054() = 0;
        virtual void         unk_055() = 0;
        virtual void         bring_up() = 0;
        virtual void         put_away() = 0;
        virtual bool         is_unlocked(bool unlock) = 0;
        virtual void         unk_059(i32 unk) = 0;
        virtual void         unk_060(i32 unk) = 0;
        virtual void         unk_061(i32 unk) = 0;
        virtual void         unk_062(i32 unk) = 0;
        virtual void         unk_063(i32 unk) = 0;
        virtual void         unk_064(i32 unk) = 0;
        virtual void         unk_065(i32 unk) = 0;
        virtual void         page_on_cancel(i32 input) = 0;
        virtual void         unk_067(i32 unk) = 0;
        virtual void         unk_068(i32 unk) = 0;
        virtual void         select_previous(i32 input) = 0;
        virtual void         select_next(i32 input) = 0;
        virtual void         unk_071(i32 unk) = 0;
        virtual void         unk_072(i32 unk) = 0;
        virtual bool         unk_073() = 0;
        virtual mash::string unk_074() = 0;
        virtual i32          unk_075() = 0;
        virtual bool         unk_076() = 0;
        virtual bool         unk_077() = 0;
        virtual bool         unk_078() = 0; // false on skills and options, true on the mission log
        virtual bool         unk_079() = 0;
        virtual bool         unk_080() = 0;
        virtual bool         has_open_dialog() = 0;
        virtual bool         unk_082() = 0;
    };

    // the second station
    class igo_3d_scrapbook_skills : public igo_3d_scrapbook_station_widget {

    public:
        u8                                 reserved_160[0x08];
        igo_3d_scrapbook_tab_pages_skills* tab_pages;
    };

    ASSERT_OFFSETOF(igo_3d_scrapbook_station_widget, page_unlocked, 0x150);

    ASSERT_OFFSETOF(igo_3d_scrapbook_skills, tab_pages, 0x168);
} // treyarch
