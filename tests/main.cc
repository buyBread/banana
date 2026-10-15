#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

static_assert(sizeof(void*) == 4, "ensure compile architecture is 32-bit");
