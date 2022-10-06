// ========================================================
//
// FILE:			/source/utils.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Utility functions.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

#include "rex.h"

#ifdef REX_SDL

void say(char* s)
{
	printf("%s\n", s);
}

#endif

#ifdef REX_DOS

void say(char* s)
{
	_outtext(s);
	_outtext("\n");
}

#endif
