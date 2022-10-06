// ========================================================
//
// FILE:			/source/inc/math_x.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Fixed-point math header.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

/*
** Fixed point maths.
** Thanks to the KenBuild, Doom and Decent for some great ideas.
*/

#define FRACCEIL 0xFFFF

#define f2i(a) ((a)>>16)
#define i2f(a) ((a)<<16)
#define f2fl(a) (((float)(a))/ i2f(1))
#define fl2f(a) ((int)((a)*i2f(1)))

#define fixceil(A) (((A)+0xFFFF)>>16)

extern int sintable[2048];

#define fixsin(A) sintable[(A)&2047]
#define fixcos(A) sintable[((A)+512)&2047]

int fixinv(int);
int fixmul(int, int);
int fixdiv(int, int);
int fixdot3(int, int, int, int, int, int);

#ifdef __WATCOMC__

#pragma aux fixinv = \
		"xor eax, eax" \
		"mov edx, 1" \
		"idiv ebx" \
		parm caller [ebx] \
		value [eax] \
		modify [eax ebx edx];
	
#pragma aux fixmul parm [eax] [edx] = \
	"imul edx" \
	"shrd eax,edx,16";

#pragma aux fixdiv parm [eax] [ebx] modify exact [eax edx] = \
	"mov edx,eax" \
	"sar edx,16" \
	"shl eax,16" \
	"idiv ebx";

#pragma aux fixdot3 = \
	"imul edx", \
	"xchg eax, ebx", \
	"xchg edx, ecx", \
	"imul edx", \
	"add ebx, eax", \
	"adc ecx, edx", \
	"mov eax, esi", \
	"imul edi", \
	"add eax, ebx", \
	"adc edx, ecx", \
	"shrd eax, edx, 16", \
	parm nomemory [eax][edx][ebx][ecx][esi][edi] \
	modify exact [eax ebx ecx edx];

#endif
