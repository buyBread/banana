#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "treyarch/mash/string.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the mission log, skills and options stations of the scrapbook
    class igo_3d_scrapbook_station_widget : public igo_3d_widget {

    public:
        virtual void         unk_054() = 0;
        virtual void         unk_055() = 0;
        virtual void         unk_056() = 0;
        virtual void         unk_057() = 0;
        virtual bool         unk_058(bool unk) = 0;
        virtual void         unk_059(i32 unk) = 0;
        virtual void         unk_060(i32 unk) = 0;
        virtual void         unk_061(i32 unk) = 0;
        virtual void         unk_062(i32 unk) = 0;
        virtual void         unk_063(i32 unk) = 0;
        virtual void         unk_064(i32 unk) = 0;
        virtual void         unk_065(i32 unk) = 0;
        virtual void         unk_066(i32 unk) = 0;
        virtual void         unk_067(i32 unk) = 0;
        virtual void         unk_068(i32 unk) = 0;
        virtual void         unk_069(i32 unk) = 0;
        virtual void         unk_070(i32 unk) = 0;
        virtual void         unk_071(i32 unk) = 0;
        virtual void         unk_072(i32 unk) = 0;
        virtual bool         unk_073() = 0;
        virtual mash::string unk_074() = 0;
        virtual i32          unk_075() = 0;
        virtual bool         unk_076() = 0;
        virtual bool         unk_077() = 0;
        virtual bool         unk_078() = 0;
        virtual bool         unk_079() = 0;
        virtual bool         unk_080() = 0;
        virtual bool         unk_081() = 0;
        virtual bool         unk_082() = 0;
    };
} // treyarch
