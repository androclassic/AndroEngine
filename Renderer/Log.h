#pragma once

#include "../AndroUtils/Utils/Trace.h"

#if ENABLE_DEBUG
	#define LOG_MSG(...) TRACE(__FILE__, __LINE__)
#else
	#define LOG_MSG(...)
#endif
