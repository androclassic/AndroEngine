#include "AndroUtils.h"
#include "Trace.h"
#include "unique_handler.h"
#include <stdio.h>
#include <limits.h>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace andro
{
#ifdef _WIN32
	afloat random_float(afloat max )
	{
		unsigned int number;
		rand_s(&number);

		return (afloat)number / ((afloat)UINT_MAX + 1) * max;
	}

	afloat GetTimeMS()
	{
		return (afloat)GetTickCount();
	}
	afloat GetTimeS()
	{
		return (afloat)GetTickCount() / 1000.0f;
	}
#else
	afloat random_float(afloat max )
	{
		ASSERT(false);
		return (afloat) 0.0f;
	}

	afloat GetTimeMS()
	{
		ASSERT(false);
		return (afloat) 0.0f;
	}
	afloat GetTimeS()
	{
		ASSERT(false);
		return (afloat) 0.0f;
	}
#endif	
}