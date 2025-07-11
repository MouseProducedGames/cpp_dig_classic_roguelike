#pragma once

#include "interned_string.hpp"

struct TAG {};
typedef InternedString<TAG> TagName;

#define DECLARE_TAG(name) DECLARE_INTERNED_STRING(TAG, name)

DECLARE_TAG(PLAYER);
DECLARE_TAG(LARGE);