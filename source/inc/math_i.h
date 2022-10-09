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
// LAST EDITED:		October 8th, 2022
//
// ========================================================

int isqrt(int value);

int imin(int a, int b);
#pragma aux imin = \
	"cmp eax, ebx", \
	"jl skipit", \
	"mov eax, ebx", \
	"skipit:", \
	parm nomemory [eax][ebx] \
	modify exact [eax];

int imax(int a, int b);
#pragma aux imax = \
	"cmp eax, ebx", \
	"jg skipit", \
	"mov eax, ebx", \
	"skipit:", \
	parm nomemory [eax][ebx] \
	modify exact [eax];

int isqr(int value);
#pragma aux isqr = \
	"imul eax, eax", \
	parm nomemory [eax] \
	modify exact [eax] \
	value [eax];

int imuldiv(int a, int b, int c);
#pragma aux imuldiv parm [eax] [edx] [ebx] modify exact [eax edx] = \
	"imul edx" \
	"idiv ebx";

void memcpy32(long *Source, long *Destination, int Length);
#pragma aux memcpy32 = " rep movsd " parm [EDI] [ESI] [ECX] modify [EDI ESI ECX];
