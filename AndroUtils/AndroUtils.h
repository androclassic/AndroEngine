
#pragma once

#include"Precompiled.h"

#include "AndroTypes.h"
#include <string>
#include <algorithm>

#define ASSERT _ASSERTE


#define to_lower( s ) std::transform(s.begin(), s.end(), s.begin(), ::tolower);

#if defined _DEBUG
 #define VERIFY ASSERT
#else
 #define VERIFY(expr) (expr)
#endif

