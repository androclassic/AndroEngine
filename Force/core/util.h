#ifndef _UTIL_H_
#define _UTIL_H_
#include <stdio.h>
#include <string>

#define WARNING( msg ){ std::string s=msg; \
	printf("%s \n",s.c_str()); }



#endif