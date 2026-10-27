#include "retail.hh"
#include "game/filespec.hh"

using namespace treyarch;

// sub_5A7F50
filespec::filespec(const mash::string &source) {
    retail::sub_958B80((u32*)this, (u32*)&source); // filespec::_extract
}
