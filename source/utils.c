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

void say(char* s)
{
	#ifdef REX_SDL
		printf("%s\n", s);
	#endif

	#ifdef REX_DOS
		_outtext(s);
		_outtext("\n");
	#endif
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

	RexGraphicsRemove();
	printf(failure_message);
	exit(EXIT_FAILURE);
}
