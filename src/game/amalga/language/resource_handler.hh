#pragma once

#include "game/amalga/mashable_resource_handler.hh"
#include "game/localized_string_table.hh"

namespace treyarch { namespace amalga {
    using lang_resource_handler = mashable_resource_handler<localized_string_table>;
}} // treyarch::amalga
