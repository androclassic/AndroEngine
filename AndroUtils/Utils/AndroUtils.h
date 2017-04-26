#pragma once


#include "AndroTypes.h"
#include <string>
#include <algorithm>
#include <assert.h>
#include <memory>

#ifdef _USE_CUDA
#include <host_defines.h>
#include <host_config.h>



#ifndef GLOBAL
#define GLOBAL __global__
#endif

#ifndef DEVICE
#define DEVICE __device__
#endif

#ifndef HOST
#define HOST __host__
#endif

#ifndef CONSTANT
#define CONSTANT __constant__
#endif

#ifndef DEVICE_HOST
#define DEVICE_HOST DEVICE HOST
#endif

DEVICE extern afloat get_random_float();

#else

#ifndef GLOBAL
#define GLOBAL 
#endif

#ifndef DEVICE
#define DEVICE
#endif

#ifndef HOST
#define HOST
#endif

#ifndef CONSTANT
#define CONSTANT
#endif


#ifndef DEVICE_HOST
#define DEVICE_HOST 
#endif

#endif



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
	DEVICE afloat random_float(afloat max = 1.0f);
	DEVICE_HOST afloat GetTimeMS();
	DEVICE_HOST afloat GetTimeS();


}
