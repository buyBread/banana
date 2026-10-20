#pragma once

#include "treyarch/amalga/merged_apk/file.hh"
#include "treyarch/nal/anim.hh"
#include "treyarch/nal/scene_anim.hh"
#include "treyarch/nal/transport_list.hh"
#include "treyarch/shared/fixed_string.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace nal {
    void init();

    transport_list* get_skeleton(fixed_string* name);
    
    anim*       get_anim(fixed_string* name);
    scene_anim* get_scene_anim(fixed_string* name);

    void load_anim(amalga::merged_apk::file*       owner,
                   amalga::merged_apk::file_entry* entry,
                   void**                          mapped_sections,
                   void*                           user_data);

    void remove_anim(amalga::merged_apk::file*       owner,
                     amalga::merged_apk::file_entry* entry,
                     void**                          mapped_sections,
                     void*                           user_data);

    void load_scene_anim(amalga::merged_apk::file*       owner,
                         amalga::merged_apk::file_entry* entry,
                         void**                          mapped_sections,
                         void*                           user_data);

    namespace references {
        inline util::memory_reference<bool> skip_linking            { 0x01115B68 };
        inline util::memory_reference<bool> skip_scene_anim_linking { 0x01115B69 };
    } // references
}} // treyarch::nal
