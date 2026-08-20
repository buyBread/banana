#pragma once

#define CONCAT_(a, b) a##b
#define CONCAT(a, b) CONCAT_(a, b)
#define UNIQUE_NAME(base) CONCAT(base, __COUNTER__)