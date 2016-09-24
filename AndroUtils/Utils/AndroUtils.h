#pragma once


#include "AndroTypes.h"
#include <string>
#include <algorithm>
#include <assert.h>
#include <memory>

#ifdef _WIN32
#define ASSERT _ASSERTE
#else
#define ASSERT(x,...) assert(x)

namespace std
{
	template<typename T, typename... Args>
	std::unique_ptr<T> make_unique(Args&&... args) {
	    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
	}
	
}
#define FALSE false
#define TRUE true

#endif

#define to_lower( s ) std::transform(s.begin(), s.end(), s.begin(), ::tolower);

#if defined _DEBUG
#define VERIFY ASSERT
#else
#define VERIFY(expr) (expr)
#endif

namespace andro
{
	afloat random_float(afloat max = 1.0f);

	afloat GetTimeMS();
	afloat GetTimeS();
}
