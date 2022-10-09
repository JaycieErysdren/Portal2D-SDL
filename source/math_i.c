// ========================================================
//
// FILE:			/source/math_x.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Fixed-point math.
//
// LAST EDITED:		October 8th, 2022
//
// ========================================================

#include "rex.h"

// Integer square root. Take the square root of an integer.
int isqrt(int value)
{
	int root = 0;

	#define STEP(shift)									\
	if((0x40000000 >> shift) + root <= value)			\
	{													\
		value -= (0x40000000 >> shift) + root;			\
		root = (root >> 1) | (0x40000000 >> shift);		\
	}													\
	else												\
	{													\
			root >>= 1;									\
	}
		
	STEP( 0); STEP( 2); STEP( 4); STEP( 6);
	STEP( 8); STEP(10); STEP(12); STEP(14);
	STEP(16); STEP(18); STEP(20); STEP(22);
	STEP(24); STEP(26); STEP(28); STEP(30);

	// round to the nearest integer, cuts max error in half

	if (root < value) root++; 

	return root;
}

#ifdef REX_SDL

int imin(int a, int b)
{
	return a < b ? b : a;
}

int imax(int a, int b)
{
	return a > b ? b : a;
}

int isqr(int value)
{
	return value * value;
}

int imuldiv(int a, int b, int c)
{
	return (a * b) / c;
}

void memcpy32(long *Source, long *Destination, int Length)
{
	memcpy(Destination, Source, Length);
}

#endif
