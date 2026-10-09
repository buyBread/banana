#include <cstring>

#include "treyarch/chuck/vm/so_data_block.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

// sub_A243D0
void so_data_block::set_to_zero() {
    if (blocksize > 0)
        std::memset(buffer, 0, blocksize);
}
