#include "retail.hh"
#include "treyarch/amalga/paths.hh"

using namespace treyarch;

// sub_7EFD90
mash::string amalga::get_amalgatoc_filename(e_platform platform) {
    return *(mash::string*)retail::sub_7EFAB0(platform) + "amalga.toc";
}
