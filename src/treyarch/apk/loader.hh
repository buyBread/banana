#pragma once

#include "treyarch/apk/file.hh"

namespace treyarch { namespace apk {
    file* relocate_file_in_place(void*            image,
                                 data_reference* &resource_references,
                                 u8*             &string_base);

    file* load_file_in_place(void* image);
}} // treyarch::apk
