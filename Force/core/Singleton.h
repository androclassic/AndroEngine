#pragma once


#ifndef SINGLETON_
#define SINGLETON_(CLASS)		\
protected:						\
 CLASS();						\
 ~CLASS();						\
 CLASS(CLASS &rhs);				\
 CLASS& operator=(CLASS& rhs);	\
public:							\
 static CLASS* GetInstance()	\
 {								\
	static CLASS s_instance;	\
	return &s_instance;			\
 }				
#endif 