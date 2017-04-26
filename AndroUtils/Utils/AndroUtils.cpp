#include "AndroUtils.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>


#ifndef _USE_CUDA
#include "Trace.h"
#include "unique_handler.h"
#endif

#if defined(_USE_CUDA) && !defined(ANDRO_UTILS) ||  !defined(_USE_CUDA) 

#ifdef _WIN32
#include <Windows.h>
#endif
#ifdef _USE_CUDA
#include <cuda_runtime_api.h>
#endif

namespace andro
{

#ifdef _WIN32

	DEVICE afloat random_float(afloat max)
	{
#ifndef _USE_CUDA
		unsigned int number;
		rand_s(&number);

		return (afloat)number / ((afloat)UINT_MAX + 1) * max;
#else
		return  get_random_float() * max;
#endif
	}

	 DEVICE_HOST afloat GetTimeMS()
	 {
#ifndef _USE_CUDA
		 return (afloat)GetTickCount();
#else
		 return 0.0f; 
#endif
	 }

	 DEVICE_HOST  afloat GetTimeS()
	 {
#ifndef _USE_CUDA
		 return (afloat)GetTickCount() / 1000.0f;
#else
		 return 0.0f; 
#endif
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

#endif //defined(_USE_CUDA) && !defined(ANDRO_UTILS)