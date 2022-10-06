// ========================================================
//
// FILE:			/source/console.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Console handling functions.
//
// LAST EDITED:		October 6th, 2022
//
// ========================================================

#include "rex.h"

void RexConsoleOutText(int x, int y, STRING text)
{
	BYTE* p;
	for (p = &pic_console.scanlines.b[y][x]; (*p++ = *text++) != 0;);
}
