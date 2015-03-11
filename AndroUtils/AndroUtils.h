
#pragma once

#include"Precompiled.h"


#define ASSERT _ASSERTE


#if defined _DEBUG
 #define VERIFY ASSERT
#else
 #define VERIFY(expr) (expr)
#endif

