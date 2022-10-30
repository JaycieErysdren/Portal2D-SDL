// ========================================================
//
// FILE:			/source/utils.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Utility functions.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

// Include global header
#include "portal2d.h"

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

	GraphicsRemove();
	printf(failure_message);
	exit(EXIT_FAILURE);
}

void fskip(FILE *fp, int num_bytes)
{
	int i;
	for (i = 0; i < num_bytes; i++)
		fgetc(fp);
}
