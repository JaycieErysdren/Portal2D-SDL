// ========================================================
//
// FILE:			/source/math_x.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Fixed-point math.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

// Include global header
#include "portal2d.h"

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

