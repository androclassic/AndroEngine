#include "AndroUtils.h"
#include "Trace.h"
#include "unique_handler.h"
#include <stdio.h>
#include <limits.h>
#include <Windows.h>

namespace andro
{
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
}