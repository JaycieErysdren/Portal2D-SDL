
/* 
** The contents of this file are subject to the Mozilla Public License
** Version 1.1 (the "License"); you may not use this file except in compliance
** with the License. You may obtain a copy of the License at
** http://www.mozilla.org/MPL/
** 
** Software distributed under the License is distributed on an "AS IS" basis,
** WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
** the specific language governing rights and limitations under the License.
*/

#ifndef __TYPES_H__
#define __TYPES_H__

/*
** Include ANSIC Headers.
*/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <memory.h>
#include <math.h>

#undef min
#undef max
/*
** Simple Datatypes
*/
typedef void VOID;
typedef long LONG;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

typedef char STRING[256];
typedef char PATH[1024];

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

typedef BYTE CLUT[256][256];
typedef BYTE PALLETE[256][3];
typedef BYTE LIGHTMAP[32][256];
typedef BYTE TEXTURE66[64][64];

#define TEXEL66(A,X,Y) *((BYTE*)A+((Y>>10)&(63<<6))+((X>>16)&63))
  
#endif

