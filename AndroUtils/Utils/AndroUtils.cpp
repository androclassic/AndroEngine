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
		unsigned int number = rand();

		return (afloat)number / ((afloat)UINT_MAX + 1) * max;
	}

	afloat GetTimeMS()
	{
		  struct timespec now;
		  if (clock_gettime(CLOCK_MONOTONIC, &now))
		    return 0;
	
		  return now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0;
	}
	afloat GetTimeS()
	{
	  struct timespec now;
	  if (clock_gettime(CLOCK_MONOTONIC, &now))
	    return 0;

	  return now.tv_sec;	
	}
#endif	
}