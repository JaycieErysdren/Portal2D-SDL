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
// DESCRIPTION:		Fixed-point math functions.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

#include "rex.h"

int fixinv(int a)
{
	return fixdiv(i2f(1), a); 
}

int fixmul(int a, int b)
{
	return ((__int64)a * b) >> 16;
}

int fixdiv(int a, int b)
{
	return ((__int64)a << 16) / b;
}

int fixdot3(int a1, int b1, int a2, int b2, int a3, int b3)
{
	return ((__int64)a1 * b1 + (__int64)a2 * b2 + (__int64)a3 * b3) >> 16;
}
