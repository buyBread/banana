#include <cstddef>
#include <doctest/doctest.h>

#include "treyarch/shared/hash/algo.hh"

TEST_SUITE("hash") {
    TEST_CASE("djb2") {
        CHECK(treyarch::hash::djb2("init_sv")       == 0x9AE7AC9Cu);
        CHECK(treyarch::hash::djb2("enx_safehouse") == 0xD4302DCDu);
        CHECK(treyarch::hash::djb2("master")        == 0x055E9FECu);

        // fold uppercase
        CHECK(treyarch::hash::djb2("INIT_SV")       == 0x9AE7AC9Cu);
        CHECK(treyarch::hash::djb2("EnX_SaFeHoUsE") == 0xD4302DCDu);
        CHECK(treyarch::hash::djb2("MaStEr")        == 0x055E9FECu);

        CHECK(treyarch::hash::djb2("actor")               == 0x07138E99u);
        CHECK(treyarch::hash::djb2("als::scripted_state") == 0xB7E96372u);
    }
}
