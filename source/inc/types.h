// ========================================================
//
// FILE:			/source/inc/types.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Simple and complex datatypes.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

// Simple datatypes
typedef void VOID;
typedef long LONG;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

typedef char STRING[256];
typedef char PATH[1024];

// Complex datatypes
typedef struct
{
	LONG x, y;
} POINT;

typedef struct
{
	LONG x1, y1, x2, y2;
} RECT;

typedef struct
{
	LONG x, xx, xxx;
	LONG y, yy, yyy;
	LONG z, zz, zzz;
} MOTION;

typedef struct
{
	LONG x, y, z;
} VECTOR;

typedef LONG PLANE[4];
typedef PLANE MATRIX[4];

// JAYCIE 2022-10-03 - this is supposed to be included automatically from
// the file "climits" in /watcom/h/, but it doesn't work for some reason.
#ifdef _M_I86
	#define INT_MIN			(-32767-1) // minimum value of an int
	#define INT_MAX			32767 // maximum value of an int
	#define UINT_MAX		65535U // maximum value of an unsigned int
#else
	#define INT_MIN			(-2147483647-1) // minimum value of an int
	#define INT_MAX			2147483647 // maximum value of an int
	#define UINT_MAX		4294967295U // maximum value of an unsigned int
#endif
