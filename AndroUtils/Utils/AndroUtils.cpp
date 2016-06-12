#include "AndroUtils.h"
#include "Trace.h"
#include "unique_handler.h"
#include <stdio.h>
#include <limits.h>
#include <Windows.h>

namespace andro
{
	float random_float(float max )
	{
		unsigned int number;
		rand_s(&number);

		return (float)number / ((float)UINT_MAX + 1) * max;
	}

	float GetTimeMS()
	{
		return (float)GetTickCount();
	}
	float GetTimeS()
	{
		return (float)GetTickCount() / 1000.0f;
	}
}