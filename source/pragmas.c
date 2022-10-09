// ========================================================
//
// FILE:			/source/pragmas.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Math ported from assembly in pragmas.h
//
// LAST EDITED:		October 8th, 2022
//
// ========================================================

#include "rex.h"

#ifdef REX_SDL

INT64 mul32_64(INT32 i1, INT32 i2)
{
	return (INT64)i1 * i2;
}

int scale(INT32 input1, INT32 input2, INT32 input3)
{
	return (int)(mul32_64(input1, input2) / (INT64)input3);
}

int scaleup(INT32 input1, INT32 input2, INT32 input3)
{
	return (int)((mul32_64(input1, input2) + input3 - 1) / (INT64)input3);
}

int mulscale(INT32 input1, INT32 input2, INT32 input3)
{
	return (int)(mul32_64(input1, input2) >> input3);
}

int dmulscale(INT32 input1, INT32 input2, INT32 input3, INT32 input4, INT32 input5)
{
	return (int)((mul32_64(input1, input2) + mul32_64(input3, input4)) >> input5);
}

int tmulscale(INT32 i1, INT32 i2, INT32 i3, INT32 i4, INT32 i5, INT32 i6, INT32 shift)
{
	return (int)((mul32_64(i1, i2) + mul32_64(i3, i4) + mul32_64(i5, i6)) >> shift);
}

INT32 divscale(INT32 i1, INT32 i2, INT32 i3)
{
	return (INT32)(((INT64)i1 << i3) / i2);
}

#endif
