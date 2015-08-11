#pragma once


#ifndef SINGLETON
#define SINGLETON(CLASS)		\
private:						\
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