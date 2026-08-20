#pragma once

#include <cstddef>

#define ASSERT_FAIL_SANITY "sanity check failed: compare offender against earlier commit?"

#define ASSERT_SIZEOF(x, z) static_assert(sizeof(x) == z, ASSERT_FAIL_SANITY)
#define ASSERT_OFFSETOF(x, y, z) static_assert(offsetof(x, y) == z, ASSERT_FAIL_SANITY)