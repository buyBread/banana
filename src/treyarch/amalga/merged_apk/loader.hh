#pragma once

#include "treyarch/amalga/merged_apk/file.hh"

namespace treyarch { namespace amalga { namespace merged_apk {
    file* relocate_file_in_place(void*            image,
                                 data_reference* &resource_references,
                                 u8*             &string_base);

    file* load_file_in_place(void* image);
}}} // treyarch::amalga::merged_apk
