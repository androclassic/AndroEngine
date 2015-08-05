//Georgescu Andrei 2011//

#ifndef _TYPES_H_
#define _TYPES_H_

typedef signed long			s64;
typedef signed int			s32;
typedef signed short		s16;
typedef signed char			s8;

typedef unsigned long int	u32;
typedef unsigned short		u16;
typedef unsigned char		u8;

typedef double				f64;
typedef bssFloat				f32;


#ifndef foreach
#define foreach(it,ARRAY) for(decltype ((ARRAY).begin()) it=((ARRAY).begin());it!=((ARRAY).end());it++)
#endif

#endif