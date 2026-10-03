#include "treyarch/amalga/resource_versions.hh"

using namespace treyarch;

// sub_72DFC0
bool amalga::resource_versions::verify(string_hash) const {
    // the newer/older reports that used the file name are stripped, and auto_version isn't checked
    return pack_version      == references::resource_pack_version.read()           &&
           entity_version    == references::resource_entity_mash_version.read()    &&
           nonentity_version == references::resource_nonentity_mash_version.read() &&
           raw_version       == references::resource_raw_mash_version.read();
}
