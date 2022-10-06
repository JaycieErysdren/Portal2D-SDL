// ========================================================
//
// FILE:			/source/inc/math_i.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Integer math.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

int isqrt(int value);

int imin(int, int);
#pragma aux imin = \
	"cmp eax, ebx", \
	"jl skipit", \
	"mov eax, ebx", \
	"skipit:", \
	parm nomemory [eax][ebx] \
	modify exact [eax];

int imax(int, int);
#pragma aux imax = \
	"cmp eax, ebx", \
	"jg skipit", \
	"mov eax, ebx", \
	"skipit:", \
	parm nomemory [eax][ebx] \
	modify exact [eax];

int isqr(int);
#pragma aux isqr = \
	"imul eax, eax", \
	parm nomemory [eax] \
	modify exact [eax] \
	value [eax];

int imuldiv(int, int, int);
#pragma aux imuldiv parm [eax] [edx] [ebx] modify exact [eax edx] = \
	"imul edx" \
	"idiv ebx";

void memcpy32(long *Source, long *Destination, int Length);
#pragma aux memcpy32 = " rep movsd " parm [EDI] [ESI] [ECX] modify [EDI ESI ECX];
