// ========================================================
//
// FILE:			/source/console.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Console handling functions.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

// Include global header
#include "portal2d.h"

void ConsoleOutText(int x, int y, STRING text)
{
	BYTE* p;
	for (p = &pic_console.scanlines.b[y][x]; (*p++ = *text++) != 0;);
}
