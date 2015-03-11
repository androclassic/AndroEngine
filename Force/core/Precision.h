   /*Georgescu Andrei	*	
	*		 Force		*
	*		 2012		*/ 

#ifndef _PRECISION_H_
#define _PRECISION_H_

#include<math.h>
#include <assert.h>

#define real double

#define real_sqrt	sqrt
#define real_pow	pow
#define real_abs	abs
#ifndef PI
#define PI 3.14159265
#endif

#ifndef RadToDeg
#define RadToDeg(x) x * 180 / PI
#endif

#ifndef DegToRad
#define DegToRad(x) x * PI / 180
#endif


#endif