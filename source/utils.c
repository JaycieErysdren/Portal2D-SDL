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

void fail(char *s, ...)
{
	va_list args;
	char failure_header[] = "Failure: ";
	char failure_message[128];

	strcpy(failure_message, failure_header);

	va_start(args, s);
	vsprintf(failure_message + (sizeof(failure_header) - 1), s, args);
	va_end(args);

	_setvideomode(_DEFAULTMODE);
	printf(failure_message);
	exit(EXIT_FAILURE);
}

#endif
